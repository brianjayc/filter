#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop to each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // get the average of RGB and set it as the new value for RGB
            uint8_t avg = round(((float)image[i][j].rgbtRed + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtBlue) / (float)3);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }

    return;
}

// Reflect image horizontally
void swap(uint8_t *x, uint8_t *y)
{
    uint8_t tmp = *x;
    *x = *y;
    *y = tmp;
}

void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //loop every pixel upto the middle pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // swap pixels from the left to the equivalent pixel on the right
            swap(&image[i][j].rgbtRed, &image[i][width - 1 - j].rgbtRed);
            swap(&image[i][j].rgbtBlue, &image[i][width - 1 - j].rgbtBlue);
            swap(&image[i][j].rgbtGreen, &image[i][width - 1 - j].rgbtGreen);
        }
    }

    return;
}



// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create a new blurred image
    RGBTRIPLE blurredImage[height][width];

    // loop every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //declare variables to get avg RGB
            float totalPixels = 1;
            float avgRed = image[i][j].rgbtRed;
            float avgBlue = image[i][j].rgbtBlue;
            float avgGreen = image[i][j].rgbtGreen;

            //get pixel value sorrounding the current pixel and add it to avg RGB, also increment totalPixel by 1

            if (i > 0 && j > 0) //top left
            {
                avgRed += image[i - 1][j - 1].rgbtRed;
                avgBlue += image[i - 1][j - 1].rgbtBlue;
                avgGreen += image[i - 1][j - 1].rgbtGreen;
                totalPixels += 1;
            }

            if (i > 0) //top
            {
                avgRed += image[i - 1][j].rgbtRed;
                avgBlue += image[i - 1][j].rgbtBlue;
                avgGreen += image[i - 1][j].rgbtGreen;
                totalPixels += 1;
            }

            if (i > 0 && j < width - 1) //top right
            {
                avgRed += image[i - 1][j + 1].rgbtRed;
                avgBlue += image[i - 1][j + 1].rgbtBlue;
                avgGreen += image[i - 1][j + 1].rgbtGreen;
                totalPixels += 1;
            }

            if (j < width - 1) //right
            {
                avgRed += image[i][j + 1].rgbtRed;
                avgBlue += image[i][j + 1].rgbtBlue;
                avgGreen += image[i][j + 1].rgbtGreen;
                totalPixels += 1;
            }

            if (i < height - 1 && j < width - 1) //bottom right
            {
                avgRed += image[i + 1][j + 1].rgbtRed;
                avgBlue += image[i + 1][j + 1].rgbtBlue;
                avgGreen += image[i + 1][j + 1].rgbtGreen;
                totalPixels += 1;
            }

            if (i < height - 1) //bottom
            {
                avgRed += image[i + 1][j].rgbtRed;
                avgBlue += image[i + 1][j].rgbtBlue;
                avgGreen += image[i + 1][j].rgbtGreen;
                totalPixels += 1;
            }

            if (i < height - 1 && j > 0) //bottom left
            {
                avgRed += image[i + 1][j - 1].rgbtRed;
                avgBlue += image[i + 1][j - 1].rgbtBlue;
                avgGreen += image[i + 1][j - 1].rgbtGreen;
                totalPixels += 1;
            }

            if (j > 0) //left
            {
                avgRed += image[i][j - 1].rgbtRed;
                avgBlue += image[i][j - 1].rgbtBlue;
                avgGreen += image[i][j - 1].rgbtGreen;
                totalPixels += 1;
            }

            // get average by dividing total RGB by the total pixels

            avgRed = avgRed / totalPixels;
            avgBlue = avgBlue / totalPixels;
            avgGreen = avgGreen / totalPixels;

            // set current pixel in the blurred image to the average RGB value above

            blurredImage[i][j].rgbtRed = round(avgRed);
            blurredImage[i][j].rgbtGreen = round(avgGreen);
            blurredImage[i][j].rgbtBlue = round(avgBlue);
        }
    }

    //copy blurred image to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = blurredImage[i][j].rgbtRed;
            image[i][j].rgbtGreen = blurredImage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = blurredImage[i][j].rgbtBlue;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create an edged image
    RGBTRIPLE edgedImage[height][width];

    // loop every pixel of image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //declare gx variables for RGB
            float gxRed = image[i][j].rgbtRed * 0;
            float gxBlue = image[i][j].rgbtBlue * 0;
            float gxGreen = image[i][j].rgbtGreen * 0;

            //get pixel value sorrounding the current pixel multiplied by the corresponding gx sobel operator
            //and add it to gx RGB

            if (i > 0 && j > 0) //top left
            {
                gxRed += image[i - 1][j - 1].rgbtRed * -1;
                gxBlue += image[i - 1][j - 1].rgbtBlue * -1;
                gxGreen += image[i - 1][j - 1].rgbtGreen * -1;
            }

            if (i > 0) //top
            {
                gxRed += image[i - 1][j].rgbtRed * 0;
                gxBlue += image[i - 1][j].rgbtBlue * 0;
                gxGreen += image[i - 1][j].rgbtGreen * 0;
            }

            if (i > 0 && j < width - 1) //top right
            {
                gxRed += image[i - 1][j + 1].rgbtRed * 1;
                gxBlue += image[i - 1][j + 1].rgbtBlue * 1;
                gxGreen += image[i - 1][j + 1].rgbtGreen * 1;
            }

            if (j < width - 1) //right
            {
                gxRed += image[i][j + 1].rgbtRed * 2;
                gxBlue += image[i][j + 1].rgbtBlue * 2;
                gxGreen += image[i][j + 1].rgbtGreen * 2;
            }

            if (i < height - 1 && j < width - 1) //bottom right
            {
                gxRed += image[i + 1][j + 1].rgbtRed * 1;
                gxBlue += image[i + 1][j + 1].rgbtBlue * 1;
                gxGreen += image[i + 1][j + 1].rgbtGreen * 1;
            }

            if (i < height - 1) //bottom
            {
                gxRed += image[i + 1][j].rgbtRed * 0;
                gxBlue += image[i + 1][j].rgbtBlue * 0;
                gxGreen += image[i + 1][j].rgbtGreen * 0;
            }

            if (i < height - 1 && j > 0) //bottom left
            {
                gxRed += image[i + 1][j - 1].rgbtRed * -1;
                gxBlue += image[i + 1][j - 1].rgbtBlue * -1;
                gxGreen += image[i + 1][j - 1].rgbtGreen * -1;
            }

            if (j > 0) //left
            {
                gxRed += image[i][j - 1].rgbtRed * -2;
                gxBlue += image[i][j - 1].rgbtBlue * -2;
                gxGreen += image[i][j - 1].rgbtGreen * -2;
            }

            //square gx RGB variables
            gxRed *= gxRed;
            gxBlue *= gxBlue;
            gxGreen *= gxGreen;

            //declare gy variables for RGB
            float gyRed = image[i][j].rgbtRed * 0;
            float gyBlue = image[i][j].rgbtBlue * 0;
            float gyGreen = image[i][j].rgbtGreen * 0;

            //get pixel value sorrounding the current pixel multiplied by the corresponding gy sobel operator
            //and add it to gy RGB

            if (i > 0 && j > 0) //top left
            {
                gyRed += image[i - 1][j - 1].rgbtRed * -1;
                gyBlue += image[i - 1][j - 1].rgbtBlue * -1;
                gyGreen += image[i - 1][j - 1].rgbtGreen * -1;
            }

            if (i > 0) //top
            {
                gyRed += image[i - 1][j].rgbtRed * -2;
                gyBlue += image[i - 1][j].rgbtBlue * -2;
                gyGreen += image[i - 1][j].rgbtGreen * -2;
            }

            if (i > 0 && j < width - 1) //top right
            {
                gyRed += image[i - 1][j + 1].rgbtRed * -1;
                gyBlue += image[i - 1][j + 1].rgbtBlue * -1;
                gyGreen += image[i - 1][j + 1].rgbtGreen * -1;
            }

            if (j < width - 1) //right
            {
                gyRed += image[i][j + 1].rgbtRed * 0;
                gyBlue += image[i][j + 1].rgbtBlue * 0;
                gyGreen += image[i][j + 1].rgbtGreen * 0;
            }

            if (i < height - 1 && j < width - 1) //bottom right
            {
                gyRed += image[i + 1][j + 1].rgbtRed * 1;
                gyBlue += image[i + 1][j + 1].rgbtBlue * 1;
                gyGreen += image[i + 1][j + 1].rgbtGreen * 1;
            }

            if (i < height - 1) //bottom
            {
                gyRed += image[i + 1][j].rgbtRed * 2;
                gyBlue += image[i + 1][j].rgbtBlue * 2;
                gyGreen += image[i + 1][j].rgbtGreen * 2;
            }

            if (i < height - 1 && j > 0) //bottom left
            {
                gyRed += image[i + 1][j - 1].rgbtRed * 1;
                gyBlue += image[i + 1][j - 1].rgbtBlue * 1;
                gyGreen += image[i + 1][j - 1].rgbtGreen * 1;
            }

            if (j > 0) //left
            {
                gyRed += image[i][j - 1].rgbtRed * 0;
                gyBlue += image[i][j - 1].rgbtBlue * 0;
                gyGreen += image[i][j - 1].rgbtGreen * 0;
            }

            //square gy RGB variables
            gyRed *= gyRed;
            gyBlue *= gyBlue;
            gyGreen *= gyGreen;

            //get total of gx and gy RGB then get sqrt
            float totalRed = sqrt(gxRed + gyRed);
            float totalBlue = sqrt(gxBlue + gyBlue);
            float totalGreen = sqrt(gxGreen + gyGreen);


            //set edged image current pixel using the total RGB from above, value is capped by 255
            if (totalRed > 255)
            {
                edgedImage[i][j].rgbtRed = 255;
            }
            else
            {
                edgedImage[i][j].rgbtRed = round(totalRed);
            }

            if (totalBlue > 255)
            {
                edgedImage[i][j].rgbtBlue = 255;
            }
            else
            {
                edgedImage[i][j].rgbtBlue = round(totalBlue);
            }

            if (totalGreen > 255)
            {
                edgedImage[i][j].rgbtGreen = 255;
            }
            else
            {
                edgedImage[i][j].rgbtGreen = round(totalGreen);
            }
        }
    }

    //copy edged image to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = edgedImage[i][j].rgbtRed;
            image[i][j].rgbtGreen = edgedImage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = edgedImage[i][j].rgbtBlue;
        }
    }

    return;
}
