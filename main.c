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
	insertPixel(bitmap, createPixel(0, 0, 0,),     0, 0);
	insertPixel(bitmap, createPixel(255, 255, 255),1, 0);
	insertPixel(bitmap, createPixel(0, 0, 0,),     2, 0);
	insertPixel(bitmap, createPixel(255, 255, 255),3, 0);
	
	insertPixel(bitmap, createPixel(255, 255, 255),0, 1);
	insertPixel(bitmap, createPixel(0, 0, 0),      1, 1);
	insertPixel(bitmap, createPixel(255, 255, 255),2, 1);
	insertPixel(bitmap, createPixel(0, 0, 0,),     3, 1);

	//Save bitmap
	char path[] = "/home/luis/Escritorio/lesson_6/bitmap_project";
	char name[] = "bitmap_1";

	writeBitmap(bitmap, path, name);

	destroyBitmap(&bitmap);
}