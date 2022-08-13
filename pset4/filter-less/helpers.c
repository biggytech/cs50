#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            double avg_shade = ((double)(image[r][c].rgbtBlue + image[r][c].rgbtGreen + image[r][c].rgbtRed) / 3);
            int shade = round(avg_shade);
            RGBTRIPLE pixel;
            pixel.rgbtBlue = shade;
            pixel.rgbtGreen = shade;
            pixel.rgbtRed = shade;
            image[r][c] = pixel;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            int sepiaRed = round(.393 * image[r][c].rgbtRed + .769 * image[r][c].rgbtGreen + .189 * image[r][c].rgbtBlue);
            int sepiaGreen = round(.349 * image[r][c].rgbtRed + .686 * image[r][c].rgbtGreen + .168 * image[r][c].rgbtBlue);
            int sepiaBlue = round(.272 * image[r][c].rgbtRed + .534 * image[r][c].rgbtGreen + .131 * image[r][c].rgbtBlue);

            RGBTRIPLE pixel;
            pixel.rgbtBlue = sepiaBlue > 255 ? 255 : sepiaBlue;
            pixel.rgbtGreen = sepiaGreen > 255 ? 255 : sepiaGreen;
            pixel.rgbtRed = sepiaRed > 255 ? 255 : sepiaRed;
            image[r][c] = pixel;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int row_middle = width / 2;

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < row_middle; c++)
        {
            RGBTRIPLE temp_pixel = image[r][c];
            image[r][c] = image[r][width - 1 - c];
            image[r][width - 1 - c] = temp_pixel;
        }

    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*new_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            int blue_sum = 0,
                green_sum = 0,
                red_sum = 0,
                from_row = r - 1,
                to_row = r + 1,
                from_col = c - 1,
                to_col = c + 1,
                pixel_count = 0;

            // count near colors
            for (int av_r = from_row; av_r <= to_row; av_r++)
            {
                for (int av_c = from_col; av_c <= to_col; av_c++)
                {
                    if (av_r > -1 && av_r < height && av_c > -1 && av_c < width)
                    {
                        blue_sum += image[av_r][av_c].rgbtBlue;
                        green_sum += image[av_r][av_c].rgbtGreen;
                        red_sum += image[av_r][av_c].rgbtRed;
                        pixel_count++;
                    }
                }
            }

            RGBTRIPLE pixel;
            pixel.rgbtBlue = round((double)blue_sum / pixel_count);
            pixel.rgbtGreen = round((double)green_sum / pixel_count);
            pixel.rgbtRed = round((double)red_sum / pixel_count);
            new_image[r][c] = pixel;
        }

    }

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            image[r][c] = new_image[r][c];
        }
    }

    free(new_image);

    return;
}
