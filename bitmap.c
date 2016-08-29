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

struct _header{
	unsigned char signature[2];
	unsigned char fileSize[4];
	unsigned char unused1[2];
	unsigned char unused2[2];
	unsigned char pixelArrayOffset[4];
};

struct _dib{
	unsigned char dibSize[4];
	unsigned char bitmapWidth[4];
	unsigned char bitmapHeight[4];
	unsigned char numberColors[2];
	unsigned char NumberBitsPerPixel[2];
	unsigned char biRGB[4];
	unsigned char bitmapSize[4];
	unsigned char printResolutionHorizontal[4];
	unsigned char printResolutionVertical[4];
	unsigned char colorsPalette[4];
	unsigned char importantColors[4];
};

static struct _header createHeader(int size, int pixelArrayOffset);
static void setSignature(struct _header *header);
static void setFileSize(struct _header *header, int fileSize);
static void setUnused(struct _header *header);
static void setPixelArrayOffset(struct _header *header, int pixelArrayOffset);

static struct _dib createDib(int size, Bitmap bitmap);
static void setDibSize(struct _dib *dib, int size);
static void setBitmapDimensions(struct _dib *dib, Bitmap bitmap);
static void setBitmapSize(struct _dib *dib, Bitmap bitmap);
static void setDibConstants(struct _dib *dib);

static int getImageSize(Bitmap bitmap);
static int getPaddingSize(Bitmap bitmap);
static int getPaddingFile(int width);

static void setBitmapSize(struct _dib *dib, Bitmap bitmap);
static void setBitmapDimensions(struct _dib *dib, Bitmap bitmap);

static void write(Bitmap bitmap, char *filename, struct _header header, struct _dib dib, int pixelArraySize);
static void setValuesInPixelArray(Bitmap bitmap, unsigned char *pixelArray, int size);
static void saveInFile(char *filename, struct _header header, struct _dib dib, unsigned char *pixelArray, int pixelArraySize);

static void reverse(unsigned char *pixelArray, int size);
static void reverseFile(unsigned char *pixelArray, int fileSize, int totalSize);
static void addPadding(Bitmap bitmap, unsigned char *pixelArray, int size, unsigned char *pixelArrayPadding, int sizePadding);

static void printHeader(struct _header header);
static void printDib(struct _dib dib);
static void printResult(struct _header header, struct _dib dib, unsigned char *pixelArrayPadding, int pixelArraySize);
static void printPixelArray(unsigned char *pixelArrayPadding, int size);







Bitmap buildBitmap(int width, int height){
	/*
	La idea es alojar una serie de datos, los que componen un bitmap en la memoria heap. Así que el 
	objetivo de este metodo es crear espacio en la memoria heap, el suficiente para que pueda almacenar
	un bitmap del tamapño especificado por los parametros.

	Asi pues, buildBitmap hace lo siguiente:
		-Reserva espacio en la memoria heap para almacenar un Bitmap
		-Reserva otro espacio en la memoria heap`para almacenar una lista de colores
		-Accede al la lista de colores del bitmap (puntero) y la cambia el valor para que apunte al espacio donde se
		 almacenan los colores.
		-Devuelve un puntero que apunte a dicho espacia en la memoria heap
	*/

	/*
	Partiendo de esto, creo un puntero que apunta a ese lugar de la heap. Es importante matizar que el puntero
	se aloja en la memoria frame, puesto que lo unico que estoy haciendo es reservar espacio en la memoria heap
	*/
	Bitmap bitmap = malloc(sizeof(struct _bitmap));
	assert(bitmap != NULL);

	//Almaceno en el espacio para el bitmap, en la heap, el ancho y alto de la imagen.
	bitmap->width = width;
	bitmap->height = height;
	int size = width*height;
	
	/*
	En otro lugar de la memoria heap creo el tamaño suficiente como para alojar una lista de pixeles
	*/
	color *p = malloc(sizeof(struct _color)*size);
	assert(p != NULL);
	/*
	Vinculo ambas direcciones, es decir, accedo al valor del la lista de pixeles del bitmap y le cambio el
	valor para que apunte al espacio de la heap, que acabo de crear, que es el que usare para almacenar 
	la lista de pixeles
	*/
	bitmap->pixels = p;

	return bitmap;
}

void setPixel(Bitmap bitmap, int x, int y, color c){
	//Formula: x+(width*y)
	int index = x + (bitmap->width * y);
	bitmap->pixels[index] = c;
	printf("\nInsertado color en la posicion: %d", index);
}

