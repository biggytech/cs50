// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Hash table size
int hashTableSize = 0;

// Helper function
void print_list(node *l)
{
    printf("print_list\n");
    printf("size %i\n", hashTableSize);
    node *list = l;
    while (list != NULL)
    {
        printf("%s ", list->word);
        list = list->next;
    }
    printf("\n");
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int wordHash = hash(word);
    node *list = table[wordHash];

    while (list != NULL)
    {
        if (strcasecmp(word, list->word) == 0)
        {
            return true;
        }
        else
        {
            list = list->next;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    char *word = malloc(LENGTH + 1);

    if (word == NULL)
    {
        printf("Cannot allocate memory for a word.\n");
        return false;
    }

    while (fscanf(file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            printf("Cannot allocate memory for a node.\n");
            return false;
        }

        strcpy(n->word, word);
        n->next = NULL;

        int nodeHash = hash(word);

        if (table[nodeHash] == NULL)
        {
            table[nodeHash] = n;
        }
        else
        {
            n->next = table[nodeHash];
            table[nodeHash] = n;
        }

        hashTableSize++;
    }

    free(word);
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return hashTableSize;
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
