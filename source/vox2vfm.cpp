#include "vox2vfm/error.h"
#include "vox2vfm/vox2vfm.h"

extern "C" {
#include "voxfield/core/model.h"
}

#define OGT_VOX_IMPLEMENTATION
#include "ogt_vox.h"

#include <stdio.h>
#include <string.h>

typedef struct voxMeta{
    char id[4];
    int ver;
} voxMeta;

int check(struct voxMeta meta){
    if(memcmp(meta.id, "VOX ", 4) == 0){
        return OK;
    }
    else {
        printf("%s %i", meta.id, meta.ver);
        return INVALID_FORMAT;
    }
}

int voxCheck(char* path) {
    if (access(path, F_OK) == 0) {
        FILE *file = fopen(path, "rb");
        struct voxMeta meta;
        fread(&meta, sizeof(struct voxMeta), 1, file);
        fclose(file);
        return check(meta);
    }
    else
    {
        return FILE_NOT_EXISTS;
    }
}

int vox2vfm(char *path, char *dest, char* name, char* description, float scale){
    if (access(path, F_OK) == 0) {
        FILE *file = fopen(path, "rb");
        struct voxMeta meta;
        fread(&meta, sizeof(struct voxMeta), 1, file);
        int res = check(meta);
        if(res == OK) {
            fseek(file, 0, SEEK_END);
            uint32_t buffer_size = ftell(file);
            fseek(file, 0, SEEK_SET);
            uint8_t *buffer = (uint8_t *)malloc(buffer_size);
            fread(buffer, buffer_size, 1, file);
            fclose(file);
            const ogt_vox_scene *scene = ogt_vox_read_scene(buffer, buffer_size);
            free(buffer);

            if (scene->num_models == 1) {
                uint32_t size_x = scene->models[0]->size_x;
                uint32_t size_y = scene->models[0]->size_y;
                uint32_t size_z = scene->models[0]->size_z;

                uint8_t colors[256];
                uint8_t colorsLength;
                //add void
                colors[0] = 0;
                colorsLength = 1;

                uint32_t size_xy = size_x * size_y;
                uint32_t size = size_xy * size_z;

                uint8_t *voxels = (uint8_t*)malloc(size);

                //TODO: optimization, remove repeating palette colors.
                for (uint32_t z = 0; z < scene->models[0]->size_z; z++) {
                    for (uint32_t y = 0; y < scene->models[0]->size_y; y++) {
                        for (uint32_t x = 0; x < scene->models[0]->size_x; x++) {
                            uint32_t mvp = x + y * size_x + z * size_xy;
                            uint32_t vfp = x + y * size_x + z * size_xy;

                            uint8_t color = scene->models[0]->voxel_data[mvp];

                            bool found = false;
                            for (int c = 0; c < colorsLength; c++) {
                                if (color == colors[c]) {
                                    found = true;
                                    voxels[vfp] = c;
                                    break;
                                }
                            }
                            if (!found) {
                                colors[colorsLength] = color;
                                voxels[vfp] = colorsLength;
                                colorsLength++;
                            }
                        }
                    }
                }

                VoxelData *voxelData = (VoxelData*)malloc(sizeof(VoxelData) * colorsLength);
                for (int i = 0; i < colorsLength; i++) {
                    voxelData[i].color.r = scene->palette.color[colors[i]].r;
                    voxelData[i].color.g = scene->palette.color[colors[i]].g;
                    voxelData[i].color.b = scene->palette.color[colors[i]].b;
                    voxelData[i].color.a = scene->palette.color[colors[i]].a;
                }
                //scene->palette.color[]
                Model model = createModel8(voxelData, colorsLength, voxels, (uint16_t) size_x, (uint16_t) size_y, (uint16_t) size_z, name,
                             strlen(name), description, strlen(description), scale);
                VoxfieldResult res = writeModelToFile(model, dest);
                if(res == 0){
                    return  OK;
                }
                else {
                    return SAVE_FALED;
                }
            }
            else if(scene->num_models == 0){
                return NO_MODELS;
            }
            else{
                return TOO_MANY_MODELS;
            }
        }
        else{
            return res;
        }
    }
    else
    {
        return FILE_NOT_EXISTS;
    }
    return 100;
}