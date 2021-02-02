# image-processing
Image Processing in C Language  
***  
## To compile:
```
 gcc `pkg-config --cflags gtk+-3.0` -o a main.c bmp_img.c conversions.c -lm `pkg-config --libs gtk+-3.0`  
 ./a
```
***
Please note that rotating image by angle is not working as well as it should. 
There are some problems that should be solved but for lack of time they will be fixed in the future. 
To fix:
- Rotate image by any angle
- Eventul Memory leaks
- Format code (functions should return either BMP or \*BMP)
