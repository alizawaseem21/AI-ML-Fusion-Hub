#include "Image.h"

/* Creates a new image and returns it.
*
 * @param  pArr: Pixel array of this image.
 * @param  width: Width of this image.
 * @param  height: Height of this image.
 * @return A pointer to a new image.
*/
Image* image_create(struct Pixel** pArr, int width, int height)
{
    Image* image = (Image*) malloc (sizeof (Image));

    image->width = width;
    image->height = height;
    image->pArr = pArr;

    return image;
}


/* Destroys an image. Does not deallocate internal pixel array.
*
 * @param  img: the image to destroy.
*/
void image_destroy(Image** img)
{
    free(*img);
    (*img) = NULL;
}

/* Returns a double pointer to the pixel array.
*
 * @param  img: the image.
*/
struct Pixel** image_get_pixels(Image* img)
{
    return img->pArr;
}

/* Returns the width of the image.
*
 * @param  img: the image.
*/
int image_get_width(Image* img)
{
    return img->width;
}

/* Returns the height of the image.
*
 * @param  img: the image.
*/
int image_get_height(Image* img)
{
    return img->height;
}

/* Converts the image to grayscale.
*
 * @param  img: the image.
*/
void image_apply_bw(Image* img)
{
    int i, j;
    float grey;

    for(i=0; i<img->width; i++)
    {
        for(j=0; j<img->height; j++)
        {
            grey = (0.299 * (float)img->pArr[i][j].red) + (0.587 * (float)img->pArr[i][j].green) + (0.114 * (float)(img->pArr[i][j].blue));
            if(grey > 255)
                grey = 255;
            img->pArr[i][j].blue = (short)grey;
            img->pArr[i][j].red = (short)grey;
            img->pArr[i][j].green = (short)grey;
        }
    }
}

/**
 * Shift color of the internal Pixel array. The dimension of the array is width * height.
 * The shift value of RGB is rShift, gShift，bShift. Useful for color shift.
 *
 * @param  img: the image.
 * @param  rShift: the shift value of color r shift
 * @param  gShift: the shift value of color g shift
 * @param  bShift: the shift value of color b shift
 */
void image_apply_colorshift(Image* img, int rShift, int gShift, int bShift)
{
    int i, j;

    for(i=0; i<img->width; i++)
    {
        for(j=0; j<img->height; j++)
        {
            img->pArr[i][j].blue += bShift;
            if( img->pArr[i][j].blue < 0 ) {
                img->pArr[i][j].blue = 0;
            } else if (img->pArr[i][j].blue > 255) {
                img->pArr[i][j].blue = 255;
            }

            img->pArr[i][j].green += gShift;
            if( img->pArr[i][j].green < 0 ) {
                img->pArr[i][j].green = 0;
            } else if (img->pArr[i][j].green > 255) {
                img->pArr[i][j].green = 255;
            }

            img->pArr[i][j].red += rShift;
            if( img->pArr[i][j].red < 0 ) {
                img->pArr[i][j].red = 0;
            } else if (img->pArr[i][j].red > 255) {
                img->pArr[i][j].red = 255;
            }
        }
    }
}

/* Converts the image to grayscale. If the scaling factor is less than 1 the new image will be
 * smaller, if it is larger than 1, the new image will be larger.
 *
 * @param  img: the image.
 * @param  factor: the scaling factor
*/
void image_apply_resize(Image* img, float factor)
{
    int x, y, x1, y1;
    int newWidth = img->width * factor;
    int newHeight = img->height * factor;

//    if factor = 0.5
//    [0.5  0]
//    [0   0.5]

//    inverse mapping
//    [1/0.5  0]
//    [0   1/0.5]

    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * newHeight);
    for (int p = 0; p < newHeight; p++) {
        pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * newWidth);
    }

    for(x = 0; x < newHeight; x++)
    {
        for( y = 0; y < newWidth; y++ )
        {
            x1 = x * 1/factor;
            y1 = y * 1/factor;

            pixels[x][y] = img->pArr[x1][y1];
        }

    }

    for(x=0; x<img->height; x++)
        free(img->pArr[x]);
    free(img->pArr);

    img->pArr = pixels;
    img->width = newWidth;
    img->height = newHeight;
}