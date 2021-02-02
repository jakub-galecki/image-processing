//
// Created by batman on 1/14/21.
//

#include "conversions.h"
#include <stdlib.h>
#include "bmp_img.h"
#include <errno.h>
#include <string.h>
#include <math.h>

#define GET_PADDING(width) ((width) % 4)
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))

void grayscale(BMP **bmp_img) {
    for (int y = (*bmp_img)->info_header.height_px - 1; y >= 0; y--) {
        for (int x = 0; x < (*bmp_img)->info_header.width_px; x++) {
            uint8_t gray = ((*bmp_img)->pixel_array[x][y].red + (*bmp_img)->pixel_array[x][y].green +
                            (*bmp_img)->pixel_array[x][y].blue) / 3;
            (*bmp_img)->pixel_array[x][y].red = (*bmp_img)->pixel_array[x][y].green = (*bmp_img)->pixel_array[x][y].blue = gray;
        }
    }
}

int smooth(int n) {
    if (n > 255) {
        return 255;
    } else if (n < 0) {
        return 0;
    } else {
        return n;
    }
}

// brightness may range from -255 to 255. Negative value would darken the image.
void changeBrightness(BMP **bmp_img, int brightness) {
    for (int y = (*bmp_img)->info_header.height_px - 1; y >= 0; y--) {
        for (int x = 0; x < (*bmp_img)->info_header.width_px; x++) {
            (*bmp_img)->pixel_array[x][y].red = smooth((*bmp_img)->pixel_array[x][y].red + brightness);
            (*bmp_img)->pixel_array[x][y].blue = smooth((*bmp_img)->pixel_array[x][y].blue + brightness);
            (*bmp_img)->pixel_array[x][y].green = smooth((*bmp_img)->pixel_array[x][y].green + brightness);
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

// source: https://www.leunen.com/cbuilder/rotbmp.html
BMP rotateImage(BMP *bmp_img, double angle) {
    BMP rotated;
    rotated.info_header = bmp_img->info_header;
    rotated.header = bmp_img->header;

    double radians = (2 * 3.1416 * angle) / 360;
    float cosine = (float) cos(radians);
    float sine = (float) sin(radians);

    float p1x = (-bmp_img->info_header.height_px * sine);
    float p1y = (bmp_img->info_header.height_px * cosine);
    float p2x = ((bmp_img->info_header.width_px * cosine) - (bmp_img->info_header.height_px * sine));
    float p2y = ((bmp_img->info_header.height_px * cosine) - (bmp_img->info_header.width_px * sine));
    float p3x = (bmp_img->info_header.width_px * cosine);
    float p3y = (bmp_img->info_header.width_px * sine);

    float minx = min(0, min(p1x, min(p2x, p3x)));
    float miny = min(0, min(p1y, min(p2y, p3y)));
    float maxx = max(p1x, max(p2x, p3x));
    float maxy = max(p1y, max(p2y, p3y));

    int rotated_width = (int) ceil(fabs(maxx) - minx);
    int rotated_height = (int) ceil(fabs(maxy) - miny);

    rotated.info_header.width_px = rotated_width;
    rotated.info_header.height_px = rotated_height;

    rotated.pixel_array = (Pixel **) malloc(sizeof(Pixel *) * rotated.info_header.width_px);
    for (int i = 0; i < rotated.info_header.width_px; i++) {
        rotated.pixel_array[i] = (Pixel *) malloc(sizeof(Pixel) * rotated.info_header.height_px);
    }
    if (rotated.pixel_array == NULL) {
        fprintf(stderr, "Could not allocate memory: %s\n", strerror(errno));
    }
    for (int y = rotated_height - 1; y >= 0; y--) {
        for (int x = 0; x < rotated_width; x++) {
            int new_x = (int) ((x + minx) * cosine + (y + miny) * sine);
            int new_y = (int) ((y + miny) * cosine - (x + minx) * sine);
            if (new_x >= 0 && new_x < rotated.info_header.width_px && new_y >= 0 &&
                new_y < rotated.info_header.height_px) {
                rotated.pixel_array[x][y] = bmp_img->pixel_array[new_x][new_y];
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
            rotated.pixel_array[x][y] = bmp_img->pixel_array[y][rotated.info_header.width_px - 1 - x];
        }
    }
    return rotated;
}
