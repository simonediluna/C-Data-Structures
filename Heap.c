#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
// #include "Utilities.c"

typedef struct {
    int* arr;
    size_t arr_len;
    size_t heap_len; 
} heap_t;

typedef enum {
    INCREASING, DECREASING
} ORDER;

// Index of the left child
int _left(int i) {
    return 2*i + 1;
}

// Index of the right child
int _right(int i) {
    return 2*i + 2;
}

// Fix the node i. The max heap property must hold for the i's children.
void max_heapify(heap_t* heap, int i) {
    int left = _left(i);
    int right = _right(i); 
    int largest = i;
    if (left < heap->heap_len && heap->arr[largest] < heap->arr[left])
        largest = left;
    if (right < heap->heap_len && heap->arr[largest] < heap->arr[right])
        largest = right;
    if (largest != i) {
        int tmp = (heap->arr)[i];
        heap->arr[i] = heap->arr[largest];
        heap->arr[largest] = tmp;
        max_heapify(heap, largest);
    }
}

// Fix the node i. The min heap property must hold for the i's children.
void min_heapify(heap_t* heap, int i) {
    int left = _left(i);
    int right = _right(i); 
    int smallest = i;
    if (left < heap->heap_len && heap->arr[smallest] > heap->arr[left])
        smallest = left;
    if (right < heap->heap_len && heap->arr[smallest] > heap->arr[right])
        smallest = right;
    if (smallest != i) {
        int tmp = heap->arr[i];
        heap->arr[i] = heap->arr[smallest];
        heap->arr[smallest] = tmp;
        min_heapify(heap, smallest);
    }
}

// Build a max heap from an array
void max_heap(heap_t* heap) {
    int i = heap->arr_len / 2 - 1;
    for (i; i >= 0; i--) {
        max_heapify(heap, i);
    }
}

// Build a min heap from an array
void min_heap(heap_t* heap) {
    int i = heap->arr_len / 2 - 1;
    for (i; i >= 0; i--) {
        min_heapify(heap, i);
    }
}

// Check if the max heap property hold
bool is_max_heap(heap_t* heap) {
    for (int i = 0; i < heap->heap_len; i++) {
        if (heap->arr[i] < heap->arr[_left(i)] || 
            heap->arr[i] < heap->arr[_right(i)])
            return false;
    }
    return true;
}

// Check if the min heap property hold
bool is_min_heap(heap_t* heap) {
    for (int i = 0; i < heap->heap_len; i++) {
        if (heap->arr[i] > heap->arr[_left(i)] || 
            heap->arr[i] > heap->arr[_right(i)])
            return false;
    }
    return true;
}

/*
Pop the maximum from the max heap. The heap implementation in this file uses static arrays, 
so the size of the array remains the same after the extract_max/min functions are executed. 
This can be changed in the future by allowing the use of dynamic arrays and using the realloc 
function to change the size of the array. In this way, we can also remove the arr_len variable 
from the heap_t structure, leaving only heap_len.
*/
int extract_max(heap_t* heap) {
    if (!is_max_heap(heap))
        max_heap(heap);    
    if (heap->heap_len == 0) {
        printf("The size of the heap is 0.");
        exit(EXIT_FAILURE);
    }
    int max = heap->arr[0];
    heap->arr[0] = heap->arr[heap->heap_len - 1];
    heap->arr[heap->heap_len - 1] = max;
    heap->heap_len -= 1;
    max_heapify(heap, 0);
    return max;
}

// Pop the min from the min heap
int extract_min(heap_t* heap) {
    if (!is_min_heap(heap))
        min_heap(heap);    
    if (heap->heap_len == 0) {
        printf("The size of the heap is 0.");
        exit(EXIT_FAILURE);
    }
    int min = heap->arr[0];
    heap->arr[0] = heap->arr[heap->heap_len - 1];
    heap->arr[heap->heap_len - 1] = min;
    heap->heap_len -= 1;
    min_heapify(heap, 0);
    return min;
}

void heapsort(int* arr, size_t len, ORDER order) {
    heap_t heap = {arr, len, len};
    if (order == INCREASING) {
        max_heap(&heap);
        for (int i = 0; i < len; i++)
            extract_max(&heap);
    } else if (order == DECREASING) {
        min_heap(&heap);
        for (int i = 0; i < len; i++)
            extract_min(&heap);
    }
}

/*
int main() {
    int* arr = rand_arr(40, -100, 100);
    print_arr(arr, 40);
    heapsort(arr, 40, INCREASING);
    print_arr(arr, 40);
    heapsort(arr, 40, DECREASING);
    print_arr(arr, 40);

    heap_t heap = {arr, 40, 40};
    if (is_max_heap(&heap))
        puts("This is a max heap");
    else
        puts("This is not a max heap");

    if (is_min_heap(&heap))
        puts("This is a min heap");
    else
        puts("This is not a min heap");    
    printf("max = %d\tmin = %d\n\n", extract_max(&heap), extract_min(&heap));
}
*/