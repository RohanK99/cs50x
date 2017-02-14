#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void) {
    float money;
    do {
    printf("O hai! How much change is owed?\n");
    money = get_float();
    } while (money <= 0.00);
    
    int change = round(money*100);
    int xCoins = 0;
    
    while (change >= 25)
     {
         xCoins++;
         change -= 25;
     }

     while (change >= 10)
     {
         xCoins++;
         change -= 10;
     }

     while (change >= 5)
     {
         xCoins++;
         change -= 5;
     }

     while (change >= 1)
     {
         xCoins++;
         change -= 1;
     }
    printf("%d\n", xCoins);
}
