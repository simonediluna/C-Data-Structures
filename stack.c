#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node {
    char ch;
    struct node* prev;
} node_t;

void push(node_t** head, char ch) {
    node_t* new_node = malloc(sizeof(node_t));
    assert(new_node);
    new_node->ch = ch;
    new_node->prev = *head;
    *head = new_node;
}

char pop(node_t** head) {
    if (!(*head))
        return '\0';
    char popped = (*head)->ch;
    node_t* tmp = (*head)->prev;
    free(*head);
    *head = tmp;
    return popped;
}