#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "bitmap_test.h"

int main(int argc, char *argv[]) {

	test();


	//Create bitmap
	int width = 100;
	int height = 100;
	Bitmap bitmap = buildBitmap(width,height);
	
	//Insert pixels
	color black = {0, 0, 0};
	color white = {255, 255, 255};

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
	char path[] = "/home/luis/Escritorio/myBitmap.bmp";

	saveBitmap(bitmap, path);

	//Le paso direccion de memoria donde se encuentra el bitmap
	destroyBitmap(&bitmap);

	return EXIT_SUCCESS;
}