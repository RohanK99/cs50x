#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[]) {
    if (argc != 2){
        printf("Missing command-line argument\n");
        return 1;
    }
    
    if (argc == 2){
        //declare variable i and assign it corresponding int value from its string in the array
        int i;
        i = atoi(argv[1]);
        //ask user for string to convert to ciphertext
        printf("plaintext: ");
        string s = get_string();
        
        printf("ciphertext: ");
        for (int k = 0, n = strlen(s); k < n; k++) {
            //check if input is a letter
            if (isalpha(s[k])) {
                //preserve capitalization
                if (isupper(s[k])) {
                    int encipher = (s[k] + i - 65) % 26 + 65;
                    printf("%c", encipher);
                }
                //preserve lowercase letters
                else {
                    int encipher = (s[k] + i - 97) % 26 + 97;
                    printf("%c", encipher);
            }
        }
        //for all other characters
        else {
            printf("%c", s[k]);
        }
    }
    //print new line
    printf("\n");
}
}
