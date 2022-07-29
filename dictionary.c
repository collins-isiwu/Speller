// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int counter = 0;

// Number of buckets in hash table
const unsigned int N = 8849;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //hash the word and insert the hash value in the variable key
    int key = hash(word);
    //create a temporary node to be used to search the hashtable
    //assign the hash value of the word being searched to the hashtable
    node *cursor = table[key];
    //iterate over cursor till the end of the it
    while (cursor != NULL)
    {
        //compare while ignoring case
        if (strcasecmp(word, cursor -> word) == 0)
        {
            return true;
        }
        //move to the next cursor
        cursor = cursor -> next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))  // *str++ is going to the next address in memory, where the next char in the string is stored
    {
        if (isupper(c))
        {
            //change to islower if isupper
            c += 32; 
        }
        hash = ((hash << 5) + hash) + (c); // hash * 33 + c  // hash << 5 = hash * 2^5
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //open the dictionary file
    FILE *name = fopen(dictionary, "r");
    if (!name)
    {
        printf("dictionary not found\n");
        return false;
    }

    char word[LENGTH + 1];
    //read strings from file one at a time
    while (fscanf(name, "%s", word) != EOF)
    {
        //create a new node
        node *tmp = malloc(sizeof(node));
        if (tmp == NULL)
        {
            printf("malloc couldn't allocate memory");
            unload();
            return false;
        }
        //copy word into node
        strcpy(tmp->word, word);
        //Set the pointer in our node
        tmp->next = table[hash(word)];
        //Save address of first node in the hashtable
        table[hash(word)] = tmp;
        counter++;
    }
    fclose(name);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //iterate over the number of buckets N
    for (int i = 0; i < N; i++)
    {
        //assign the address of the hash table to node cursor
        node *cursor = table[i];
        //iterate over cursor till the end of the
        while (cursor != NULL)
        {
            // node temp helps us to free space while we move to the next cursor
            node *temp = cursor;
            cursor = cursor -> next;
            free(temp);
        }
    }
    //you can only return true in this type of function
    return true;
}
