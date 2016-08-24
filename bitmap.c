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
void printResult(struct _header header, struct _bitmap bitmap);
int getPading(int width);
void printBitmap(struct _bitmap bitmap);


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

void setPixel(Bitmap bitmap, int x, int y, color c){
	//Formula: x+(width*y)
	int index = x + (bitmap->width * y);
	bitmap->pixels[index] = c;
	printf("\nInsertado color en la posicion: %d", index);
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
	header->unused1[0] = 0;
	header->unused1[1] = 0;
	header->unused2[0] = 0;
	header->unused2[1] = 0;
}

void setPixelArrayOffset(struct _header *header, int pixelArrayOffset){
	header->pixelArrayOffset[0] = pixelArrayOffset & 0xff;
	header->pixelArrayOffset[1] = (pixelArrayOffset>>8) & 0xff;
	header->pixelArrayOffset[2] = (pixelArrayOffset>>16) & 0xff;
	header->pixelArrayOffset[3] = (pixelArrayOffset>>24) & 0xff;
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


	//Tanaño en numero de bytes
	int headerSize = 14;
	int dibSize = 40;
	int pixelArraySize = (bitmap->width * bitmap->height)*3;
	

	int fileSize = headerSize + dibSize + pixelArraySize;
	int pixelArrayOffset = headerSize + dibSize;

	struct _header header = createHeader(fileSize, pixelArrayOffset);
	/*
	struct _dib dib = createDib(bitmap);
	char *pixelArray = createPixelArray(bitmap);
	//*/


	printResult(header, *bitmap);

	//write();

	//*/
}

void printResult(struct _header header, struct _bitmap bitmap){
	printHeader(header);
	//printDib();
	printBitmap(bitmap);
}

void printHeader(struct _header header){
	printf("\n-Header signature-");
	printf("\n%c", header.signature[0]);
	printf(",%c", header.signature[1]);
	printf("\n-Header size-");
	printf("\n%d",(int)header.size[0]);
	printf(",%d", (int)header.size[1]);
	printf(",%d", (int)header.size[2]);
	printf(",%d", (int)header.size[3]);
	printf("\n-Unused-");
	printf("\n%d", (int)header.unused1[0]);
	printf(",%d", (int)header.unused1[1]);
	printf(",%d", (int)header.unused2[0]);
	printf(",%d", (int)header.unused2[1]);
	printf("\n-Pixel array offset-");
	printf("\n%d", (int)header.pixelArrayOffset[0]);
	printf(",\n%d", (int)header.pixelArrayOffset[1]);
	printf(",\n%d", (int)header.pixelArrayOffset[2]);
	printf(",\n%d", (int)header.pixelArrayOffset[3]);

}

int getPading(int width){
	  
	int rest = width%4;
	if(rest == 0){
		printf("\nEl ancho de la imagen es multiplo de 4");
	}else{
		printf("\nEl resto de la imagen es: %d", rest);
	}
	return 0;
}


void printBitmap(struct _bitmap bitmap){
	printf("\n-Pixel array-\n");
	int i = 0;
	while(i < bitmap.width*bitmap.height){
		printf("[");
		int r = (int)(bitmap.pixels + i)->red;
		int g = (int)(bitmap.pixels + i)->green;
		int b = (int)(bitmap.pixels + i)->blue;
		printf("%d", r);
		printf(",%d", g);
		printf(",%d", b);
		printf("]");
		i++;
		if(i % bitmap.width == 0){
			printf("\n");
		}
	}
}


void destroyBitmap(Bitmap *bitmap){

}