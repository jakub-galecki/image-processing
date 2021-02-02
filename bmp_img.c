//
// Created by batman on 1/6/21.
//

#include <stdlib.h>
#include "bmp_img.h"
#include <errno.h>
#include <string.h>

#define GET_PADDING(width) ((width) % 4)

BMP *read_image(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open the file: %s\n", strerror(errno));
    }
    BMP *bmp_image = malloc(sizeof(BMP));
    if (bmp_image == NULL) {
        fprintf(stderr, "Could not allocate memory: %s\n", strerror(errno));
    }
    fread(&(bmp_image->header), sizeof(BMP_Header), 1, file);
    fread(&(bmp_image->info_header), sizeof(BMP_INFOHeader), 1, file);

    bmp_image->pixel_array = (Pixel **) malloc(sizeof(Pixel *) * bmp_image->info_header.width_px);
    for (int i = 0; i < bmp_image->info_header.width_px; i++) {
        bmp_image->pixel_array[i] = (Pixel *) malloc(sizeof(Pixel) * bmp_image->info_header.height_px);
    }
    if (bmp_image->pixel_array == NULL) {
        fprintf(stderr, "Could not allocate memory: %s\n", strerror(errno));
    }
    int padding = GET_PADDING(bmp_image->info_header.width_px);
    for (int y = bmp_image->info_header.height_px - 1; y >= 0; y--) {
        for (int x = 0; x < bmp_image->info_header.width_px; x++) {
            fread(&bmp_image->pixel_array[x][y], sizeof(Pixel), 1, file);
        }
        fseek(file, padding, SEEK_CUR);
    }
    return bmp_image;
}

void print_pixel_array_by_color(BMP *bmp_image, int option) {
    for (int y = bmp_image->info_header.height_px - 1; y >= 0; y--) {
        for (int x = 0; x < bmp_image->info_header.width_px; x++) {
            switch (option) {
                case 0:
                    printf("%d ", bmp_image->pixel_array[x][y].red);
                    break;
                case 1:
                    printf("%d ", bmp_image->pixel_array[x][y].green);
                    break;
                case 2:
                    printf("%d ", bmp_image->pixel_array[x][y].blue);
                    break;
                default:
                    printf("Wrong option, select from 0 - red,1 - green,2 - blue");
                    exit(EXIT_FAILURE);
            }
        }
        printf("\n");
    }
}

void write_image_by_filename(BMP *bmp_image, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Could not open the file: %s\n", strerror(errno));
    }

    int padding = GET_PADDING(bmp_image->info_header.width_px);

    fwrite(&bmp_image->header, sizeof(bmp_image->header), 1, file);
    fwrite(&bmp_image->info_header, sizeof(bmp_image->info_header), 1, file);
    for (int y = bmp_image->info_header.height_px - 1; y >= 0; y--) {
        for (int x = 0; x < bmp_image->info_header.width_px; x++) {
            fwrite(&bmp_image->pixel_array[x][y], sizeof(Pixel), 1, file);
        }
        for (int i = 0; i < padding; i++) {
            fputc(0x00, file);
        }
    }

}