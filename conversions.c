//
// Created by batman on 1/14/21.
//

#include "conversions.h"
#include <stdlib.h>
#include "bmp_img.h"
#include <errno.h>
#include <string.h>
#include <math.h>

void grayscale(BMP **bmp_img) {
    for (int y = (*bmp_img)->info_header.height_px - 1; y >= 0; y--) {
        for (int x = 0; x < (*bmp_img)->info_header.width_px; x++) {
            uint8_t gray = ((*bmp_img)->pixel_array[x][y].red + (*bmp_img)->pixel_array[x][y].green +
                            (*bmp_img)->pixel_array[x][y].blue) / 3;
            (*bmp_img)->pixel_array[x][y].red = (*bmp_img)->pixel_array[x][y].green = (*bmp_img)->pixel_array[x][y].blue = gray;
        }
    }
}

// brightness may range from -255 to 255. Negative value would darken the image.
void changeBrightness(BMP **bmp_img, uint16_t brightness) {
    for (int y = (*bmp_img)->info_header.height_px - 1; y >= 0; y--) {
        for (int x = 0; x < (*bmp_img)->info_header.width_px; x++) {
            (*bmp_img)->pixel_array[x][y].red = (*bmp_img)->pixel_array[x][y].red + brightness;
            (*bmp_img)->pixel_array[x][y].blue = (*bmp_img)->pixel_array[x][y].blue + brightness;
            (*bmp_img)->pixel_array[x][y].green = (*bmp_img)->pixel_array[x][y].green + brightness;
        }
    }
}

static Pixel setPixel(const Pixel source) {
    Pixel dest;
    dest.blue = source.blue;
    dest.green = source.green;
    dest.red = source.red;
    return dest;
}

// TODO - creates broken image - probably issue with padding
BMP rotateImage(BMP *bmp_img, double angle) {
    BMP rotated;
    rotated.info_header = bmp_img->info_header;
    rotated.header = bmp_img->header;

    double radians = (M_PI / 180.0) * angle;
    double cosine = cos(radians);
    double sine = sin(radians);

    rotated.info_header.height_px =
            round(fabs(bmp_img->info_header.height_px * cosine) + fabs(bmp_img->info_header.width_px * sine)) + 1;

    rotated.info_header.width_px =
            round(fabs(bmp_img->info_header.width_px * cosine) + fabs(bmp_img->info_header.height_px * sine)) + 1;


    rotated.pixel_array = (Pixel **) malloc(sizeof(Pixel *) * rotated.info_header.width_px);
    for (int i = 0; i < rotated.info_header.width_px; i++) {
        rotated.pixel_array[i] = (Pixel *) malloc(sizeof(Pixel) * rotated.info_header.height_px);
    }
    if (rotated.pixel_array == NULL) {
        fprintf(stderr, "Could not allocate memory: %s\n", strerror(errno));
    }

    int y0org = (int) round(((double) (bmp_img->info_header.height_px + 1) / 2) - 1);
    int x0org = (int) round(((double) (bmp_img->info_header.width_px + 1) / 2) - 1);

    int y0 = (int) round(((double) (rotated.info_header.height_px + 1) / 2) - 1);
    int x0 = (int) round(((double) (rotated.info_header.width_px + 1) / 2) - 1);

    for (int y = bmp_img->info_header.height_px - 1; y >= 0; y--) {
        for (int x = 0; x < bmp_img->info_header.width_px; x++) {
            int i = bmp_img->info_header.height_px - 1 - y - y0org;
            int j = bmp_img->info_header.width_px - 1 - x - x0org;

            int iprime = (int) round(-j * sine + i * cosine);
            int jprime = (int) round(j * cosine + i * sine);

            iprime = y0 - iprime;
            jprime = x0 - jprime;

            if (jprime >= 0 && jprime <= rotated.info_header.width_px && iprime >= 0 &&
                iprime <= rotated.info_header.height_px) {
                rotated.pixel_array[iprime, jprime] = &bmp_img->pixel_array[y][x];
            }
        }
    }
    return rotated;
}
BMP rotateImageRight(BMP *bmp_img) {
    BMP rotated;
    rotated.info_header = bmp_img->info_header;
    rotated.header = bmp_img->header;

    rotated.info_header.height_px = bmp_img->info_header.width_px;
    rotated.info_header.width_px = bmp_img->info_header.height_px;

    rotated.pixel_array = (Pixel **) malloc(sizeof(Pixel *) * rotated.info_header.width_px);
    for (int i = 0; i < rotated.info_header.width_px; i++) {
        rotated.pixel_array[i] = (Pixel *) malloc(sizeof(Pixel) * rotated.info_header.height_px);
    }
    if (rotated.pixel_array == NULL) {
        fprintf(stderr, "Could not allocate memory: %s\n", strerror(errno));
    }

    for (int y = rotated.info_header.height_px - 1; y >= 0; y--) {
        for (int x = 0; x < rotated.info_header.width_px; x++) {
            rotated.pixel_array[x][y] = bmp_img->pixel_array[y][x];
        }
    }
    return rotated;
}
