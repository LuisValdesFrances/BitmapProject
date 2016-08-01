#ifndef __BITMAP_H__
#define __BITMAP_H__

#define FALSE 0
#define TRUE  1

#define MAX_IMAGE_WIDTH 1024
#define MAX_IMAGE_HEIGHT 1024

#define BMP_HEADER_ROWS 5
#define DIB_HEADER_ROWS 11
#define SIZE_VALUE_HEADER 4 //Can be 4 or 2
#define BMP_HEADER_SIZE 14
#define DIB_HEADER_SIZE 40
#define OFFSET_PIXEL_ARRAY (SIZE_BMP_HEADER + SIZE_DIB_HEADER)
#define NUMBER_BITS_PIXEL 24

#define PIXEL_IMAGE_SIZE 3

/*Abstract data structure*/ 
typedef struct _bitmap *Bitmap;//Puntero hacia _bitmap

/*Specific data structure*/
typedef struct _pixel Pixel;
typedef struct _header Header;

//Public methodes interface
Bitmap buildBitmap(int imageWidth, int imageHeight);

Pixel createPixel(int redValue, int blueValue, int greenValue);

void insertPixel(Bitmap bitmap, int row, int column, int redValue, int blueValue, int greenValue);

void writeBitmap(Bitmap bitmap, char *path, char *name);

void destroyBitmap(Bitmap *bitmap);

#endif	