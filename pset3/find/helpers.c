/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */

bool search(int value, int values[], int n)
{
    if (n < 0) {
        return false;
    }
    
    int firstIndex = 0;
    int lastIndex = n-1;
    while (firstIndex <= lastIndex){
        int middleIndex = (firstIndex + lastIndex)/2;
        if (value == values[middleIndex]) {
        printf("Found the needle");
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
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int i, j, temp;
 
    for (i = 1; i < n; i++) {
      for (j = 0; j < n - 1; j++) {
         if (values[j] > values[j + 1]) {
            temp = values[j];
            values[j] = values[j + 1];
            values[j + 1] = temp;
         }
      }
    // TODO: implement an O(n^2) sorting algorithm
    return;
}
}
