// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = LENGTH - 1;

// Hash table
node *table[N];

// hash table size
int tbSize = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hashIndex = hash(word);
    node *cursor = table[hashIndex];
    while (cursor != NULL)
    {

        // converts word to lower case
        char *wordCopy = malloc((strlen(word) + 1) * sizeof(char));
        strcpy(wordCopy, word);

        for (int i = 0; i < strlen(wordCopy); i++)
        {
            wordCopy[i] = tolower(wordCopy[i]);
        }

        if (strcmp(cursor->word, wordCopy) == 0)
        {
            free(wordCopy);
            return true;
        }
        free(wordCopy);
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return strlen(word) - 1;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // set all hash table pointers to null
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    // read words
    char word[LENGTH + 1];
    while (fscanf(dict, "%s", word) != EOF)
    {
        // process the word into the hash table
        node *newNode = malloc(sizeof(node));
        int hashIndex = hash(word);
        strcpy(newNode->word, word);
        newNode->next = table[hashIndex];
        table[hashIndex] = newNode;
        tbSize++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return tbSize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
