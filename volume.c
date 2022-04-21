// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]); // atof converts a string into a float number

    // TODO: Copy header from input file to output file
    uint8_t header[HEADER_SIZE];
    fread(header, sizeof(uint8_t), HEADER_SIZE, input);
    fwrite(header, sizeof(uint8_t), HEADER_SIZE, output);
    printf("ftell: %ld", ftell(output));

    int16_t sample;
    fseek(input, 0, SEEK_END);
    long n_byte = ftell(input);
    fseek(input, 44, SEEK_SET);

    /*Here I intended to find the EOF in a different way. Instead of using the EOF macro, I decided to use a
    ftell at the very last byte of the file so I can know the file's size and use it to know its end.
    
    In the for loop I'm using the HEADER_SIZE const to start the loop at byte 44 (after the header of input)
    and I'm incrementing i by 2, because the int16_t type uses 2 byte of memory and fread and fwrite moves the
    file pointer 2 bytes to the end. So, to compare i with the file size (n_byte) I need to increment it by 2*/

    for (long i = HEADER_SIZE; i < n_byte; i += 2)
    {
        // When I'm finnished copying the header, I get the sample from the input file and update it
        fread(&sample, sizeof(int16_t), 1, input);
        sample = sample * factor;
        fwrite(&sample, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
