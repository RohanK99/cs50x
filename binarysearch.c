#include <stdio.h>
#include <cs50.h>

int main()
{
   int c, n, value, values[100];
 
   printf("Enter number of elements\n");
   scanf("%d",&n);
 
   printf("Enter %d integers\n", n);
 
   for (c = 0; c < n; c++)
      scanf("%d",&values[c]);
 
   printf("Enter value to find\n");
   scanf("%d", &value);
   
   if (n < 0) {
        return false;
    }
    
   int firstIndex = 0;
   int lastIndex = n - 1;
   
    while (firstIndex <= lastIndex){
        int middleIndex = (firstIndex + lastIndex)/2;
        if (value == values[middleIndex]) {
        printf("%d found at location %d.\n", value, middleIndex+1);;
        return true;
        } else if (value < values[middleIndex]) {
        lastIndex = middleIndex - 1;
        } else if (value > values[middleIndex]) {
        firstIndex = middleIndex + 1;
        }
        else {
            break;
        }
    }
    return false;
    
   if (firstIndex > lastIndex)
      printf("Not found!);
 
   return 0;   
}
