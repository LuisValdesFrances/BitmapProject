#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

int main(int argc, char *argv[]) {

	//Create bitmap
	Bitmap bitmap = buildBitmap(4,4);
	
	//Insert pixels
	/*
	0,1,0,1
	1,0,1,1
	*/
	insertPixel(bitmap, 0, 0, 0, 0, 0);
	insertPixel(bitmap, 1, 0, 0, 0, 0);
	insertPixel(bitmap, 2, 0, 0, 0, 0);
	insertPixel(bitmap, 3, 0, 0, 0, 0);
	
	insertPixel(bitmap, 0, 1, 0, 0, 0);
	insertPixel(bitmap, 1, 1, 0, 0, 0);
	insertPixel(bitmap, 2, 1, 0, 0, 0);
	insertPixel(bitmap, 3, 1, 0, 0, 0);

	//Save bitmap
	char path[] = "/home/luis/Escritorio/lesson_6/bitmap_project";
	char name[] = "bitmap_1";

	writeBitmap(bitmap, path, name);

	destroyBitmap(&bitmap);

	return EXIT_SUCCESS;
}