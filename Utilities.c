#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Create the 'Node' data type
typedef struct node {
    int value;
    struct node* next;
} Node;

// Function prototypes
int* rand_arr(size_t size, int min_val, int max_val);
Node* rand_list(size_t size, int min_val, int max_val);
void print_list(Node* headPtr);
void print_arr(int* arr, size_t size);
Node* last_node(Node* ptr);
Node* partition(Node* leftPtr, Node* rightPtr);
Node* quicksort(Node* leftPtr, Node* rightPtr);


// Function definitions

// Generate a random array
int* rand_arr(size_t size, int min_val, int max_val) {
    int* arr = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
        arr[i] = rand() % (max_val - min_val + 1) + min_val;
    return arr;
}

/*  The function generates a linked list of 'size' elements with random 
    values in the range ['min_val', 'max_val']  */
Node* rand_list(size_t size, int min_val, int max_val) {
    srand(time(NULL));
    Node* headPtr = NULL;

    if (!size)
        return headPtr;

    // Create the head of the list
    headPtr = malloc(sizeof(Node));
    if (!headPtr) {
        puts("Memory not allocated. The program will be terminated.");
        exit(EXIT_FAILURE);
    }
    headPtr->next = NULL;
    headPtr->value = rand() % (max_val - min_val + 1) + min_val;
    size--;

    // Create all the other nodes
    Node* currPtr = headPtr;
    while (size > 0) {
        Node* new_nodePtr = malloc(sizeof(Node));
        if (!new_nodePtr) {
            puts("Memory not allocated. The program will be terminated.");
            exit(EXIT_FAILURE);
        }
        new_nodePtr->next = NULL; 
        new_nodePtr->value = rand() % (max_val - min_val + 1) + min_val;
        currPtr->next = new_nodePtr;
        size--;
        currPtr = new_nodePtr;
    }

    return headPtr;
}

// Print all the values in the link list
void print_list(Node* headPtr) {
    while (headPtr) {
        printf("%d ", headPtr->value);
        headPtr = headPtr->next;
    }
    puts("");
}

void print_arr(int* arr, size_t size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    puts("");
}

// Quicksort subroutine to find the last element of the list
Node* last_node(Node* ptr) {
    if (!ptr) 
        return ptr;
    while (ptr->next)
        ptr = ptr->next;
    return ptr;
}

// Quicksort subroutine to sort one element
Node* partition(Node* leftPtr, Node* rightPtr) {
    Node* frontierPtr;  // 1st node of the subarray with values > pivot
    Node* predPtr;  // Frontier predecessor
    int pivot; 
    
    frontierPtr = leftPtr;
    predPtr = NULL;
    pivot = rightPtr->value;
    while (leftPtr != rightPtr) {
        // Swap the current node with the 1st node of the larger subarray
        if (leftPtr->value <= pivot) {
            int temp = frontierPtr->value;
            frontierPtr->value = leftPtr->value;
            leftPtr->value = temp;
            predPtr = frontierPtr;
            frontierPtr = frontierPtr->next;
        }
        leftPtr = leftPtr->next;
    }
    // Put the pivot in the correct position
    rightPtr->value = frontierPtr->value;
    frontierPtr->value = pivot;
    return predPtr;

}

Node* quicksort(Node* leftPtr, Node* rightPtr) {
    if (leftPtr != rightPtr && leftPtr && rightPtr) {
        Node* p = partition(leftPtr, rightPtr);
        quicksort(leftPtr, p);
        /*  Before to run quicksort on the right subarray we have to ensure
            that the left pointer is not NULL and that it is not beyond
            the right pointer.*/
        if (p && p != rightPtr && p->next != rightPtr) {
            quicksort(p->next->next, rightPtr);
        } else if (!p && leftPtr != rightPtr) {
            quicksort(leftPtr->next, rightPtr);
        }
    }
} 
