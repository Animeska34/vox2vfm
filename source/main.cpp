#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vox2vfm/error.h"
#include "vox2vfm/vox2vfm.h"

#define READ_ARGUMENT 0
#define READ_DESTINATION 1
#define READ_NAME 2
#define READ_DESCRIPTION 3
#define READ_SCALE 4


int main(int argc, char *argv[]) {
    return vox2vfm("untitled.vox", "untitled.vox.vfm", "test", "test", 1);
    if (argc < 3)
    {
        printf("Usage:\nvox2vfm modelPath -n name -d description -s scale (optional, default: 0) -t destinationPath (optional) -c only check voxFile without conversion");
        return INVALID_ARGUMENT;
    }
    else
    {
        char *dest;
        dest = (char*) malloc(strlen(argv[1]) + 5);
        strcpy(dest, argv[1]);
        strcat(dest, ".vfm");

        char *name = "test";
        char *description = "test";
        float scale = 1;

        int mode = READ_ARGUMENT;
        for(int i = 2; i < argc; i++){
            switch(mode){
                case READ_ARGUMENT:
                    if(strcmp(argv[i], "-n") == 0){
                        mode = READ_NAME;
                    } else if(strcmp(argv[i], "-d") == 0){
                        mode = READ_DESCRIPTION;
                    } else if(strcmp(argv[i], "-t") == 0){
                        mode = READ_DESTINATION;
                    } else if(strcmp(argv[i], "-s") == 0){
                        mode = READ_SCALE;
                    }
                    else if(strcmp(argv[i], "-c") == 0){
                        int res = voxCheck(argv[1]);
                        if(res == 0) {
                            printf("This is a VOX file\n");
                            return OK;
                        }
                        else{
                            printError(res);
                            return res;
                        }
                    } else{
                        printf("Unknown argument: %s", argv[i]);
                        return INVALID_ARGUMENT;
                    }
                    break;
                case READ_NAME:
                    name = argv[i];
                    mode = READ_ARGUMENT;
                    break;
                case READ_DESCRIPTION:
                    description = argv[i];
                    mode = READ_ARGUMENT;
                    break;
                case READ_DESTINATION:
                    dest = argv[i];
                    mode = READ_ARGUMENT;
                    break;
                case READ_SCALE:
                    scale = atof(argv[i]);
                    mode = READ_ARGUMENT;
                    break;
                default:
                    return INVALID_ARGUMENT;
                    break;
            }
        }

        int res = vox2vfm(argv[1], dest, name, description, scale);

        printError(res);
        free(dest);
        return res;
    }
}