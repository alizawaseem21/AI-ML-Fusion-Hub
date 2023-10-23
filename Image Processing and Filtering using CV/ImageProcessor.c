#include <unistd.h>
#include <string.h>
#include "Image.h"
#include "BMPHandler.h"

int main(int argc, char *argv[])
{
    if(argc < 2) {
        fprintf(stderr, "Input file not specified.\n");
        exit(1);
    }
    int opt;
    char *input_file = argv[1];
    char *output_file = NULL;
    float scale_factor = 0;
    int rShift = 0, bShift = 0, gShift = 0, bw = -1;

    opterr = 0;

    while ((opt = getopt(argc, argv, "o:s:r:w^g:b")) != -1) {
        switch (opt) {
            case 'o':
                output_file = optarg;
                break;
            case 's':
                scale_factor = atof(optarg);
                break;
            case 'w':
                bw = 1;
                break;
            case 'r':
                rShift = atoi(optarg);
                break;
            case 'g':
                gShift = atoi(optarg);
                break;
            case 'b':
                bShift = atoi(optarg);
                break;
        }
    }

    //START - HEADER FUNCTIONALITY MINIMAL EXAMPLE (DO NOT SUBMIT)
    struct BMP_Header BMP;
    struct DIB_Header DIB;

    FILE* file_input = fopen(input_file, "rb");

    if(file_input == NULL) {
        fprintf(stderr, "Input file does not exist.\n");
        exit(1);
    }

    char new_out[100];
    if(output_file == NULL) {
        for(int i=0, j=0; i<strlen(input_file); i++, j++)
        {
            if(input_file[i] == '.') {
                strcat(new_out, "_copy");
                j += 5;
            }
            new_out[j] = input_file[i];
            new_out[j+1] = 0;
        }
        output_file = new_out;
    }

    readBMPHeader(file_input, &BMP);
    readDIBHeader(file_input, &DIB);

    struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * DIB.height);
    for (int p = 0; p < 152; p++) {
        pixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * DIB.width);
    }

    readPixelsBMP(file_input, pixels, DIB.width, DIB.height);
    fclose(file_input);

    Image* img = image_create(pixels, DIB.width, DIB.height);

    if(bw == 1)
        image_apply_bw(img);

    if(rShift != 0  || bShift != 0 || gShift != 0)
    	image_apply_colorshift(img, rShift, gShift, bShift);

    if(scale_factor != 0) {
        image_apply_resize(img, scale_factor);
        DIB.width = DIB.width * scale_factor;
        DIB.height = DIB.height * scale_factor;
    }

    FILE* file_output = fopen(output_file, "wb");
    writeBMPHeader(file_output, &BMP);
    writeDIBHeader(file_output, &DIB);
    writePixelsBMP(file_output, image_get_pixels(img), image_get_width(img), image_get_width(img));
    image_destroy(&img);
    fclose(file_output);

    return 0;
}