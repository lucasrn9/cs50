#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");

    // Create a buffer for a block of data
    uint8_t buffer[512];

    int JPEGCounter = -1;
    FILE *recoveredImg = NULL;
    // While there's still data left to read from the memory card
    while (fread(buffer, 1, 512, card) == 512)
    {
        // verify if a new jpeg file was found
        bool foundNewJPG = (buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0);

        if (foundNewJPG)
        {
            JPEGCounter++;
            char imgName[8];
            sprintf(imgName, "%03i.jpg", JPEGCounter);
            if (JPEGCounter > 0)
            {
                fclose(recoveredImg);
            }
            recoveredImg = fopen(imgName, "w");
        }
        // Write to JPEG from the data
        if (JPEGCounter > -1)
        {
            fwrite(buffer, 512, 1, recoveredImg);
        }
    }
    fclose(recoveredImg);
    fclose(card);
}
