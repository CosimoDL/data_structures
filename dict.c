#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 10
#define LOAD_FACTOR_THRESHOLD 0.7

typedef struct dictionary_node
{
    char *key;
    char *value;
    struct dictionary_node *next;
} dictionary_node;

typedef struct dictionary
{
    size_t size;
    size_t capacity;
    dictionary_node **table;
} dictionary;

size_t djb33x_hash(const char *key, const size_t keylen)
{
    size_t hash = 5381;
    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}

dictionary *create_dictionary(size_t capacity)
{
    dictionary *dict = (dictionary *)malloc(sizeof(dictionary));
    dict->size = 0;
    dict->capacity = capacity;
    dict->table = (dictionary_node **)calloc(capacity, sizeof(dictionary_node *));
    return dict;
}

void free_dictionary(dictionary *dict)
{
    for (size_t i = 0; i < dict->capacity; i++)
    {
        dictionary_node *current = dict->table[i];
        while (current != NULL)
        {
            dictionary_node *next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            current = next;
        }
    }
    free(dict->table);
    free(dict);
}

void print_dictionary(dictionary *dict)
{
    printf("\nDictionary content:\n");
    for (size_t i = 0; i < dict->capacity; i++)
    {
        printf("Node %zu: ", i);
        dictionary_node *current = dict->table[i];
        while (current != NULL)
        {
            printf("(%s: %s) ", current->key, current->value);
            current = current->next;
        }
        printf("\n");
    }
}

void resize(dictionary *dict, size_t new_capacity)
{
    dictionary *new_dict = create_dictionary(new_capacity);
    for (size_t i = 0; i < dict->capacity; i++)
    {
        dictionary_node *current = dict->table[i];
        while (current != NULL)
        {
            dictionary_node *next = current->next;
            size_t index = djb33x_hash(current->key, strlen(current->key)) % new_capacity;
            current->next = new_dict->table[index];
            new_dict->table[index] = current;
            current = next;
        }
    }
    free(dict->table);
    dict->table = new_dict->table;
    dict->capacity = new_capacity;
    free(new_dict);
}

const char *find(dictionary *dict, const char *key)
{
    size_t index = djb33x_hash(key, strlen(key)) % dict->capacity;
    dictionary_node *current = dict->table[index];
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void insert(dictionary *dict, const char *key, const char *value)
{
    if (!find(dict, key))
    {
        size_t index = djb33x_hash(key, strlen(key)) % dict->capacity;
        dictionary_node *new_node = (dictionary_node *)malloc(sizeof(dictionary_node));
        new_node->key = _strdup(key);
        new_node->value = _strdup(value);
        new_node->next = dict->table[index];
        dict->table[index] = new_node;
        dict->size++;

        // Check load factor
        double load_factor = (double)dict->size / dict->capacity;
        if (load_factor > LOAD_FACTOR_THRESHOLD)
        {
            print_dictionary(dict);
            printf("\nResize needed, dictionary after resize\n");
            size_t new_capacity = dict->capacity * 2;
            resize(dict, new_capacity);
            print_dictionary(dict);
        }
    }
}

void remove_entry(dictionary *dict, const char *key)
{
    size_t index = djb33x_hash(key, strlen(key)) % dict->capacity;
    dictionary_node *prev = NULL;
    dictionary_node *current = dict->table[index];

    // Find the key
    while (current != NULL && strcmp(current->key, key) != 0)
    {
        prev = current;
        current = current->next;
    }

    // Key found
    if (current != NULL)
    {
        // Key is the head
        if (prev == NULL)
        {
            dict->table[index] = current->next;
        }
        else
        {
            prev->next = current->next;
        }

        printf("\nKey: (%s) Value: (%s) removed!\n", current->key, current->value);

        free(current->key);
        free(current->value);
        free(current);
        dict->size--;
    }
}

int main()
{
    dictionary *dict = create_dictionary(INITIAL_CAPACITY);

    insert(dict, "key1", "value1");
    insert(dict, "key2", "value2");
    insert(dict, "key3", "value3");
    insert(dict, "key4", "value4");
    insert(dict, "key5", "value5");
    insert(dict, "key6", "value6");
    insert(dict, "key7", "value7");
    insert(dict, "key8", "value8");
    insert(dict, "key9", "value9");

    print_dictionary(dict);

    remove_entry(dict, "key4");

    const char *result = find(dict, "key2");
    if (result != NULL)
    {
        printf("\nValue found: %s\n", result);
    }
    else
    {
        printf("\nKey not found\n");
    }

    free_dictionary(dict);
    return 0;
}
