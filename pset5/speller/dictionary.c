#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Number of linked lists in hash table.
#define hashsize 179479

// define struct with word element that points to next node
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

// Hash table is an array of linked lists.
node* hashtable[hashsize];

int hash_function(const char* word);

// number of words in dictionary
int wordcount = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int len = strlen(word);
    char lowercase[LENGTH+1];
    
    // Convert word to lowercase to compare w/ hash table
    for (int i = 0; i < len; i++)
    {
        if(isupper(word[i])) {
            lowercase[i] = tolower(word[i]) ;
        } else {
            lowercase[i] = word[i];
        }
    }
    
    // Add null character to end of char array.
    lowercase[len] = '\0';
    
    // Use hash function to correctly place word
    int head = hash_function(lowercase);
    
    // Set cursor to first node in head.
    node* cursor = hashtable[head];   
    
    //compare the word the cursor is pointing to an the one hashed, if they are the same then word is spelled correctly
    while (cursor != NULL)
    {
        if (strcmp(lowercase, cursor->word) == 0) 
        {
            return true;
        }
        cursor = cursor->next;
    }
    
    return false;
}


/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    //set all has values to null
    for(int i = 0; i < hashsize; i++)
    {
        hashtable[i] = NULL;
    }   
    
    // Open the dictionary 
    FILE* dict;    
    dict = fopen(dictionary, "r"); 
      
    // Check if dictionary opened correctly.
    if (dict == NULL)
    {
        printf("Could not load dictionary\n");
        return false;
    }  
    
    char buffer[LENGTH+1];
    
    //loop through file and save all words
    while (fscanf(dict, "%s", buffer) > 0)
    {
        // Allocate memory for a new node.
        node* newnode = malloc(sizeof(node));
        
        // Set node's next pointer to NULL.
        newnode->next = NULL;
        
        // node's word to buffer value
        strcpy(newnode->word, buffer);
        
        //find location of next word
        int head = hash_function(newnode->word);
        
        //if first node replace null with new node
        if (hashtable[head] == NULL)
        {
            hashtable[head] = newnode;
        }
        // set new node's pointer to the first node 
        // Then set new node to be the first node in the linked list
        else
        {
            newnode->next = hashtable[head];
            hashtable[head] = newnode;
        }

        wordcount++;
    }
    
    //close dictionary
    fclose(dict);
    return true;
}

unsigned int size(void)
{
    return wordcount;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // loop through and cursor point at all hashed values
    for (int i = 0; i < hashsize;i++)
    {
        node* cursor = hashtable[i];
        while (cursor != NULL)
        {
            //set temp to point at cursors location, move cursor to next node and free temp 
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    } 
    return true;
}

//hash function taken from the internet
//finds correct place for the word
int hash_function(const char* word)
{
    int sum = 0;
    int len = strlen(word);

    for (int i = 0; i < len; i++)
    {
        sum += word[i];
    }
    
    int head = sum % hashsize;
    return head;
}