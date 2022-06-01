#pragma once
#include <stdio.h>

//error codes
#define OK 0
#define INVALID_ARGUMENT 1
#define FILE_NOT_EXISTS 2
#define INVALID_FORMAT 3
#define NO_MODELS 4
#define TOO_MANY_MODELS 5

static void printError(int error) {
    switch (error) {
        case OK:
            //printf("no errors");
            break;
        case INVALID_ARGUMENT:
            printf("ERROR %i: invalid arguments provided\n", error);
            break;
        case FILE_NOT_EXISTS:
            printf("ERROR %i: File not exists or name is invalid\n", error);
            break;
        case INVALID_FORMAT:
            printf("ERROR %i: Invalid file format. Check it is MagickVoxel (.vox) file\n", error);
            break;
        case NO_MODELS:
            printf("ERROR %i: vox scene has no models\n", error);
            break;
        case TOO_MANY_MODELS:
            printf("ERROR %i: vox has more than one model. Only single model exporting supported\n", error);
            break;
        default:
            printf("ERROR %i\n", error);
            break;
    }
}