void saveBitmap(Bitmap bitmap, char *filename){

	//Tanaño en numero de bytes
	int headerSize = 14;
	int dibSize = 40;
	int pixelArraySize = getImageSize(bitmap) + getPaddingSize(bitmap); 
	
	int fileSize = headerSize + dibSize + pixelArraySize;
	int pixelArrayOffset = headerSize + dibSize;

	struct _header header = createHeader(fileSize, pixelArrayOffset);
	struct _dib dib = createDib(dibSize, bitmap);

	write(bitmap, filename, header, dib, pixelArraySize);
}

void destroyBitmap(Bitmap bitmap){
	free(bitmap->pixels);
	bitmap->pixels = NULL;
	free(bitmap);
	bitmap = NULL;
}

static struct _header createHeader(int fileSize, int pixelArrayOffset) {

	struct _header header;
	setSignature(&header);
	setFileSize(&header, fileSize);
	setUnused(&header);
	setPixelArrayOffset(&header, pixelArrayOffset);
	return header;
};

static struct _dib createDib(int size, Bitmap bitmap) {

	struct _dib dib;
	setDibSize(&dib, size);
	setBitmapDimensions(&dib, bitmap);
	setBitmapSize(&dib, bitmap);
	setDibConstants(&dib);

	return dib;
};

static void write(Bitmap bitmap, char *filename, struct _header header, struct _dib dib, int pixelArraySize){
	
	unsigned char pixelArrayPadding[pixelArraySize];
	
	setValuesInPixelArray(bitmap, pixelArrayPadding, pixelArraySize);
	
	printResult(header, dib, pixelArrayPadding, pixelArraySize);

	saveInFile(filename, header, dib, pixelArrayPadding, pixelArraySize);
}


/*
Header fuctions
*/
static void setSignature(struct _header *header){
	header->signature[0] = 'B';
	header->signature[1] = 'M';
}
static void setFileSize(struct _header *header, int fileSize){
	/*
	Compara los cuatro bits de la derecha (Cada 'f' ocupa dos bits(valor 15 en decimal)) con 1111. 
	Devuelve 1 si encuentra 1 o 0 en caso contrario
	*/
	header->fileSize[0] = fileSize & 0xff;
	header->fileSize[1] = (fileSize>>8) & 0xff;
	header->fileSize[2] = (fileSize>>16) & 0xff;
	header->fileSize[3] = (fileSize>>24) & 0xff;
}
static void setUnused(struct _header *header){
	header->unused1[0] = 0;
	header->unused1[1] = 0;
	header->unused2[0] = 0;
	header->unused2[1] = 0;
}
static void setPixelArrayOffset(struct _header *header, int pixelArrayOffset){
	header->pixelArrayOffset[0] = pixelArrayOffset & 0xff;
	header->pixelArrayOffset[1] = (pixelArrayOffset>>8) & 0xff;
	header->pixelArrayOffset[2] = (pixelArrayOffset>>16) & 0xff;
	header->pixelArrayOffset[3] = (pixelArrayOffset>>24) & 0xff;
}

/*
DIB fuctions
*/
static void setDibSize(struct _dib *dib, int size){
	dib->dibSize[0] = size & 0xff;
	dib->dibSize[1] = (size>>8) & 0xff;
	dib->dibSize[2] = (size>>16) & 0xff;
	dib->dibSize[3] = (size>>24) & 0xff;
}
static void setBitmapDimensions(struct _dib *dib, Bitmap bitmap){
	dib->bitmapWidth[0] = bitmap->width & 0xff;
	dib->bitmapWidth[1] = (bitmap->width>>8) & 0xff;
	dib->bitmapWidth[2] = (bitmap->width>>16) & 0xff;
	dib->bitmapWidth[3] = (bitmap->width>>24) & 0xff;
	dib->bitmapHeight[0] = bitmap->height & 0xff;;
	dib->bitmapHeight[1] = (bitmap->height>>8) & 0xff;
	dib->bitmapHeight[2] = (bitmap->height>>16) & 0xff;
	dib->bitmapHeight[3] = (bitmap->height>>24) & 0xff;
}
static void setBitmapSize(struct _dib *dib, Bitmap bitmap){
	int totalSize = getImageSize(bitmap) + getPaddingSize(bitmap);
	dib->bitmapSize[0] = totalSize & 0xff;
	dib->bitmapSize[1] = (totalSize>>8) & 0xff;;
	dib->bitmapSize[2] = (totalSize>>16) & 0xff;;
	dib->bitmapSize[3] = (totalSize>>24) & 0xff;;
}
static void setDibConstants(struct _dib *dib){
	dib->numberColors[0] = 0;
	dib->numberColors[1] = 1;
	dib->NumberBitsPerPixel[0] = 24;
	dib->NumberBitsPerPixel[1] = 0;
	dib->biRGB[0] = 0;
	dib->biRGB[1] = 0;
	dib->biRGB[2] = 0;
	dib->biRGB[3] = 0;
	
	dib->printResolutionHorizontal[0] = 2835 & 0xff;
	dib->printResolutionHorizontal[1] = (2835>>8) & 0xff;
	dib->printResolutionHorizontal[2] = (2835>>16) & 0xff;
	dib->printResolutionHorizontal[3] = (2835>>24) & 0xff;
	dib->printResolutionVertical[0] = 2835 & 0xff;
	dib->printResolutionVertical[1] = (2835>>8) & 0xff;
	dib->printResolutionVertical[2] = (2835>>16) & 0xff;
	dib->printResolutionVertical[3] = (2835>>24) & 0xff;
	dib->colorsPalette[0] = 0;
	dib->colorsPalette[1] = 0;
	dib->colorsPalette[2] = 0;
	dib->colorsPalette[3] = 0;
	dib->importantColors[0] = 0;
	dib->importantColors[1] = 0;
	dib->importantColors[2] = 0;
	dib->importantColors[3] = 0;
}

