#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[]) {
    
	//make sure #of arguments inputted is 2 otherwise ends script
	
	if (argc != 2){
		printf("Usage: ./vigenere k");
		return 1;
	}
	
	//checks if the second argument contains anything other than characters 
	for (int i = 0, n = strlen(argv[1]); i < n; i++){
		if (isalpha(argv[1][i]) == false) {
			printf("Invalid input; input must only contain letters");
			return 1;
		}
	}

	//asks user for string to be enciphered    
	printf("plaintext: ");
	string text = GetString();
	
	printf("ciphertext: ");
	//length of inputted key phrase
	int length = strlen(argv[1]);

	//loop over plaintext
	for (int i = 0, j = 0, cipher = 0, n = strlen(text); i < n; i++) {
		//get character to iterate
		char position = text[i];
		//get character to iterate
		char key = argv[1][j % length];
		
		//convert key to alphabetical index
		if (isupper(key)){
			key -= 65;
		}
		
		if (islower(key)){
			key -= 97;
		}
		
		//add the position with the key convert to alphabetical index add and then revert back
		//j++ to get to next character in key string
		if (isupper(position)){
			cipher = (position - 65 + key) % 26 + 65;
			j++;
		}
		
		else if (islower(position)){
			cipher = (position - 97 + key) % 26 + 97;
			j++;
		}
		
		//if string contains anything other than characters
		else {
			cipher = position;
		}
		printf("%c", cipher);
	}
	printf("\n");
}
