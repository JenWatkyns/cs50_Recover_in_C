#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for valid usage
    if (argc != 2)
    {
        printf("Usage: ./recover [forensic image...]\n");
        return 1;
    }

    // Open memory card
    else
    {
        char *rawFile = argv[1];
        FILE *f = fopen(rawFile, "r");

        // Check if file can be opened and print error
        if (f == NULL)
        {
            printf("File could not be opened\n");
            return 1;
        }

        // Initialise variables
        BYTE buffer[512];
        int counter = 0;
        FILE *img = NULL;
        char filename[8];

        // Read file
        while (fread(&buffer, 512, 1, f) == 1)
        {
            // Search for JPEG header [0xff, 0xd8, 0xff, 0xe1-f]
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
            {
                if (!(counter == 0))
                {
                    fclose(img);
                }

                // Make new JPEG / Generate filenames with ### - 2 decimal numbers starting at 000.jpg
                sprintf(filename, "%03i.jpg", counter);
                img = fopen(filename, "w");
                counter++;
            }

            // Writing files
            if (!(counter == 0))
            {
                fwrite(&buffer, 512, 1, img);
            }
        }

        // Close open files
        fclose(f);
        fclose(img);

        return 0;
    }
}
