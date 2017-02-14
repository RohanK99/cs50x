#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    
    //prompt user for name
    string name = get_string();
    
    //make sure string does not create error
    if (name != NULL) {
        //print first initial
        printf("%c", toupper(name[0]));
        
        //find space and print character in string after it
        for (int i = 0; i < strlen(name); i++) {
            if (name[i] == ' ') {
                printf("%c", toupper(name[i+1]));
            }
        }
    }
    else {
        //exit function if NULL string created
        return 1;
    }
    //return new line
    printf("\n");
}
