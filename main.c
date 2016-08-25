#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

int main(int argc, char *argv[]) {

	//Create bitmap
	Bitmap bitmap = buildBitmap(2,2);
	
	//Insert pixels
	/*
	0,1,0,1
	1,0,1,1
	*/
	//color black;
	color white;
	//black.red = 0;
	//black.green = 0;
	//black.blue = 0;
	white.red = 255;
	white.green = 255;
	white.blue = 255;

	/*
	setPixel(bitmap, 0, 0, black);
	setPixel(bitmap, 1, 0, white);
	setPixel(bitmap, 2, 0, black);
	setPixel(bitmap, 3, 0, white);
	
	setPixel(bitmap, 0, 1, white);
	setPixel(bitmap, 1, 1, black);
	setPixel(bitmap, 2, 1, white);
	setPixel(bitmap, 3, 1, black);

	*/
	color blue;
	color green;
	color red;
	
	blue.red = 0;
	blue.green = 0;
	blue.blue = 255;
	green.red = 0;
	green.green = 255;
	green.blue = 0;
	red.red = 255;
	red.green = 0;
	red.blue = 0;
	/*
	blue.red = 1;
	blue.green = 2;
	blue.blue = 3;
	green.red = 4;
	green.green = 5;
	green.blue = 6;
	red.red = 7;
	red.green = 8;
	red.blue = 9;
	white.red = 10;
	white.green = 11;
	white.blue = 12;
	*/
	setPixel(bitmap, 0, 0, blue);
	setPixel(bitmap, 1, 0, green);
	setPixel(bitmap, 0, 1, red);
	setPixel(bitmap, 1, 1, white);

	//Save bitmap
	char path[] = "/home/luis/Escritorio/lesson_6/bitmap_project/img_1";

	saveBitmap(bitmap, path);

	destroyBitmap(&bitmap);
	
	

	return EXIT_SUCCESS;
}