/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }
    
    //create scale factor
    int n = atoi(argv[1]);
       
    // ensure n is is a positive int less than or equal to 100
    if(n > 100 || n < 1)
    {
        printf("n must be between 1 and 100\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    } 
    
    //original width and height
    int width = bi.biWidth;
    int height = bi.biHeight;
   
    // scaled width and height
    bi.biWidth *= n;
    bi.biHeight *= n;
       
    // store padding
    int originalpadding =  (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // update image size
    bi.biSizeImage = abs(bi.biHeight) * ((bi.biWidth * sizeof (RGBTRIPLE)) + padding);
    
    // update file size
    bf.bfSize = bi.biSizeImage + sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER); 

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // allocate storage for scanline
    RGBTRIPLE *buffer = malloc(sizeof(RGBTRIPLE) * (bi.biWidth));

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(height); i < biHeight; i++)
    {
        // define integer that tracks location
        int seek = 0;
        // iterate over pixels in scanline
        for (int j = 0; j < width; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            //write pixel into buffer memory
            for(int count = 0; count < n; count++)
            {
                *(buffer+(seek)) = triple;
                seek++;
            }
        }
            
        // skip over padding, if any
        fseek(inptr, originalpadding, SEEK_CUR);

        // write RGBTRIPLE to outfile
           for(int r = 0; r < n; r++)
           {
                fwrite((buffer), sizeof(RGBTRIPLE), bi.biWidth, outptr);

                // write padding to outfile
                for (int k = 0; k < padding; k++)
                    fputc(0x00, outptr);
           }        
    }

    // free memory from buffer
    free(buffer);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
