#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "bitmap_test.h"
#include "bitmap.h"

static void testBuildBitmap();
static void testSetPixel();
static void testSaveBitmap();
static void testLoadBitmap();
static void testDestroyBitmap();

 void test() {
 	testBuildBitmap();
	testSetPixel();
	testSaveBitmap();
	testLoadBitmap();
	testDestroyBitmap();
	printf("All passed!\n");
}

static void testBuildBitmap(){
	printf("testBuildBitmap\n");
	Bitmap bitmap = buildBitmap(1000,1000);
	assert(bitmap != NULL);
	destroyBitmap(&bitmap);
}

static void testSetPixel(){
	printf("testSetPixel\n");
	int width = 1000;
	int height = 1000;
	Bitmap bitmap = buildBitmap(width, height);
	color *c;

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
	
	char path[] = "/home/luis/Escritorio/bitmapTest1.bmp";
	int width = 100;
	int height = 100;
	Bitmap bitmap = buildBitmap(width,height);
	saveBitmap(bitmap, path);
	destroyBitmap(&bitmap);

	FILE *fp = NULL;
	fp = fopen (path, "r");
 	assert(fp != NULL);
 	fclose(fp);
 }

static void testLoadBitmap(){
	printf("testLoadBitmap\n");
	char path[] = "/home/luis/Escritorio/bitmapTest2.bmp";
	int width = 1024;
	int height = 1024;
	Bitmap bitmap = buildBitmap(width,height);
	
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
	saveBitmap(bitmap, path);

	Bitmap lBitmap = loadBitmap(path);

	y = 0;
	x = 0;
	color *c;
	color *c2;
	while(y < height){
		while(x < width){
			c = getPixel(bitmap, x, y);
			c2 = getPixel(lBitmap, x, y);
			assert(c->red == c2->red && c->green == c2->green && c->blue == c2->blue);
			x++;
		}
		x = 0;
		y++;
	}
	destroyBitmap(&bitmap);
	destroyBitmap(&lBitmap);
}

static void testDestroyBitmap(){
	printf("testDestroyBitmap\n");
	Bitmap bitmap = buildBitmap(10,10);
	//&bitmap se comporta como un puntero un puntero
	destroyBitmap(&bitmap);
	assert(bitmap == NULL);
}