#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitmap.h"

/* implementation */
struct _bitmap{
	int width;
	int height;
	color *pixels;
};


struct _header {
	unsigned char signature[2];
	unsigned char size[4];
	unsigned char unused1[2];
	unsigned char unused2[2];
	unsigned char pixelArrayOffset[4];
};

void printHeader(struct _header header);
void printResult(struct _header header, struct _bitmap bitmap, int size);


Bitmap buildBitmap(int width, int height){
	Bitmap bitmap = malloc(sizeof(struct _bitmap));
	assert(bitmap != NULL);

	bitmap->width = width;
	bitmap->height = height;
	int size = width*height;
	color *pixels = malloc(sizeof(struct _color)*size);
	assert(pixels != NULL);

	bitmap->pixels = pixels;

	return bitmap;
}

void setPixel(Bitmap bitmap, int x, int y, color color){

}

static void setSignature(struct _header *header){
	header->signature[0] = 'B';
	header->signature[1] = 'M';
}

static void setSize(struct _header *header, int size){
	/*
	Compara los cuatro bits de la derecha (Cada 'f' ocupa dos bits(valor 15 en decimal)) con 1111. 
	Devuelve 1 si encuentra 1 o 0 en caso contrario
	*/
	header->size[0] = size & 0xff;
	header->size[1] = (size>>8) & 0xff;
	header->size[2] = (size>>16) & 0xff;
	header->size[3] = (size>>24) & 0xff;
}

void setUnused(struct _header *header){

}

void setPixelArrayOffset(struct _header *header, int pixelArrayOffset){
	
}

static struct _header createHeader(int size, int pixelArrayOffset) {

	struct _header header;
	setSignature(&header);
	setSize(&header, size);
	setUnused(&header);
	setPixelArrayOffset(&header, pixelArrayOffset);

	return header;
}


void saveBitmap(Bitmap bitmap, char *filename){


	int pixelArraySize = 254;//bitmap->width * bitmap->height;
	int dibSize = 1;
	int headerSize = 1;

	int fileSize = headerSize + dibSize + pixelArraySize;
	int pixelArrayOffset = headerSize + dibSize;

	struct _header header = createHeader(fileSize, pixelArrayOffset);
	/*
	struct _dib dib = createDib(bitmap);
	char *pixelArray = createPixelArray(bitmap);
	//*/


	printResult(header, *bitmap, 0);

	//write();

	//*/
}

void printResult(struct _header header, struct _bitmap bitmap, int size){
	printHeader(header);
	//printDib();
	//printBitmap(Bitmap bitmap, size);
}

void printHeader(struct _header header){
	printf("\n-Header signature-");
	printf("\n%c", header.signature[0]);
	printf("\n%c", header.signature[1]);
	printf("\n-Header size-");
	printf("\n%d", (int)header.size[0]);
	printf("\n%d", (int)header.size[1]);
	printf("\n%d", (int)header.size[2]);
	printf("\n%d", (int)header.size[3]);

}

/*
void printBitmap(_bitmap bitmap, int size){
	printf("\n-Pixel array-\n");
	int i = 0;
	while(i < size){
		printf("[");
		char r = (bitmap.pixels + i)->red;
		char g = (bitmap.pixels + i)->green;
		char b = (bitmap.pixels + i)->blue;
		printf("%c", r);
		printf("%c", g);
		printf("%c", b);
		printf("]");
	}
}
*/

void destroyBitmap(Bitmap *bitmap){

}