#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

int main(int argc, char *argv[]) {

	//Create bitmap
	Bitmap bitmap = buildBitmap(4,2);
	
	//Insert pixels
	/*
	0,1,0,1
	1,0,1,1
	*/
	color black;
	color white;
	black.red = 0;
	black.green = 0;
	black.blue = 0;
	white.red = 255;
	white.green = 255;
	white.blue = 255;

	setPixel(bitmap, 0, 0, black);
	setPixel(bitmap, 1, 0, white);
	setPixel(bitmap, 2, 0, black);
	setPixel(bitmap, 3, 0, white);
	
	setPixel(bitmap, 0, 1, white);
	setPixel(bitmap, 1, 1, black);
	setPixel(bitmap, 2, 1, white);
	setPixel(bitmap, 3, 1, black);

	

	//Save bitmap
	char path[] = "/home/luis/Escritorio/lesson_6/bitmap_project/img_1";

	saveBitmap(bitmap, path);

	destroyBitmap(&bitmap);
	
	

	return EXIT_SUCCESS;
}