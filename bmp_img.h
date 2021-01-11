//
// Created by batman on 1/6/21.
//

#ifndef IMAGE_PROCESSING_BMP_IMG_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct __attribute__((packed)) {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BMP_Header;

typedef struct __attribute__((packed)) {
    uint32_t dib_header_size;
    int32_t width_px;
    int32_t height_px;
    uint16_t num_planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size_bytes;
    int32_t x_resolution_ppm;
    int32_t y_resolution_ppm;
    uint32_t num_colors;
    uint32_t important_colors;
} BMP_INFOHeader;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Pixel;

typedef struct {
    BMP_Header header;
    BMP_INFOHeader info_header;
    Pixel **pixel_array;
} BMP;

BMP *read_bmp(FILE *file);

void print_pixel_array_by_color(BMP *bmp_image, int option);

void write_image(BMP *bmp_image, FILE *file);

#define IMAGE_PROCESSING_BMP_IMG_H
#endif //IMAGE_PROCESSING_BMP_IMG_H
