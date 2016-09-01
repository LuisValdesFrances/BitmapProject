#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "bitmap_test.h"
#include "bitmap.h"

static void testBuildBitmap();
static void testSetPixel();
static void testSaveBitmap();
static void testDestroyBitmap();

 void test() {
 	testBuildBitmap();
	testSetPixel();
	testSaveBitmap();
	testDestroyBitmap();
	printf("All passed!\n");
}

static void testBuildBitmap(){
	printf("testBuildBitmap\n");
	Bitmap bitmap = buildBitmap(100,100);
	assert(bitmap != NULL);
	destroyBitmap(&bitmap);
}

static void testSetPixel(){
	printf("testSetPixel\n");
	int width = 100;
	int height = 100;
	Bitmap bitmap = buildBitmap(width, height);
	color *c;

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
	
	y = 0;
	x = 0;
	while(y < height){
		while(x < width){
			c = getPixel(bitmap, x, y);
			if((y%2 == 0 && x%2 == 0) || (y%2 != 0 && x%2 != 0)){
				assert(black.red == c->red && black.green == c->green && black.blue == c->blue);
			}else{
				assert(white.red == c->red && white.green == c->green && white.blue == c->blue);
			}
			x++;
		}
		x = 0;
		y++;
	}
	destroyBitmap(&bitmap);
}

static void testSaveBitmap(){
	printf("testSaveBitmap\n");
	char path[] = "/home/luis/Escritorio/bitmapTest.bmp";
	/*
	int width = 34;
	int height = 57;
	Bitmap bitmap = buildBitmap(width,height);
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
	saveBitmap(bitmap, path);
	destroyBitmap(&bitmap);
	*/
	Bitmap lBitmap = loadBitmap(path);

	destroyBitmap(&lBitmap);

}

static void testDestroyBitmap(){
	printf("testDestroyBitmap\n");
	Bitmap bitmap = buildBitmap(10,10);
	//&bitmap se comporta como un puntero un puntero
	destroyBitmap(&bitmap);
	assert(bitmap == NULL);
}