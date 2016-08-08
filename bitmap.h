#ifndef __BITMAP_H__
#define __BITMAP_H__

#define FALSE 0
#define TRUE  1

#define MAX_IMAGE_WIDTH 1024
#define MAX_IMAGE_HEIGHT 1024

/*Abstract data structure*/ 
/*typedef oculta el tipo dee dato (int, char, etc) No nos interesa*/
typedef struct _bitmap *Bitmap;//Puntero hacia _bitmap

/*Specific data structure*/
typedef struct _pixel Pixel;

//Public methodes interface
Bitmap buildBitmap(int imageWidth, int imageHeight);

void insertPixel(Bitmap bitmap, int row, int column, int redValue, int blueValue, int greenValue);

void writeBitmap(Bitmap bitmap, char *path, char *name);

void destroyBitmap(Bitmap *bitmap);

#endif	