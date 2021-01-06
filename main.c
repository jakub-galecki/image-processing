#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(){
    FILE *image_input = fopen("sample.bmp","r");
    if(image_input == NULL){
        printf("No such file.");
        abort();
    }
    FILE *image_output = fopen("sample_converted.bmp", "w+");
    // Read header of the bmp image -  http://www.ue.eti.pg.gda.pl/fpgalab/zadania.spartan3/zad_vga_struktura_pliku_bmp.html
    // Header 14 bytes, InfoHeader 40 bytes
    unsigned int header[54];
    for (int i = 0; i < 54; i++){
        header[i] = fgetc(image_input);
    }



    return 0;
}