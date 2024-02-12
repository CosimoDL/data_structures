#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HASHMAP_SIZE 64
#define HASHMAP_SIZE_LIST 5

typedef struct set_node
{
    const char *key;
    size_t key_len;
    struct set_node *next;
}set_node;

typedef struct set_table
{
    struct set_node hashmap[HASHMAP_SIZE][HASHMAP_SIZE_LIST];
}set_table;

size_t djb33x_hash(const char *key, const size_t keylen)
{
    size_t hash = 5381;

    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }

    return hash;
}

void print_set(set_table *set)
{
    for (size_t index = 0; index < HASHMAP_SIZE; index++)
    {
        for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++)
        {
            if (set->hashmap[index][i].key_len != 0)
            {
                printf("%s at index %llu slot %llu\n", set->hashmap[index][i].key, index, i);
            }
        }
    }
}

void insert(set_table *set, const char *key)
{
    const size_t key_len = strlen(key);

    const size_t hash = djb33x_hash(key, key_len);

    const size_t index = hash % HASHMAP_SIZE;

    printf("hash of %s = %llu (index: %llu)\n", key, hash, index);

    for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++)
    {
        if (set->hashmap[index][i].key_len == 0)
        {
            set->hashmap[index][i].key = _strdup(key);
            set->hashmap[index][i].key_len = key_len;
            set->hashmap[index][i].next = NULL;
            printf("added %s at index %llu slot %llu\n", key, index, i);
            
            return;
        }
    }

    printf("COLLISION! for %s (index %llu)\n", key, index);
}

void remove(set_table *set, const char *key)
{
    const size_t key_len = strlen(key);

    const size_t hash = djb33x_hash(key, key_len);

    const size_t index = hash % HASHMAP_SIZE;
    
    set_node *current = NULL, *prev = NULL;

    for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++)
    {
        current = &(set->hashmap[index][i]);

        while (current != NULL && current->key_len > 0)
        {
            if (current->key_len == key_len && !memcmp(current->key, key, key_len))
            {
                printf("Removing %s from index %llu slot %llu\n", key, index, i);

                // head is the key
                if (prev != NULL)
                {
                    prev->next = current->next;
                }
                else
                {
                    if (current->next != NULL)
                    {
                        current->key = current->next->key;
                        current->key_len = current->next->key_len;

                        set_node *temp = current->next;
                        current->next = temp->next;
                        free((void *)temp->key);
                        free(temp);
                    }
                    else
                    {
                        current->key_len = 0;
                    }
                }

                free((void *)current->key);

                return;
            }

            prev = current;
            current = current->next;
        }
    }
    printf("The key: %s cannot be removed, key not found\n", key);
}

void find(set_table *set, const char *key)
{
    const size_t key_len = strlen(key);

    const size_t hash = djb33x_hash(key, key_len);

    const size_t index = hash % HASHMAP_SIZE;

    printf("hash of %s = %llu (index: %llu)\n", key, hash, index);

    for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++)
    {
        if (set->hashmap[index][i].key_len > 0)
        {
            if (set->hashmap[index][i].key_len == key_len && !memcmp(set->hashmap[index][i].key, key, key_len))
            {
                printf("FOUND %s at index %llu slot %llu\n", key, index, i);
                return;
            }
        }
    }

    printf("The key: %s was not found\n", key);
}

void destroy(set_table *set)
{
    for (size_t index = 0; index < HASHMAP_SIZE; index++)
    {
        for (size_t i = 0; i < HASHMAP_SIZE_LIST; i++)
        {
            if (set->hashmap[index][i].key_len != 0)
            {
                // Free the memory allocated for the key
                free((void *)set->hashmap[index][i].key);
            }
        }
    }

    // After freeing the key memory reset the set to an initial state
    memset(set, 0, sizeof(set_table));
}


int main(int argc, char **argv)
{
    set_table myset;
    memset(&myset, 0, sizeof(set_table));

    insert(&myset, "Hello");
    insert(&myset, "Test");
    insert(&myset, "XYZ");
    insert(&myset, "AAAAAA");
    insert(&myset, "AAAAAA");
    insert(&myset, "AAAAAA");
    insert(&myset, "AAAAAA");
    remove(&myset, "AAAAAA");
    insert(&myset, "AAAAAA");
    insert(&myset, "AAAAAA");


    find(&myset, "XYZ");

    remove(&myset, "Test");

    destroy(&myset);
    
    return 0;
}