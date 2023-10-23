#include <string.h>
#include "BMPHandler.h"

/**
 * Read BMP header of a BMP file.
 *
 * @param  file: A pointer to the file being read
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE* file, struct BMP_Header* header)
{
    // read bitmap file header ( 14 bytes )
    fread(&header->signature[0], sizeof(char) , 1 , file);
    fread(&header->signature[1], sizeof(char) , 1 , file);
    fread(&header->size, sizeof(unsigned int) , 1, file);
    fread(&header->reserved1, sizeof(short) , 1, file);
    fread(&header->reserved2, sizeof(short) , 1, file);
    fread(&header->offset_pixel_array, sizeof(unsigned int) , 1, file);
}

/**
 * Write BMP header of a file. Useful for creating a BMP file.
 *
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeBMPHeader(FILE* file, struct BMP_Header* header)
{
    fwrite((char*)header->signature, sizeof(char)*2 , 1 , file);
    fwrite((char*)&header->size, sizeof(unsigned int) , 1, file);
    fwrite((char*)&header->reserved1, sizeof(short) , 1, file);
    fwrite((char*)&header->reserved2, sizeof(short) , 1, file);
    fwrite((char*)&header->offset_pixel_array, sizeof(unsigned int) , 1, file);
}

/**
 * Read DIB header from a BMP file.
 *
 * @param  file: A pointer to the file being read
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header)
{
    fread(&header->headerSize, sizeof(unsigned int) , 1 , file);
    fread(&header->width, sizeof(int) , 1 , file);
    fread(&header->height, sizeof(int) , 1 , file);
    fread(&header->panesSize, sizeof(unsigned short) , 1 , file);
    fread(&header->bitsPerPixel, sizeof(unsigned short) , 1 , file);
    fread(&header->compression, sizeof(unsigned int) , 1 , file);
    fread(&header->imageSize, sizeof(unsigned int) , 1 , file);
    fread(&header->horizontalResolution, sizeof(unsigned int) , 1 , file);
    fread(&header->verticalResolution, sizeof(unsigned int) , 1 , file);
    fread(&header->colorPalette, sizeof(unsigned int) , 1 , file);
    fread(&header->importantColors, sizeof(unsigned int) , 1 , file);
}

/**
 * Write DIB header of a file. Useful for creating a BMP file.
 *
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeDIBHeader(FILE* file, struct DIB_Header* header)
{
    fwrite((char*)&header->headerSize, sizeof(unsigned int) , 1 , file);
    fwrite((char*)&header->width, sizeof(int) , 1 , file);
    fwrite((char*)&header->height, sizeof(int) , 1 , file);
    fwrite((char*)&header->panesSize, sizeof(unsigned short) , 1 , file);
    fwrite((char*)&header->bitsPerPixel, sizeof(unsigned short) , 1 , file);
    fwrite((char*)&header->compression, sizeof(unsigned int) , 1 , file);
    fwrite((char*)&header->imageSize, sizeof(unsigned int) , 1 , file);
    fwrite((char*)&header->horizontalResolution, sizeof(unsigned int) , 1 , file);
    fwrite((char*)&header->verticalResolution, sizeof(unsigned int) , 1 , file);
    fwrite((char*)&header->colorPalette, sizeof(unsigned int) , 1 , file);
    fwrite((char*)&header->importantColors, sizeof(unsigned int) , 1 , file);
}

/**
 * Make BMP header based on width and height. Useful for creating a BMP file.
 *
 * @param  header: Pointer to the destination DIB header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makeBMPHeader(struct BMP_Header* header, int width, int height)
{
    strcpy(header->signature, "BM");
    header->size = 14 + 40 + ((width*3) * height);
    header->reserved1 = 0;
    header->reserved2 = 0;
    header->offset_pixel_array = 14 + 40;
}
//struct DIB_Header{
//    //TODO: Finish struct
//    unsigned int headerSize;
//    int width;
//    int height;
//    unsigned short panesSize;
//    unsigned short bitsPerPixel;
//    unsigned int compression;
//    unsigned int imageSize;
//    unsigned int horizontalResolution;
//    unsigned int verticalResolution;
//    unsigned int colorPalette;
//    unsigned int importantColors;
//};
/**
* Make new DIB header based on width and height.Useful for creating a BMP file.
*
* @param  header: Pointer to the destination DIB header
* @param  width: Width of the image that this header is for
* @param  height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header* header, int width, int height)
{
    header->headerSize = 40;
    header->width = width;
    header->height = height;
    header->panesSize = 1;
    header->bitsPerPixel = 24;
    header->compression = 0;
    header->imageSize = (width*3) * height;
    header->horizontalResolution = 3780;
    header->verticalResolution = 3780;
    header->colorPalette = 0;
    header->importantColors = 0;
}

/**
 * Read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read
 * @param  pArr: Pixel array to store the pixels being read
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height)
{
    int i, j, k;
    int paddedRow = (width*3 + 3) & (~3);
    unsigned char grid[paddedRow], temp;
    for(i = 0; i < height; i++)
    {
        fread(grid, sizeof(unsigned char), paddedRow, file);
        for(j = 0, k=0; j < width*3; j += 3, k++)
        {
            // format (B, G, R)
            pArr[i][k].blue = (short)grid[j];
            pArr[i][k].green = (short)grid[j+1];
            pArr[i][k].red = (short)grid[j+2];
        }
    }
}

/**
 * Write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image to write to the file
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {

    unsigned char padding[3] = {0, 0, 0}, pixel[3];
    int paddingSize = (4 - (width * 3) % 4) % 4;

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            // format (B, G, R)
            pixel[0] = (unsigned char)pArr[i][j].blue;
            pixel[1] = (unsigned char)pArr[i][j].green;
            pixel[2] = (unsigned char)pArr[i][j].red;

            fwrite(pixel, sizeof(unsigned char)*3, 1, file);
        }
        fwrite(padding, sizeof(unsigned char), paddingSize, file);
    }
}
