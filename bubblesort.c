#include <stdio.h>

int main()
{
    int values[100],i,n,j,temp;
    printf("Enter the number of elements to be sorted: ");
    scanf("%d",&n);
    for(i=0; i < n; i++)
    
    {
        printf("%d. Enter number: ",i+1);
        scanf("%d", &values[i]);
    }

    for (i = 0; i < n - 1; i++) {
      for (j = 0; j < n - 1; j++) {
         if (values[j] > values[j + 1]) {
            temp = values[j];
            values[j] = values[j + 1];
            values[j + 1] = temp;
         }
      }
    }
    
    printf("Increasing order: ");
    for(i=0;i<n;++i)
         printf("%d  ",values[i]);
    return 0;
}

