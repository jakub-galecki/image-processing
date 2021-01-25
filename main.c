#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bmp_img.h"
#include "conversions.h"

int main(int argc, char **argv) {
    FILE *image_input = fopen("sample.bmp", "rb");
    // For now works only with 24 bit.
    if (image_input == NULL) {
        printf("No such file.");
        abort();
    }
    BMP *bmp = read_bmp(image_input);
    BMP rotated = rotateImageRight(bmp);
    fclose(image_input);
    FILE *tmp = fopen(" output.bmp", "wb");
    write_image(&rotated, tmp);
    fclose(tmp);
    return 0;
}
