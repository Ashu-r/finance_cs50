// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

int words = 0;
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int this_hash = hash(word);
    if (table[this_hash] == NULL)
    {
        return false;
    }
    node *trav = table[this_hash];
    while (trav != NULL)
    {
        if (!(strcasecmp(word, trav->word)))
        {
            return true;
        }
        trav = trav->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // (djb2) this algorithm (k=33) was first reported by dan bernstein many years ago in
    // comp.lang.c
    // source: http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    int c;

    while ((c = tolower(*word++)))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    // hash(dictionary)
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        return false;
    }
    // This easy thing took hours :(
    // This just initializes to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    char cword[LENGTH + 1];
    while (true)
    {
        if (fscanf(f, "%s", cword) == EOF)
        {
            break;
        }
        node *each_word = malloc(sizeof(node));
        if (each_word == NULL)
        {
            return false;
        }
        strcpy(each_word->word, cword);

        unsigned int this_hash = hash(cword);
        words++;
        // node *temp = table[this->hash];

        if (table[this_hash] == NULL)
        {
            each_word->next = NULL;
            table[this_hash] = each_word;
        }
        else
        {
            each_word->next = table[this_hash];
            table[this_hash] = each_word;
        }
    }
    fclose(f);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *trav = table[i];
        node *temp = table[i];
        while (trav != NULL)
        {
            temp = trav;
            trav = trav->next;
            free(temp);
        }
    }
    return true;
}
