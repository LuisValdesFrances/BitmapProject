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

static struct _dib createDib(int size, Bitmap bitmap);

static int getImageSize(Bitmap bitmap);
static int getPaddingSize(Bitmap bitmap);
static int getPaddingFile(int width);

static void write(Bitmap bitmap, char *filename, struct _header header, struct _dib dib, int pixelArraySize);
static void setValuesInPixelArray(Bitmap bitmap, unsigned char *pixelArray, int size);
static void saveInFile(char *filename, struct _header header, struct _dib dib, unsigned char *pixelArray, int pixelArraySize);

static void reverse(unsigned char *pixelArray, int size);
static void reverseFile(unsigned char *pixelArray, int fileSize, int totalSize);
static void addPadding(Bitmap bitmap, unsigned char *pixelArray, int size, unsigned char *pixelArrayPadding, int sizePadding);

static void readBitmap(char *filename, unsigned char *bitmapBuffer);
static int getBitmapWidth(unsigned char *bitmapBuffer);
static int getBitmapHeight(unsigned char *bitmapBuffer);
static int getPixelArrayOffset(unsigned char *bitmapBuffer);
static void extractPixelArray(Bitmap bitmap, unsigned char *bitmapBuffer, unsigned char *pixelArray);
static void deletePadding(Bitmap bitmap, unsigned char *pixelArrayPadding, unsigned char *pixelArray);
static void buildColorsArray(Bitmap bitmap, unsigned char *pixelArray, color *pixelColors);


static void printResult(struct _header header, struct _dib dib, unsigned char *pixelArrayPadding, int pixelArraySize);
static void printHeader(struct _header header);
static void printDib(struct _dib dib);
static void printPixelArray(unsigned char *pixelArrayPadding, int size);


static void setInt(unsigned char *start, int number);
static void setShort(unsigned char *start, int value);

