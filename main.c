#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bmp_img.h"

int main() {
    FILE *image_input = fopen("sample.bmp", "rb");
    if (image_input == NULL) {
        printf("No such file.");
        abort();
    }
    BMP *bmp = read_bmp(image_input);
    fclose(image_input);
    FILE *tmp = fopen(" output.bmp", "wb");
    write_image(bmp, tmp);
    fclose(tmp);
    return 0;
}