#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitmap.h"
#include "bitmap_print.h"

static void printHeader(struct _header header);
static void printDib(struct _dib dib);
static void printPixelArray(unsigned char *pixelArrayPadding, int size);

void printResult(struct _header header, struct _dib dib, unsigned char *pixelArrayPadding, int pixelArraySize){
	printHeader(header);
	printDib(dib);
	printPixelArray(pixelArrayPadding, pixelArraySize);
}

static void printHeader(struct _header header){
	printf("\n\n***HEADER***");
	printf("\n-Header signature-");
	printf("\n%c", header.signature[0]);
	printf(",%c", header.signature[1]);
	printf("\n-Header file size-");
	printf("\n%d",(int)header.fileSize[0]);
	printf(",%d", (int)header.fileSize[1]);
	printf(",%d", (int)header.fileSize[2]);
	printf(",%d", (int)header.fileSize[3]);
	printf("\n-Unused-");
	printf("\n%d", (int)header.unused1[0]);
	printf(",%d", (int)header.unused1[1]);
	printf(",%d", (int)header.unused2[0]);
	printf(",%d", (int)header.unused2[1]);
	printf("\n-Pixel array offset-");
	printf("\n%d", (int)header.pixelArrayOffset[0]);
	printf(",%d", (int)header.pixelArrayOffset[1]);
	printf(",%d", (int)header.pixelArrayOffset[2]);
	printf(",%d", (int)header.pixelArrayOffset[3]);
}

static void printDib(struct _dib dib){
	printf("\n\n***DIB***");
	printf("\n-Dib size-");
	printf("\n%d",(int)dib.dibSize[0]);
	printf(",%d", (int)dib.dibSize[1]);
	printf(",%d", (int)dib.dibSize[2]);
	printf(",%d", (int)dib.dibSize[3]);
	printf("\n-Bitmap width-");
	printf("\n%d",(int)dib.bitmapWidth[0]);
	printf(",%d", (int)dib.bitmapWidth[1]);
	printf(",%d", (int)dib.bitmapWidth[2]);
	printf(",%d", (int)dib.bitmapWidth[3]);
	printf("\n-Bitmap height-");
	printf("\n%d",(int)dib.bitmapHeight[0]);
	printf(",%d", (int)dib.bitmapHeight[1]);
	printf(",%d", (int)dib.bitmapHeight[2]);
	printf(",%d", (int)dib.bitmapHeight[3]);
	printf("\n-Number colors-");
	printf("\n%d",(int)dib.numberColors[0]);
	printf(",%d", (int)dib.numberColors[1]);
	printf("\n-Number bits x pixel-");
	printf("\n%d",(int)dib.NumberBitsPerPixel[0]);
	printf(",%d", (int)dib.NumberBitsPerPixel[1]);
	printf("\n-BI_RGB-");
	printf("\n%d",(int)dib.biRGB[0]);
	printf(",%d", (int)dib.biRGB[1]);
	printf(",%d", (int)dib.biRGB[2]);
	printf(",%d", (int)dib.biRGB[3]);
	printf("\n-Bitmap size-");
	printf("\n%d",(int)dib.bitmapSize[0]);
	printf(",%d", (int)dib.bitmapSize[1]);
	printf(",%d", (int)dib.bitmapSize[2]);
	printf(",%d", (int)dib.bitmapSize[3]);
	printf("\n-Print resolution horizontal-");
	printf("\n%d",(int)dib.printResolutionHorizontal[0]);
	printf(",%d", (int)dib.printResolutionHorizontal[1]);
	printf(",%d", (int)dib.printResolutionHorizontal[2]);
	printf(",%d", (int)dib.printResolutionHorizontal[3]);
	printf("\n-Print resolution vertical-");
	printf("\n%d",(int)dib.printResolutionVertical[0]);
	printf(",%d", (int)dib.printResolutionVertical[1]);
	printf(",%d", (int)dib.printResolutionVertical[2]);
	printf(",%d", (int)dib.printResolutionVertical[3]);
	printf("\n-Number of colors palette-");
	printf("\n%d",(int)dib.colorsPalette[0]);
	printf(",%d", (int)dib.colorsPalette[1]);
	printf(",%d", (int)dib.colorsPalette[2]);
	printf(",%d", (int)dib.colorsPalette[3]);
	printf("\n-Importants colors-");
	printf("\n%d",(int)dib.importantColors[0]);
	printf(",%d", (int)dib.importantColors[1]);
	printf(",%d", (int)dib.importantColors[2]);
	printf(",%d", (int)dib.importantColors[3]);
}

static void printPixelArray(unsigned char *pixelArrayPadding, int size){
	printf("\n\n***PIXEL ARRAY***\n");
	int i = 0;
	while(i < size){
		printf("%d\n",(int)pixelArrayPadding[i]);
		i++;
	}
}