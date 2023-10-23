#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;
    char *input_file = NULL;
    char *output_file = NULL;
    double scale_factor = 1.0;
    int rotation_angle = 0;
    int do_write = 0;
    int do_show = 0;

    while ((opt = getopt(argc, argv, "i:o:s:r:ws")) != -1) {
        switch (opt) {
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 's':
                scale_factor = atof(optarg);
                break;
            case 'r':
                rotation_angle = atoi(optarg);
                break;
            case 'w':
                do_write = 1;
                break;
            case 's':
                do_show = 1;
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-i input_file] [-o output_file] [-s scale_factor] [-r rotation_angle] [-w] [-s]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Check if required arguments are present
    if (input_file == NULL) {
        fprintf(stderr, "Input file not specified.\n");
        exit(EXIT_FAILURE);
    }

    // Process the arguments
    printf("Input file: %s\n", input_file);
    printf("Output file: %s\n", output_file);
    printf("Scale factor: %f\n", scale_factor);
    printf("Rotation angle: %d\n", rotation_angle);
    printf("Write output file: %s\n", do_write ? "yes" : "no");
    printf("Show processed image: %s\n", do_show ? "yes" : "no");

    // Rest of the program logic goes here...

    return 0;
}
