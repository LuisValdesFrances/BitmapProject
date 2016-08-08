#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitmap.h"


struct _pixel{
	char red;
	char green;
	char blue;
};

struct _bitmap{
	Pixel *pixels;
};

Bitmap buildBitmap(int width, int height){
	Bitmap bitmap = malloc(sizeof(struct _bitmap));
	assert(bitmap != NULL);

	//Necesito guardar esto fuera del frame???????
	//Pixel arrayPixel[width*height];
	//Pixel *pPixel = malloc(sizeof(arrayPixel));
	int imgSize = width*height;
	Pixel *pPixel = malloc(sizeof(Pixel)*imgSize);


	bitmap->pixels = pPixel;
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