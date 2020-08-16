#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check if there are 2 arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
    }

    BYTE data[512];

    // open memory card
    FILE *f = fopen(argv[1], "r");
    // should not run if there is no "forensic image"
    if (f == NULL)
    {
        return 1;
    }
    int first_jpeg = 0;

    // jpeg file count to count file to be numbered
    int jpeg_count = 0;
    // jpegs to be output
    FILE *jpegs;
    // repeat until end of card:
    while (fread(data, 512, 1, f) != 0)
    {
        // read 512b into a buffer

        // if start of new jpeg:
        if ((data[0] == 0xff && data[1] == 0xd8 && data[2] == 0xff) && (data[3] & 0xf0) == 0xe0)
        {
            // if first jpeg
            if (first_jpeg)
            {
                fclose(jpegs);
            }
            // else not a first jpeg
            else
            {
                first_jpeg = 1;
            }
            char filename[8];
            sprintf(filename, "%03i.jpg", jpeg_count++);
            // jpeg_count++;
            jpegs = fopen(filename, "w");

            // no data in jpeg
            if (jpegs == NULL)
            {
                return 1;
            }
            fwrite(data, 512, 1, jpegs);
        }
        // else if already found jpeg
        else if (first_jpeg)
        {
            // keep on writing
            fwrite(data, 512, 1, jpegs);
        }
    }
    // close any remeaning files
    fclose(jpegs);
    fclose(f);
    return 0;
}
