#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do {
        printf("Height:");
        height = get_int();
    } while (height < 0 || height > 23); 
    for (int i = 0; i<height; i++) {
    int space, hash;
    for (space = i; space<height-1; space++) {
        printf(" ");
    }
    
    for (hash = 0; hash < i+2; hash++) {
        printf("#");
    }
        printf("\n");
}
}
