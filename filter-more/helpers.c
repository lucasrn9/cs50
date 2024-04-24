#include "helpers.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            RGBTRIPLE pixel = image[h][w];
            int average = round((pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3.0);
            pixel.rgbtBlue = average;
            pixel.rgbtGreen = average;
            pixel.rgbtRed = average;
            image[h][w] = pixel;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        int half = 0;
        bool isWidthOdd = (width % 2) != 0;
        if (isWidthOdd)
        {
            half = floor(width / 2);
        }
        else
        {
            half = (width / 2) - 1;
        }
        for (int j = 0; j <= half; j++)
        {
            RGBTRIPLE tmp;
            tmp = image[i][j];
            image[i][j] = image[i][(width - 1) - j];
            image[i][(width - 1) - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE bluredImg[height][width];
    // iterate through rows
    for (int i = 0; i < height; i++)
    {
        int hasRowsAbove = (i - 1) >= 0;
        int hasRowsBelow = (i + 1) <= height - 1;
        // iterate through pixels
        for (int j = 0; j < width; j++)
        {
            int hasPixelsToLeft = (j - 1) >= 0;
            int hasPixelsToRight = (j + 1) <= width - 1;
            RGBTRIPLE pixel = image[i][j];
            int totalPixels = 0, totalRed = 0, totalGreen = 0, totalBlue = 0;

            // pixel itself
            totalPixels++;
            totalRed += pixel.rgbtRed;
            totalGreen += pixel.rgbtGreen;
            totalBlue += pixel.rgbtBlue;
            // neighbor pixels

            // left pixel
            if (hasPixelsToLeft)
            {
                totalPixels++;
                totalRed += image[i][j - 1].rgbtRed;
                totalGreen += image[i][j - 1].rgbtGreen;
                totalBlue += image[i][j - 1].rgbtBlue;
            }
            // right pixel
            if (hasPixelsToRight)
            {
                totalPixels++;
                totalRed += image[i][j + 1].rgbtRed;
                totalGreen += image[i][j + 1].rgbtGreen;
                totalBlue += image[i][j + 1].rgbtBlue;
            }
            if (hasRowsAbove)
            {
                // imediately up pixel
                totalPixels++;
                totalRed += image[i - 1][j].rgbtRed;
                totalGreen += image[i - 1][j].rgbtGreen;
                totalBlue += image[i - 1][j].rgbtBlue;
                // up left pixel
                if (hasPixelsToLeft)
                {
                    totalPixels++;
                    totalRed += image[i - 1][j - 1].rgbtRed;
                    totalGreen += image[i - 1][j - 1].rgbtGreen;
                    totalBlue += image[i - 1][j - 1].rgbtBlue;
                }
                // up right pixel
                if (hasPixelsToRight)
                {
                    totalPixels++;
                    totalRed += image[i - 1][j + 1].rgbtRed;
                    totalGreen += image[i - 1][j + 1].rgbtGreen;
                    totalBlue += image[i - 1][j + 1].rgbtBlue;
                }
            }
            if (hasRowsBelow)
            {
                // imediately below pixel
                totalPixels++;
                totalRed += image[i + 1][j].rgbtRed;
                totalGreen += image[i + 1][j].rgbtGreen;
                totalBlue += image[i + 1][j].rgbtBlue;
                // below left pixel
                if (hasPixelsToLeft)
                {
                    totalPixels++;
                    totalRed += image[i + 1][j - 1].rgbtRed;
                    totalGreen += image[i + 1][j - 1].rgbtGreen;
                    totalBlue += image[i + 1][j - 1].rgbtBlue;
                }
                // below right pixel
                if (hasPixelsToRight)
                {
                    totalPixels++;
                    totalRed += image[i + 1][j + 1].rgbtRed;
                    totalGreen += image[i + 1][j + 1].rgbtGreen;
                    totalBlue += image[i + 1][j + 1].rgbtBlue;
                }
            }
            // calc average
            int avgRed = round((double) totalRed / totalPixels);
            int avgGreen = round((double) totalGreen / totalPixels);
            int avgBlue = round((double) totalBlue / totalPixels);
            bluredImg[i][j].rgbtRed = avgRed;
            bluredImg[i][j].rgbtGreen = avgGreen;
            bluredImg[i][j].rgbtBlue = avgBlue;
        }
    }
    // copy bluredImg to image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = bluredImg[h][w];
        }
    }
    return;
}

// Detect edges