Bitmap buildBitmap(int width, int height){
	if(width > MAX_IMAGE_WIDTH || height > MAX_IMAGE_HEIGHT){
		return NULL;
	}
	/*
	La idea es alojar una serie de datos, los que componen un bitmap en la memoria heap. Así que el 
	objetivo de este metodo es crear espacio en la memoria heap, el suficiente para que pueda almacenar
	un bitmap del tamapño especificado por los parametros.

	Asi pues, buildBitmap hace lo siguiente:
		-Reserva espacio en la memoria heap para almacenar un Bitmap
		-Reserva otro espacio en la memoria heap para almacenar una lista de colores
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

Bitmap loadBitmap(char *filename){
	
	int maxImageSize = (MAX_IMAGE_WIDTH * MAX_IMAGE_HEIGHT)*3;
	int maxPaddingSize = getPaddingFile(MAX_IMAGE_WIDTH) * MAX_IMAGE_HEIGHT;
	int headerSize = 14;
	int dibSize = 40;
	int maxBitmapSize = maxImageSize + maxPaddingSize + headerSize + dibSize;  

	//Como la imagen puede ser muy grande, decido alojarla en el heap
	
	unsigned char *buffer = malloc(sizeof(char)*maxBitmapSize);

	assert(buffer != NULL);

	readBitmap(filename, buffer);
	

 	int imgWidth= getBitmapWidth(buffer);
 	int imgHeight = getBitmapHeight(buffer);

 	Bitmap bitmap = buildBitmap(imgWidth, imgHeight);

 	
 	unsigned char pixelArray[getImageSize(bitmap)];
 	unsigned char pixelArrayPadding[getImageSize(bitmap) + getPaddingSize(bitmap)];
 	
 	int imageSize = bitmap->width*bitmap->height;
 	color *pixelColors = malloc(sizeof(color)*imageSize);

 	assert(pixelColors != NULL);

	
 	extractPixelArray(bitmap, buffer, pixelArrayPadding);
 	deletePadding(bitmap, pixelArrayPadding, pixelArray);
 	
 	
 	reverse(pixelArray, getImageSize(bitmap));
 	
 	int fileSize = (bitmap->width)*3;
 	
 	
 	reverseFile(pixelArray, fileSize, getImageSize(bitmap));
	
	buildColorsArray(bitmap, pixelArray, pixelColors);
	
	//Insert pixels
	int x = 0;
	int y = 0;
	int i = 0;
	while(y < bitmap->height){
		while(x < bitmap->width){
			setPixel(bitmap, x, y, pixelColors[i]);
			x++;
			i++;
		}
		x = 0;
		y++;
	}

	free(pixelColors);
	pixelColors = NULL;

	free(buffer);
 	buffer = NULL;

 	return bitmap;
}

static void buildColorsArray(Bitmap bitmap, unsigned char *pixelArray, color *pixelColors){
	int imageSize = bitmap->width*bitmap->height;
	int i = 0;
	int j = 0;
	while(i < imageSize){
		pixelColors[i].blue = pixelArray[j++];
		pixelColors[i].green = pixelArray[j++];
		pixelColors[i].red = pixelArray[j++];
		i++;
	}
}

static void extractPixelArray(Bitmap bitmap, unsigned char *bitmapBuffer, unsigned char *pixelArrayPadding){
	
	int i = 0;
	int offset = getPixelArrayOffset(bitmapBuffer);
	int imageSizePadding = getImageSize(bitmap) + getPaddingSize(bitmap);
	while(i < imageSizePadding){
		pixelArrayPadding[i] = bitmapBuffer[i + offset];
		i++;
	}
}

static void deletePadding(Bitmap bitmap, unsigned char *pixelArrayPadding, unsigned char *pixelArray){
	int i = 0;
	int p = 0;
	int fileSize = bitmap->width*3;
	int imageSize = getImageSize(bitmap);
	while(i < imageSize){
		pixelArray[i] = pixelArrayPadding[p];
		i++;
		if(i > 0 && i % fileSize == 0){
			p += getPaddingFile(bitmap->width);
		}
		p++;
	}
}

static void readBitmap(char *filename, unsigned char *bitmapBuffer){
	int totalLines = 0;
 	int data = 0;
 	FILE *fp;
 	
 	fp = fopen (filename, "r");
 	assert(fp != NULL);
 	
 	
 	while((data = fgetc(fp)) != EOF){
 		bitmapBuffer[totalLines] = data;
 		totalLines++;
 	}
 	fclose(fp);
}

static int getBitmapWidth(unsigned char *bitmapBuffer){
	int width = 0;
	width += bitmapBuffer[18];
	width += (bitmapBuffer[19]<<8);
	width += (bitmapBuffer[20]<<16);
	width += (bitmapBuffer[21]<<24);
	return width;
}

static int getBitmapHeight(unsigned char *bitmapBuffer){
	int height = 0;
	height += bitmapBuffer[22];
	height += (bitmapBuffer[23]<<8);
	height += (bitmapBuffer[24]<<16);
	height += (bitmapBuffer[25]<<24);
	return height;
}

static int getPixelArrayOffset(unsigned char *bitmapBuffer){
	int offset = 0;
	offset += bitmapBuffer[10];
	offset += (bitmapBuffer[11]<<8);
	offset += (bitmapBuffer[12]<<16);
	offset += (bitmapBuffer[13]<<24);
	return offset;
}

/*
La funcion se come como paramentro un puntero a un puntero de bitmap.
La funcion free solo resetea a 0 los valores guardados en el heap y comunica al sistema de que ese
espacio esta disponible de nuevo.
En cambio cuando se asigna a un puntero a NULL, se des-referencia hacia a donde apuntaba (Su valor no apunta a nada), 
y por este motivo es necesario actuar sobre el valor del puntero (que esta fuera de la función) y no sobre el puntero
que se duplica dentro de la función.
*/
void destroyBitmap(Bitmap *pBitmap){
	free((*pBitmap)->pixels);
	(*pBitmap)->pixels = NULL;
	free(*pBitmap);
	*pBitmap = NULL;
}

void setPixel(Bitmap bitmap, int x, int y, color c){
	//Formula: x+(width*y)
	assert(x < bitmap->width && y < bitmap->height);
	int index = x + (bitmap->width * y);
	bitmap->pixels[index] = c;
}

color *getPixel(Bitmap bitmap, int x, int y){
	assert(x < bitmap->width && y < bitmap->height);
	int index = x + (bitmap->width * y);
	color *c = &(bitmap->pixels[index]);
	return c;
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

static struct _header createHeader(int fileSize, int pixelArrayOffset) {

	struct _header header;
	
	header.signature[0] = 'B';
	header.signature[1] = 'M';

	setInt(header.fileSize, fileSize);
	setShort(header.unused1, 0);
	setShort(header.unused2, 0);
	setInt(header.pixelArrayOffset, pixelArrayOffset);
	return header;
};

static struct _dib createDib(int size, Bitmap bitmap) {

	struct _dib dib;
	setInt(dib.dibSize, size);	
	setInt(dib.bitmapWidth, bitmap->width);
	setInt(dib.bitmapHeight, bitmap->height);
	setInt(dib.bitmapSize, getImageSize(bitmap) + getPaddingSize(bitmap));
	//Rest constans
	dib.numberColors[0] = 0;
	dib.numberColors[1] = 1;
	dib.NumberBitsPerPixel[0] = 24;
	dib.NumberBitsPerPixel[1] = 0;
	setInt(dib.biRGB, 0);
	setInt(dib.printResolutionHorizontal, 2835);
	setInt(dib.printResolutionVertical, 2835);
	setInt(dib.colorsPalette, 0);
	setInt(dib.importantColors, 0);

	return dib;
};

static void write(Bitmap bitmap, char *filename, struct _header header, struct _dib dib, int pixelArraySize){
	
	unsigned char pixelArrayPadding[pixelArraySize];
	
	setValuesInPixelArray(bitmap, pixelArrayPadding, pixelArraySize);
	
	printResult(header, dib, pixelArrayPadding, pixelArraySize);

	saveInFile(filename, header, dib, pixelArrayPadding, pixelArraySize);
}

/*
	Compara los cuatro bits de la derecha (Cada 'f' ocupa dos bits(valor 15 en decimal)) con 1111. 
	Devuelve 1 si encuentra 1 o 0 en caso contrario
*/
static void setInt(unsigned char *start, int value){
	start[0] = value & 0xff; // *(start + 0)
	start[1] = (value>>8) & 0xff;
	start[2] = (value>>16) & 0xff;
	start[3] = (value>>24) & 0xff;	
}

static void setShort(unsigned char *start, int value){
	start[0] = value & 0xff; // *(start + 0)
	start[1] = (value>>8) & 0xff;
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
		//Reserse each file
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
 	
 	fp = fopen(filename, "w"); //parametro para escritura al final y para file tipo texto
 	
 	fwrite(&header, sizeof(struct _header), 1, fp);
 	fwrite(&dib, sizeof(struct _dib), 1, fp);
 	
 	//Pixel array
 	int i = 0;
 	while(i < pixelArraySize){
		fputc(pixelArray[i], fp);
		i++;
	}
	fclose(fp);
}


static void printResult(struct _header header, struct _dib dib, unsigned char *pixelArrayPadding, int pixelArraySize){
	printHeader(header);
	printDib(dib);
	printPixelArray(pixelArrayPadding, pixelArraySize);
}

static void printHeader(struct _header header){
	/*
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
	*/
}

static void printDib(struct _dib dib){
	/*
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
	*/
}

static void printPixelArray(unsigned char *pixelArrayPadding, int size){
	/*
	printf("\n\n***PIXEL ARRAY***\n");
	int i = 0;
	while(i < size){
		printf("%d\n",(int)pixelArrayPadding[i]);
		i++;
	}
	*/
}

static int getImageSize(Bitmap bitmap){
	return (bitmap->width*bitmap->height) * 3;
}

static int getPaddingSize(Bitmap bitmap){
	return getPaddingFile(bitmap->width) * bitmap->height;
}

static int getPaddingFile(int width){
	int rest = width%4;
	return rest;
}


