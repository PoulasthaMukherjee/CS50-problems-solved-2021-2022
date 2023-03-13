#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Find average of RGB in a pixel
            int avg = round(((float)image[i][j].rgbtRed + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtBlue) / 3);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            //Sepia formula
            int sRed = round((0.393 * red) + (0.769 * green) + (0.189 * blue));
            //Max color intensity can be 255 for RGB
            if (sRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sRed;
            }
            int sGreen = round((0.349 * red) + (0.686 * green) + (0.168 * blue));
            if (sGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sGreen;
            }
            int sBlue = round((0.272 * red) + (0.534 * green) + (0.131 * blue));
            if (sBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sBlue;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            //Swap pixels right to left and vice-versa using temp
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Create temp image to store blurred values
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int totalR;
            int totalG;
            int totalB;
            float counter;
            totalR = totalG = totalB = counter = 0;

            //Bottom right corner pixel
            if (i >= 0 && j >= 0)
            {
                totalR += temp[i][j].rgbtRed;
                totalG += temp[i][j].rgbtGreen;
                totalB += temp[i][j].rgbtBlue;
                counter++;
            }

            //Bottom left corner pixel
            if (i >= 0 && j - 1 >= 0)
            {
                totalR += temp[i][j - 1].rgbtRed;
                totalG += temp[i][j - 1].rgbtGreen;
                totalB += temp[i][j - 1].rgbtBlue;
                counter++;
            }

            //Top left corner pixel
            if (i - 1 >= 0 && j >= 0)
            {
                totalR += temp[i - 1][j].rgbtRed;
                totalG += temp[i - 1][j].rgbtGreen;
                totalB += temp[i - 1][j].rgbtBlue;
                counter++;
            }

            //Top right corner pixel
            if (i - 1 >= 0 && j - 1 >= 0)
            {
                totalR += temp[i - 1][j - 1].rgbtRed;
                totalG += temp[i - 1][j - 1].rgbtGreen;
                totalB += temp[i - 1][j - 1].rgbtBlue;
                counter++;
            }

            //Bottom edge pixels
            if ((i >= 0 && j + 1 >= 0) && (i >= 0 && j + 1 < width))
            {
                totalR += temp[i][j + 1].rgbtRed;
                totalG += temp[i][j + 1].rgbtGreen;
                totalB += temp[i][j + 1].rgbtBlue;
                counter++;
            }

            //Top edge pixels
            if ((i - 1 >= 0 && j + 1 >= 0) && (i - 1 >= 0 && j + 1 < width))
            {
                totalR += temp[i - 1][j + 1].rgbtRed;
                totalG += temp[i - 1][j + 1].rgbtGreen;
                totalB += temp[i - 1][j + 1].rgbtBlue;
                counter++;
            }

            //Left edge pixels
            if ((i + 1 >= 0 && j >= 0) && (i + 1 < height && j >= 0))
            {
                totalR += temp[i + 1][j].rgbtRed;
                totalG += temp[i + 1][j].rgbtGreen;
                totalB += temp[i + 1][j].rgbtBlue;
                counter++;
            }

            //Right edge pixels
            if ((i + 1 >= 0 && j - 1 >= 0) && (i + 1 < height && j - 1 >= 0))
            {
                totalR += temp[i + 1][j - 1].rgbtRed;
                totalG += temp[i + 1][j - 1].rgbtGreen;
                totalB += temp[i + 1][j - 1].rgbtBlue;
                counter++;
            }

            //Middle pixels
            if ((i + 1 >= 0 && j + 1 >= 0) && (i + 1 < height && j + 1 < width))
            {
                totalR += temp[i + 1][j + 1].rgbtRed;
                totalG += temp[i + 1][j + 1].rgbtGreen;
                totalB += temp[i + 1][j + 1].rgbtBlue;
                counter++;
            }

            //Put average values of RGB on pixels
            image[i][j].rgbtRed = round(totalR / counter);
            image[i][j].rgbtGreen = round(totalG / counter);
            image[i][j].rgbtBlue = round(totalB / counter);
        }
    }
    return;
}