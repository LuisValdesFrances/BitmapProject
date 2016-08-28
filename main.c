#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

int main(int argc, char *argv[]) {

	//Create bitmap
	int width = 100;
	int height = 100;
	Bitmap bitmap = buildBitmap(width,height);
	
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

	int y = 0;
	int x = 0;
	while(y < height){
		
		while(x < width){
			if((y%2 == 0 && x%2 == 0) || (y%2 != 0 && x%2 != 0)){
				setPixel(bitmap, x, y, black);
			}else{
				setPixel(bitmap, x, y, white);
			}
			x++;
		}
		x = 0;
		y++;
	}
	//Save bitmap
	char path[] = "/home/luis/Escritorio/img_3.bmp";

	saveBitmap(bitmap, path);

	destroyBitmap(bitmap);

	return EXIT_SUCCESS;
}