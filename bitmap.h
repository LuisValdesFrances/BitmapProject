#ifndef __BITMAP_H__
#define __BITMAP_H__


/*Abstract data structure*/ 
typedef struct _bitmap *Bitmap;


typedef struct _color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} color;


//Public methodes interface
Bitmap buildBitmap(int width, int height);

Bitmap loadBitmap(char *filename);

void setPixel(Bitmap bitmap, int x, int y, color color);

color *getPixel(Bitmap bitmap, int x, int y);

void saveBitmap(Bitmap bitmap, char *filename);

void destroyBitmap(Bitmap *pBitmap);

#endif






