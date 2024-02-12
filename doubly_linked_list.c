#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct Node {
   int data;
   int key;
   struct Node *next;
   struct Node *prev;
}Node;

Node *head = NULL;
Node *current = NULL;
Node *last = NULL;

int get_list_length(Node* head) {
    int length = 0;
    Node* current = head;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

Node *list_get_tail(Node **head)
{
    Node *doubly_linked_list = *head;
    Node *last_node = NULL;

    while (doubly_linked_list->next != NULL)
    {
        last_node = doubly_linked_list;
        doubly_linked_list = doubly_linked_list->next;
    }
    last_node = doubly_linked_list;

    return last_node;
}

void print_list(Node *head)
{
    printf("\n[");
    
    while(head != NULL)
    {
        printf(" %d,%d " ,head->key,head->data);
        head = head->next;
    }
    printf("]");
}

void add_first(Node **head, int key, int data)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->key = key;
    new_node->data = data;

    if ((*head) == NULL)
    {
        last = new_node;
    }
    else
    {
        (*head)->prev = new_node;
    }

    new_node->next = *head;

    *head = new_node;
}

void add_last(Node **head, int key, int data)
{    
    Node *new_node = malloc(sizeof(Node));
    new_node->key = key;
    new_node->data = data;

    Node *last_node = list_get_tail(head);
    last_node->next = new_node;
    new_node->prev = last_node;
    new_node->next = NULL;
}

bool add_after_node(Node** head, int nodeKey, int key, int data)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->key = key;
    new_node->data = data;

    Node *current = *head;

    //if list is empty
    if(current == NULL) return false;

    while(current->key != nodeKey)
    {
        //if it is last node
        if(current->next == NULL) 
        {
            return false;
        }
        else
        {
            current = current->next;
        }
    }

    if(current->next == NULL)
    {
        new_node->next = NULL;
        last = new_node;
    }
    else
    {
        new_node->next = current->next;
        current->next->prev = new_node;
    }

    new_node->prev = current;
    current->next = new_node;
    return true;
}

bool add_prev_node(Node** head, int nodeKey, int key, int data)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->key = key;
    new_node->data = data;

    Node *current = *head;

    //if list is empty
    if(current == NULL) return false;

    if (current->key == nodeKey)
    {
        add_first(head, key, data);
        return true;
    }
    

    while(current->key != nodeKey)
    {
        //if it is last node
        if(current->next == NULL) 
        {
            return false;
        }
        else
        {
            current = current->next;
        }
    }

    new_node->next = current;
    new_node->prev = current->prev;
    current->prev->next = new_node;
    return true;
}

void delete_first(Node **head)
{
    if (*head != NULL)
    {
        Node *temp = *head;
        *head = temp->next;
        (*head)->prev = NULL;
        print_list(*head);
        printf(" Deleted First Node key,data-> %d,%d", temp->key, temp->data);
        free(temp);
    }
}

void delete_last(Node **head)
{
   Node *currentNode = *head;
   Node *temp;

    if (*head != NULL && (*head)->next != NULL)
    {
        while (currentNode->next->next != NULL)
        {
            currentNode = currentNode->next;
        }

        temp = currentNode->next;
        currentNode->next = NULL;
        temp->prev = NULL;
        print_list(*head);
        printf(" Deleted last Node key,data-> %d,%d", temp->key, temp->data);
        free(temp);
        free(current);
    }
    else
    {
        *head = NULL;
        free(current);
    }
}

void delete_at_node(Node **head, int key)
{
    Node *current = *head;

    // If list is empty
    if (*head == NULL)
    {
        printf("List is empty!");
        return;
    }

    while (current->key != key)
    {
        if(current->next == NULL) 
        {
            print_list(*head);
            printf(" No key find");
            return;
        }
        else
        {
            current = current->next;
        }
    }
    
    if (current == *head) // If head node itself holds the key to be deleted
    {
        *head = current->next;
        (*head)->prev = NULL;
        print_list(*head);
        printf(" Deleted Node key,data-> %d,%d", current->key,current->data);
        free(current);
    }
    else if (current == list_get_tail(head)) // If last node hold the key 
    {
        current->prev->next = NULL;
        current->prev = NULL;
        print_list(*head);
        printf(" Deleted Node key,data-> %d,%d", current->key,current->data);
        free(current);
    }
    else
    {
        current->next->prev = current->prev;
        current->prev->next = current->next;
        print_list(*head);
        printf(" Deleted Node key,data-> %d,%d", current->key,current->data);
        free(current);
    }
}

void swap_nodes(Node *node1, Node *node2) {
    int tempKey = node1->key;
    int tempData = node1->data;
    node1->data = node2->data;
    node2->data = tempData;
    node1->key = node2->key;
    node2->key = tempKey;
}

void shuffle_list(Node *head) 
{
    srand(time(NULL));

    int length = get_list_length(head);
    Node *current = head;

    while (current != NULL) 
    {
        int randomIndex = rand() % length;
        Node *randomNode = head;

        // Find the random node
        for (int i = 0; i < randomIndex; i++) {
            randomNode = randomNode->next;
        }

        // Swap node's data
        swap_nodes(current, randomNode);

        current = current->next;
    }
}

void destroy(Node **head)
{
    Node *current = *head;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

int main(int argc, char **argv)
{
    add_first(&head, 1, 10);
    add_first(&head, 2, 20);
    add_first(&head, 3, 30);
    print_list(head);

    add_last(&head, 4, 40);
    print_list(head);

    add_after_node(&head, 2, 5, 50);
    print_list(head);

    add_prev_node(&head, 1, 6, 60);
    print_list(head);

    shuffle_list(head);
    print_list(head);
    printf(" Shuffled DoublyLinkedList");

    delete_first(&head);
    
    delete_at_node(&head, 1);

    delete_last(&head);

    destroy(&head);

    return 0;
}