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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*new_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    int gx_coef[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int gy_coef[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            int gx_blue = 0,
                gx_green = 0,
                gx_red = 0,
                gy_blue = 0,
                gy_green = 0,
                gy_red = 0;

            // count near colors
            for (int av_r = -1; av_r <= 1; av_r++)
            {
                for (int av_c = -1; av_c <= 1; av_c++)
                {
                    int row_index = r + av_r,
                        col_index = c + av_c;

                    int pixel_blue = 0,
                        pixel_green = 0,
                        pixel_red = 0;

                    if (row_index > -1 && row_index < height && col_index > -1 && col_index < width)
                    {
                        pixel_blue = image[row_index][col_index].rgbtBlue;
                        pixel_green = image[row_index][col_index].rgbtGreen;
                        pixel_red = image[row_index][col_index].rgbtRed;
                    }

                    // count Gx, Gy
                    gx_blue += pixel_blue * gx_coef[av_r + 1][av_c + 1];
                    gx_green += pixel_green * gx_coef[av_r + 1][av_c + 1];
                    gx_red += pixel_red * gx_coef[av_r + 1][av_c + 1];

                    gy_blue += pixel_blue * gy_coef[av_r + 1][av_c + 1];
                    gy_green += pixel_green * gy_coef[av_r + 1][av_c + 1];
                    gy_red += pixel_red * gy_coef[av_r + 1][av_c + 1];
                }
            }

            double g_blue = sqrt(gx_blue * gx_blue + gy_blue * gy_blue);
            new_image[r][c].rgbtBlue = round(g_blue) > 255 ? 255 : round(g_blue);
            double g_green = sqrt(gx_green * gx_green + gy_green * gy_green);
            new_image[r][c].rgbtGreen = round(g_green) > 255 ? 255 : round(g_green);
            double g_red = sqrt(gx_red * gx_red + gy_red * gy_red);
            new_image[r][c].rgbtRed = round(g_red) > 255 ? 255 : round(g_red);
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
