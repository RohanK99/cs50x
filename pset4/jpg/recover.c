#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//fix 8 bit size integer
typedef uint8_t BYTE;

#define BLOCK 512

int main(void){
    
    //open memory card file 
    FILE* file = fopen("card.raw", "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file\n");
        return 1;
    }
    
    //store file data in size
    BYTE size[512];
    
    //initialize variables to search for jpeg
    int jpgcount = 0;
    
    //output of written image
    FILE* outptr = NULL;
    char* filename = malloc(sizeof(char));
    
    while (fread(&size, sizeof(BYTE) * 512, 1, file) == 1) {
        //compare bytes to find start of jpg file
        if (size[0] == 255 && size[1] == 216 && size[2] == 255 && (size[3] == 224 || size[3] == 225)) {
            
            if (jpgcount != 0) {
                fclose(outptr);
            }
            
            jpgcount++;
            
            //open output file 
            sprintf(filename, "%03d.jpg", (jpgcount - 1));
            outptr = fopen(filename, "w");
            
            if (outptr == NULL) {
                printf("file could not be created\n");
                return 2;
            }
            
            //write data to outptr(jpg)
            fwrite(&size, sizeof(BYTE) * BLOCK, 1, outptr);
            
        } else if (jpgcount != 0) {
            
            fwrite(&size, sizeof(BYTE) * BLOCK, 1, outptr);
        }
    } 
    
    //close memory card file
    fclose(file);
    
    //close output
    fclose(outptr);
    
    //free memory in filename
    free(filename);
    
    //end
    return 0;
}