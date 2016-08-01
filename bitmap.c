#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitmap.h"


struct _pixel{
	int red;
	int green;
	int blue;
};

struct _header{
	int values[SIZE_VALUE_HEADER];
};

struct _bitmap{
	Header BMP[BMP_HEADER_ROWS];
	Header DIB[DIB_HEADER_ROWS];
	Pixel pixels[MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT];
	int imageWidth;
	int imageHeight;
};

Bitmap buildBitmap(int width, int height){
	Bitmap bitmap = malloc(sizeof(struct _bitmap));
	assert(bitmap != NULL);
	bitmap->imageWidth = width;
	bitmap->imageHeight = height;
	//bitmap->pixels[imageWidth*imageHeight];
	return bitmap;
}

Pixel createPixel(int redValue, int greenValue, int blueValue){
	Pixel p = {.red = redValue, .green = greenValue, .blue = blueValue};
	return p;
}

void insertPixel(Bitmap bitmap, int row, int column, int redValue, int blueValue, int greenValue){

}

void writeBitmap(Bitmap bitmap, char *path, char *name){

}

void destroyBitmap(Bitmap *bitmap){

}