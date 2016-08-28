#ifndef __BITMAP_H__
#define __BITMAP_H__


#define MAX_IMAGE_WIDTH 1024
#define MAX_IMAGE_HEIGHT 1024

/*Abstract data structure*/ 
typedef struct _bitmap *Bitmap;


typedef struct _color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} color;


//Public methodes interface
Bitmap buildBitmap(int width, int height);

void setPixel(Bitmap bitmap, int x, int y, color color);

void saveBitmap(Bitmap bitmap, char *filename);

void destroyBitmap(Bitmap bitmap);

#endif