void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imageWithFilter[height][width];
    // iterate through rows
    for (int i = 0; i < height; i++)
    {
        int hasRowsAbove = (i - 1) >= 0;
        int hasRowsBelow = (i + 1) <= height - 1;
        // iterate through pixels
        for (int j = 0; j < width; j++)
        {
            int hasPixelsToLeft = (j - 1) >= 0;
            int hasPixelsToRight = (j + 1) <= width - 1;
            RGBTRIPLE pixel = image[i][j];
            int gXTotalRed = 0, gXTotalGreen = 0, gXTotalBlue = 0;
            int gYTotalRed = 0, gYTotalGreen = 0, gYTotalBlue = 0;

            // pixel itself
            gXTotalRed += (pixel.rgbtRed * 0);
            gXTotalGreen += (pixel.rgbtGreen * 0);
            gXTotalBlue += (pixel.rgbtBlue * 0);

            gYTotalRed += (pixel.rgbtRed * 0);
            gYTotalGreen += (pixel.rgbtGreen * 0);
            gYTotalBlue += (pixel.rgbtBlue * 0);
            // neighbor pixels

            // left pixel
            if (hasPixelsToLeft)
            {
                gXTotalRed += (image[i][j - 1].rgbtRed * -2);
                gXTotalGreen += (image[i][j - 1].rgbtGreen * -2);
                gXTotalBlue += (image[i][j - 1].rgbtBlue * -2);

                gYTotalRed += (image[i][j - 1].rgbtRed * 0);
                gYTotalGreen += (image[i][j - 1].rgbtGreen * 0);
                gYTotalBlue += (image[i][j - 1].rgbtBlue * 0);
            }

            // right pixel
            if (hasPixelsToRight)
            {
                gXTotalRed += (image[i][j + 1].rgbtRed * 2);
                gXTotalGreen += (image[i][j + 1].rgbtGreen * 2);
                gXTotalBlue += (image[i][j + 1].rgbtBlue * 2);

                gYTotalRed += (image[i][j + 1].rgbtRed * 0);
                gYTotalGreen += (image[i][j + 1].rgbtGreen * 0);
                gYTotalBlue += (image[i][j + 1].rgbtBlue * 0);
            }

            if (hasRowsAbove)
            {
                // imediately up pixel
                gXTotalRed += (image[i - 1][j].rgbtRed * 0);
                gXTotalGreen += (image[i - 1][j].rgbtGreen * 0);
                gXTotalBlue += (image[i - 1][j].rgbtBlue * 0);

                gYTotalRed += (image[i - 1][j].rgbtRed * -2);
                gYTotalGreen += (image[i - 1][j].rgbtGreen * -2);
                gYTotalBlue += (image[i - 1][j].rgbtBlue * -2);
                // up left pixel
                if (hasPixelsToLeft)
                {
                    gXTotalRed += (image[i - 1][j - 1].rgbtRed * -1);
                    gXTotalGreen += (image[i - 1][j - 1].rgbtGreen * -1);
                    gXTotalBlue += (image[i - 1][j - 1].rgbtBlue * -1);

                    gYTotalRed += (image[i - 1][j - 1].rgbtRed * -1);
                    gYTotalGreen += (image[i - 1][j - 1].rgbtGreen * -1);
                    gYTotalBlue += (image[i - 1][j - 1].rgbtBlue * -1);
                }
                // up right pixel
                if (hasPixelsToRight)
                {
                    gXTotalRed += (image[i - 1][j + 1].rgbtRed * 1);
                    gXTotalGreen += (image[i - 1][j + 1].rgbtGreen * 1);
                    gXTotalBlue += (image[i - 1][j + 1].rgbtBlue * 1);

                    gYTotalRed += (image[i - 1][j + 1].rgbtRed * -1);
                    gYTotalGreen += (image[i - 1][j + 1].rgbtGreen * -1);
                    gYTotalBlue += (image[i - 1][j + 1].rgbtBlue * -1);
                }
            }

            if (hasRowsBelow)
            {
                // imediately below pixel
                gXTotalRed += (image[i + 1][j].rgbtRed * 0);
                gXTotalGreen += (image[i + 1][j].rgbtGreen * 0);
                gXTotalBlue += (image[i + 1][j].rgbtBlue * 0);

                gYTotalRed += (image[i + 1][j].rgbtRed * 2);
                gYTotalGreen += (image[i + 1][j].rgbtGreen * 2);
                gYTotalBlue += (image[i + 1][j].rgbtBlue * 2);
                // below left pixel
                if (hasPixelsToLeft)
                {
                    gXTotalRed += (image[i + 1][j - 1].rgbtRed * -1);
                    gXTotalGreen += (image[i + 1][j - 1].rgbtGreen * -1);
                    gXTotalBlue += (image[i + 1][j - 1].rgbtBlue * -1);

                    gYTotalRed += (image[i + 1][j - 1].rgbtRed * 1);
                    gYTotalGreen += (image[i + 1][j - 1].rgbtGreen * 1);
                    gYTotalBlue += (image[i + 1][j - 1].rgbtBlue * 1);
                }
                // below right pixel
                if (hasPixelsToRight)
                {
                    gXTotalRed += (image[i + 1][j + 1].rgbtRed * 1);
                    gXTotalGreen += (image[i + 1][j + 1].rgbtGreen * 1);
                    gXTotalBlue += (image[i + 1][j + 1].rgbtBlue * 1);

                    gYTotalRed += (image[i + 1][j + 1].rgbtRed * 1);
                    gYTotalGreen += (image[i + 1][j + 1].rgbtGreen * 1);
                    gYTotalBlue += (image[i + 1][j + 1].rgbtBlue * 1);
                }
            }
            // calc sums
            int XYsum[3];
            XYsum[0] = round(sqrt((double) (gXTotalRed * gXTotalRed) + (double) (gYTotalRed * gYTotalRed)));
            XYsum[1] = round(sqrt((double) (gXTotalGreen * gXTotalGreen) + (double) (gYTotalGreen * gYTotalGreen)));
            XYsum[2] = round(sqrt((double) (gXTotalBlue * gXTotalBlue) + (double) (gYTotalBlue * gYTotalBlue)));

            // cap max 255
            for (int xy = 0; xy < 3; xy++)
            {
                if (XYsum[xy] > 255)
                {
                    XYsum[xy] = 255;
                }
            }

            imageWithFilter[i][j].rgbtRed = XYsum[0];
            imageWithFilter[i][j].rgbtGreen = XYsum[1];
            imageWithFilter[i][j].rgbtBlue = XYsum[2];
        }
    }
    // copy image with edge filter to image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = imageWithFilter[h][w];
        }
    }
    return;
}
