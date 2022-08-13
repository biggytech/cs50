#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    char *card_file = argv[1];

    FILE *file = fopen(card_file, "r");

    if (file == NULL)
    {
        printf("Cannot open the file\n");
        return 1;
    }

    BYTE block[512];
    int images_counter = 0;
    char *image_name = malloc(7);
    FILE *image = NULL;

    while (fread(block, 1, 512, file) == 512)
    {
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && block[3] >= 0xe0 && block[3] <= 0xef)
        {
            if (image != NULL)
            {
                fclose(image);
            }

            sprintf(image_name, "%03d.jpg", images_counter);
            // create file
            image = fopen(image_name, "a+");
            images_counter++;
        }

        if (image != NULL)
        {
            fwrite(block, 1, 512, image);
        }
    }

    if (image != NULL)
    {
        fclose(image);
    }

    free(image_name);

    fclose(file);
}
