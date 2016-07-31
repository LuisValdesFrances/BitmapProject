#ifndef __BITMAP_H__
#define __BITMAP_H__

#define FALSE 0
#define TRUE  1

#define BMP_HEADER_SIZE 14
#define DIB_HEADER_SIZE 40
#define OFFSET_PIXEL_ARRAY (SIZE_BMP_HEADER + SIZE_DIB_HEADER)
#define NUMBER_BITS_PIXEL 24

#define PIXEL_IMAGE_SIZE 3
#define PADDING_IMAGE_SIZE 2

/*
Duda, cada fila del archivo .bmp tiene un tamaño determinado. Esta informacion es inherente al
propio formato .bmp ¿Necesito almanezar esta informacion en algún lado definido en esta interface?

const unsigned int BIMAP_ROW_SIZE[] = {
	//BMP HEADER
	2, //"BM"
	4, //Size of BMP file
	2, //Application specefic
	2, //Application specefic
	4, //Offset pixel array start
	//DIB HEADER
	4, //Number of bits in DIB header
	...


}

*/

//Public methodes interface
typedef struct _pixel *Pixel;
/*
	int red;
	int green;
	int blue;
*/

Pixel createPixel(int red, int blue, int green);

/*Abstract data structure*/ 
typedef struct _bitmap *Bitmap;
/*
	int[] BMP_Header
	int[] DIB_Header
	int[] imagePixels

*/
Bitmap buildBitmap(int imageWidth, int imageHeight);
/*
	BMP_Header[SIZE_BMP_HEADER]
	BMP_Header[SIZE_DIB_HEADER]
	imagePixels[getSizeImage(imageWidth, imageHeight)]
*/

void insertPixel(Bitmap bitmap, Pixel pixel, int row, int column);

void writeBitmap(Bitmap bitmap, *char path, *char name);


//Internal methodes
/**
Get image size (including padding)
*/
int getBitmapSize(int bitmapWidth, int bitmapHeight)

/**
Get char array of hexadecimnal value. The array size is determite for rowSize
*/
*char getHexValue(int value, int rowSize)

/**
Devuelve un array de valores hexadecimales, con la informacion de color para un pixel
El parametro de entrada es un array de enteros, con la informacion del color de un pixel en formato decimal
[255,0,0] -> [FF,0,0]
*/

*char getGreenComponent(*int color);
*char getRedComponent(*int color);
*char getBlueComponent(*int color);