/*
write pixel array functions
*/
static void setValuesInPixelArray(Bitmap bitmap, unsigned char *pixelArrayPadding, int size){
	
	unsigned char pArray[getImageSize(bitmap) + getPaddingSize(bitmap)];

	
	int i = 0;
	int arrayIndex = 0;
	while(i < bitmap->width*bitmap->height){

		pArray[arrayIndex++] = (bitmap->pixels + i)->blue;
		pArray[arrayIndex++] = (bitmap->pixels + i)->green;
		pArray[arrayIndex++] = (bitmap->pixels + i)->red;  
		i++;
	}

	//Atencion, en el archivo .bitmap este array se recorre de izquierda a derecha y de abajo a arriba
	reverse(pArray, getImageSize(bitmap));
	
	int fileSize = bitmap->width*3;
	reverseFile(pArray, fileSize, getImageSize(bitmap));
	
	addPadding(bitmap, pArray, getImageSize(bitmap), pixelArrayPadding, getPaddingFile(bitmap->width));
}

static void addPadding(Bitmap bitmap, unsigned char *pixelArray, int size, unsigned char *pixelArrayPadding, int sizePadding){

	int i = 0;
	int paddingCount = 0;
	int totalCount = 0;

	
	while(i < size){
		
		pixelArrayPadding[totalCount] = pixelArray[i]; 
		totalCount++;
		i++;
		//Add pading
		if(i % (bitmap->width*3) == 0){
			
			while(paddingCount < sizePadding){
				pixelArrayPadding[totalCount] = 0;
				paddingCount++;
				totalCount++;
			}
			paddingCount = 0;
			
		}
	}
}

static void reverse(unsigned char *pixelArray, int size){
	int i = 0;
	unsigned char aux;

	while(i < size/2){
		aux = pixelArray[i];
		pixelArray[i] = pixelArray[size - (i+1)];
		pixelArray[size - (i+1)] = aux;
		i++;
	}
}

static void reverseFile(unsigned char *pixelArray, int fileSize, int totalSize){

	unsigned char aux[fileSize];
	int i = 0;
	int j = 0;
	int fileControl = 0;
	while(i < totalSize){
		aux[fileControl] = pixelArray[i];
		i++;
		fileControl++;
		//Reserse any file
		if(i%fileSize == 0){
			reverse(aux, fileSize);
			while(j < fileSize){
				pixelArray[i - (fileSize-j)] = aux[j];
				j++;
			}
			j = 0;
			fileControl = 0;
		}
	}
}

static void saveInFile(char *filename, struct _header header, struct _dib dib, unsigned char *pixelArray, int pixelArraySize){
	FILE *fp;
 	
 	fp = fopen (filename, "a+t" ); //parametro para escritura al final y para file tipo texto
 	
 	fwrite(&header, sizeof(struct _header), 1, fp);
 	fwrite(&dib, sizeof(struct _dib), 1, fp);
 	
 	//Pixel array
 	int i = 0;
 	while(i < pixelArraySize){
		fputc(pixelArray[i], fp);
		i++;
	}
	fclose (fp);
}

static void printResult(struct _header header, struct _dib dib, unsigned char *pixelArrayPadding, int pixelArraySize){
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

static int getImageSize(Bitmap bitmap){
	return (bitmap->width*bitmap->height) * 3;
}

static int getPaddingSize(Bitmap bitmap){
	return (getPaddingFile(bitmap->width) * bitmap->height);
}

static int getPaddingFile(int width){
	  
	int rest = width%4;
	return rest;
}


