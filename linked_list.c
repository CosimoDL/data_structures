#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
}Node;

Node *head = NULL;
Node *current = NULL;

Node *list_get_tail(Node **head)
{
    Node *linked_list = *head;
    Node *last_node = NULL;

    while (linked_list->next != NULL)
    {
        last_node = linked_list;
        linked_list = linked_list->next;
    }
    last_node = linked_list;

    return last_node;
}

void print_linked_list(Node *head)
{
    printf("\n[");
    
    while(head != NULL)
    {
        printf(" %d " ,head->data);
        head = head->next;
    }
    printf("]");
}

void add_first(Node **head, int data)
{
    Node *new_node = malloc(sizeof(Node));

    new_node->data = data;

    new_node->next = *head;

    *head = new_node;
}

void add_last(Node **head, int data)
{    
    Node *new_node = malloc(sizeof(Node));

    new_node->data = data;

    Node *last_node = list_get_tail(head);
    last_node->next = new_node;
    new_node->next = NULL;
}


void add_at_node(Node** head, int key, int data)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;

    Node *temp = *head;

    // If head node itself holds the key
    if (temp != NULL && temp->data == key)
    {
        new_node->next = (*head)->next;
        (*head)->next = new_node;
        return;
    }

    while (temp != NULL && temp->data != key)
    {
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL) return;

    new_node->next = temp->next;
    temp->next = new_node;
}

void delete_first(Node **head)
{
    if (*head != NULL)
    {
        Node *temp = *head;
        *head = temp->next;
        free(temp);
    }
}

void delete_at_node(Node **head, int key)
{
    Node *temp = *head;
    Node *prev;

    //If head node itself holds the key to be deleted
    if (temp != NULL && temp->data == key)
    {
        *head = temp->next;
        free(temp);
        return;
    }

    // Search for the key to be deleted, keep track of the previous node as we need to change 'prev->next'
    while (temp != NULL && temp->data != key)
    {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL)
    {
        printf("Key not found!");
        return;
    }
    
    prev->next = temp->next;
    temp->next = NULL;
    free(temp);
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
        free(temp);
    }
    else // Head is last
    {
        temp = *head;
        *head = NULL;
        free(temp);
    }
}

void reverse_list(Node **head)
{
    Node *prev_head = NULL;
    Node *curr_head = *head;
    Node *temp;

   while(curr_head != NULL) 
   {
        temp = curr_head->next;
        curr_head->next = prev_head;
        prev_head = curr_head;
        curr_head = temp;
   }
   *head = prev_head;
}

void destroy(Node **head)
{
    while (head != NULL)
    {
        Node *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

int main(int argc, char **argv)
{
    add_first(&head, 10);
    add_first(&head, 20);
    add_last(&head, 50);
    add_first(&head, 30);
    add_first(&head, 40);
    print_linked_list(head);

    add_at_node(&head, 40, 17);
    print_linked_list(head);

    delete_first(&head);
    print_linked_list(head);

    delete_at_node(&head, 20);
    print_linked_list(head);

    delete_last(&head);
    print_linked_list(head);

    reverse_list(&head);
    print_linked_list(head);

    destroy(&head);

    return 0;
}