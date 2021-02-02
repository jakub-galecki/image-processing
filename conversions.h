//
// Created by batman on 1/14/21.
//

#include "bmp_img.h"

#ifndef IMAGE_PROCESSING_CONVERSIONS_H
#define IMAGE_PROCESSING_CONVERSIONS_H

void grayscale(BMP **img);

void changeBrightness(BMP **bmp_img, int brightness);

BMP rotateImage(BMP *bmp_img, double angle);
BMP rotateImageRight(BMP *bmp_img);

#endif //IMAGE_PROCESSING_CONVERSIONS_H