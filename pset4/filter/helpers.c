#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg
                = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE backup[height][width];

    // first making backup as this makes it lot of easier
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            backup[i][j] = image[i][j];
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, wid = width; j < width; j++)
        {
            image[i][j] = backup[i][wid - 1];
            wid--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE backup[height][width];

    // first making backup as this makes it lot of easier
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            backup[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0, total = 0; j < width; j++)
        {
            float blue_add = 0;
            float green_add = 0;
            float red_add = 0;
            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    if ((k < 0) || (l < 0) || (k > height - 1) || (l > width - 1))
                    {
                        continue;
                    }
                    else
                    {
                        blue_add += backup[k][l].rgbtBlue;
                        green_add += backup[k][l].rgbtGreen;
                        red_add += backup[k][l].rgbtRed;
                        total++;
                    }
                }
            }
            image[i][j].rgbtBlue = round(blue_add / total);
            image[i][j].rgbtGreen = round(green_add / total);
            image[i][j].rgbtRed = round(red_add / total);
            total = 0;

            // image[i][j].rgbtBlue = round (( backup[i-1][j-1].rgbtBlue +
            // backup[i-1][j].rgbtBlue +backup[i-1][j+1].rgbtBlue
            //                 +backup[i][j-1].rgbtBlue + backup[i][j].rgbtBlue
            //                 +backup[i][j+1].rgbtBlue
            //                 +backup[i+1][j-1].rgbtBlue + backup[i+1][j].rgbtBlue
            //                 +backup[i+1][j+1].rgbtBlue ) /9);

            // image[i][j].rgbtGreen = round (( backup[i-1][j-1].rgbtGreen +
            // backup[i-1][j].rgbtGreen +backup[i-1][j+1].rgbtGreen
            //                 +backup[i][j-1].rgbtGreen + backup[i][j].rgbtGreen
            //                 +backup[i][j+1].rgbtGreen
            //                 +backup[i+1][j-1].rgbtGreen + backup[i+1][j].rgbtGreen
            //                 +backup[i+1][j+1].rgbtGreen ) /9);

            // image[i][j].rgbtRed = round (( backup[i-1][j-1].rgbtRed +
            // backup[i-1][j].rgbtRed +backup[i-1][j+1].rgbtRed
            //                 +backup[i][j-1].rgbtRed + backup[i][j].rgbtRed
            //                 +backup[i][j+1].rgbtRed
            //                 +backup[i+1][j-1].rgbtRed + backup[i+1][j].rgbtRed
            //                 +backup[i+1][j+1].rgbtRed ) /9);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE backup[height][width];

    // first making backup as this makes it lot of easier
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, wid = width; j < width; j++)
        {
            backup[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0, total = 0; j < width; j++)
        {
            int gxR = 0;
            int gyR = 0;
            int gxG = 0;
            int gyG = 0;
            int gxB = 0;
            int gyB = 0;
            int cases = 1;
            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    if ((k < 0) || (l < 0) || (k > height - 1) || (l > width - 1))
                    {
                        cases++;
                        continue;
                    }
                    else
                    {
                        switch (cases)
                        {
                            case 1:
                                gxR += (-1 * backup[k][l].rgbtRed);
                                gyR += (-1 * backup[k][l].rgbtRed);
                                gxG += (-1 * backup[k][l].rgbtGreen);
                                gyG += (-1 * backup[k][l].rgbtGreen);
                                gxB += (-1 * backup[k][l].rgbtBlue);
                                gyB += (-1 * backup[k][l].rgbtBlue);
                                cases++;
                                break;
                            case 2:

                                // gxR += (0 * backup[k][l].rgbtRed);
                                gyR += (-2 * backup[k][l].rgbtRed);
                                // gxG += (0 * backup[k][l].rgbtGreen);
                                gyG += (-2 * backup[k][l].rgbtGreen);
                                // gxB += (0 * backup[k][l].rgbtBlue);
                                gyB += (-2 * backup[k][l].rgbtBlue);
                                cases++;
                                break;

                            case 3:

                                gxR += (1 * backup[k][l].rgbtRed);
                                gyR += (-1 * backup[k][l].rgbtRed);
                                gxG += (1 * backup[k][l].rgbtGreen);
                                gyG += (-1 * backup[k][l].rgbtGreen);
                                gxB += (1 * backup[k][l].rgbtBlue);
                                gyB += (-1 * backup[k][l].rgbtBlue);
                                cases++;
                                break;

                            case 4:

                                gxR += (-2 * backup[k][l].rgbtRed);
                                // gyR += (0 * backup[k][l].rgbtRed);
                                gxG += (-2 * backup[k][l].rgbtGreen);
                                // gyG += (0 * backup[k][l].rgbtGreen);
                                gxB += (-2 * backup[k][l].rgbtBlue);
                                // gyB += (0 * backup[k][l].rgbtBlue);
                                cases++;
                                break;

                            case 5:

                                // gxR += (0 * backup[k][l].rgbtRed);
                                // gyR += (0 * backup[k][l].rgbtRed);
                                // gxG += (0 * backup[k][l].rgbtGreen);
                                // gyG += (0 * backup[k][l].rgbtGreen);
                                // gxB += (0 * backup[k][l].rgbtBlue);
                                // gyB += (0 * backup[k][l].rgbtBlue);
                                cases++;
                                break;

                            case 6:

                                gxR += (2 * backup[k][l].rgbtRed);
                                // gyR += (0 * backup[k][l].rgbtRed);
                                gxG += (2 * backup[k][l].rgbtGreen);
                                // gyG += (0 * backup[k][l].rgbtGreen);
                                gxB += (2 * backup[k][l].rgbtBlue);
                                // gyB += (0 * backup[k][l].rgbtBlue);
                                cases++;
                                break;

                            case 7:

                                gxR += (-1 * backup[k][l].rgbtRed);
                                gyR += (1 * backup[k][l].rgbtRed);
                                gxG += (-1 * backup[k][l].rgbtGreen);
                                gyG += (1 * backup[k][l].rgbtGreen);
                                gxB += (-1 * backup[k][l].rgbtBlue);
                                gyB += (1 * backup[k][l].rgbtBlue);
                                cases++;
                                break;

                            case 8:

                                // gxR += (0 * backup[k][l].rgbtRed);
                                gyR += (2 * backup[k][l].rgbtRed);
                                // gxG += (0 * backup[k][l].rgbtGreen);
                                gyG += (2 * backup[k][l].rgbtGreen);
                                // gxB += (0 * backup[k][l].rgbtBlue);
                                gyB += (2 * backup[k][l].rgbtBlue);
                                cases++;
                                break;

                            case 9:

                                gxR += (1 * backup[k][l].rgbtRed);
                                gyR += (1 * backup[k][l].rgbtRed);
                                gxG += (1 * backup[k][l].rgbtGreen);
                                gyG += (1 * backup[k][l].rgbtGreen);
                                gxB += (1 * backup[k][l].rgbtBlue);
                                gyB += (1 * backup[k][l].rgbtBlue);
                                cases++;
                                break;
                        }
                    }
                }
            }
            int newR = round(sqrt(pow(gxR, 2) + pow(gyR, 2)));
            newR = (newR > 255 ? 255 : newR);
            image[i][j].rgbtRed = newR;

            int newG = round(sqrt(pow(gxG, 2) + pow(gyG, 2)));
            newG = (newG > 255 ? 255 : newG);
            image[i][j].rgbtGreen = newG;

            int newB = round(sqrt(pow(gxB, 2) + pow(gyB, 2)));
            newB = (newB > 255 ? 255 : newB);
            image[i][j].rgbtBlue = newB;
        }
    }
    return;
}
