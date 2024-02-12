#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 10

typedef struct set_node
{
    char *key;
    struct set_node *next;
} set_node;

typedef struct set
{
    size_t size;
    size_t capacity;
    set_node **table;
} set;

size_t djb33x_hash(const char *key, const size_t keylen)
{
    size_t hash = 5381;
    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}

set *create_set(size_t capacity)
{
    set *s = (set *)malloc(sizeof(set));
    s->size = 0;
    s->capacity = capacity;
    s->table = (set_node **)calloc(capacity, sizeof(set_node *));
    return s;
}

void free_set(set *s)
{
    for (size_t i = 0; i < s->capacity; i++)
    {
        set_node *current = s->table[i];
        while (current != NULL)
        {
            set_node *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(s->table);
    free(s);
}

bool contains(const set *s, const char *key)
{
    size_t index = djb33x_hash(key, strlen(key)) % s->capacity;
    set_node *current = s->table[index];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            printf("The key find %s", key);
            return true;
        }
        current = current->next;
    }
    printf("Key not found %s", key);
    return false;
}

void insert(set *s, const char *key)
{
    if (contains(s, key))
    {
        return; //uique key check
    }
    size_t index = djb33x_hash(key, strlen(key)) % s->capacity;
    set_node *new_node = (set_node *)malloc(sizeof(set_node));
    new_node->key = strdup(key);
    new_node->next = s->table[index];
    s->table[index] = new_node;
    s->size++;
}

void remove(set *s, const char *key)
{
    size_t index = djb33x_hash(key, strlen(key)) % s->capacity;
    set_node *prev = NULL;
    set_node *current = s->table[index];

    // find the key
    while (current != NULL && strcmp(current->key, key) != 0)
    {
        prev = current;
        current = current->next;
    }

    // key found
    if (current != NULL)
    {
        // the head is the key
        if (prev == NULL)
        {
            s->table[index] = current->next;
        }
        else
        {
            prev->next = current->next;
        }

        free(current->key);
        free(current);
        s->size--;
    }
}

void print_set(const set *s)
{
    printf("Set content:\n");
    for (size_t i = 0; i < s->capacity; i++)
    {
        printf("Bucket %zu: ", i);
        set_node *current = s->table[i];
        while (current != NULL)
        {
            printf("%s ", current->key);
            current = current->next;
        }
        printf("\n");
    }
}

int main()
{
    set *s = create_set(INITIAL_CAPACITY);

    insert(s, "key1");
    insert(s, "key2");
    insert(s, "key3");

    print_set(s);

    remove(s, "key2");

    print_set(s);

    free_set(s);
    return 0;
}
