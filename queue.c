#include <stdio.h>
#include <stdlib.h>

typedef struct queue {
    char ch;
    struct queue* prev;
} queue_t;

void enqueue(queue_t** head, queue_t** tail, char ch);
void dequeue(queue_t** head, queue_t** tail);
void print_queue(queue_t* head);

int main(void) {
    queue_t *head = NULL, *tail = NULL;
    for (char i = 'a'; i < 'z'; i++)
        enqueue(&head, &tail, i);
    print_queue(head);
    for (int i = 0; i < 5; i++)
        dequeue(&head, &tail);
    print_queue(head);
    return 0;    
}

void enqueue(queue_t** head, queue_t** tail, char ch) {
    queue_t* new_node = malloc(sizeof(queue_t));
    new_node->ch = ch;
    new_node->prev = NULL;
    if (!(*head))
        *head = new_node;
    else
        (*tail)->prev = new_node;
    *tail = new_node;
    return;
}

void dequeue(queue_t** head, queue_t** tail) {
    if (!(*head))
        return;
    queue_t* tmp = *head;
    *head = (*head)->prev;
    free(tmp);
    if (!(*head))
        *tail = NULL;
}

void print_queue(queue_t* head) {
    printf("%s ", "Head ->");
    while (head) {
        printf("%c ", head->ch);
        head = head->prev;
    }
    printf("%s", "<- Tail");
    puts("");
}