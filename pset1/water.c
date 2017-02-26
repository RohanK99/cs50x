#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("How many minutes do you shower?");
    int x = get_int();
    printf("Shower: %i\n", x);
    printf("Bottles: %i\n", x*192/16);
}
