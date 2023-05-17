#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#define SIZE 100

/*****************************************************************************
This program implements the hash table data structure using the double hashing
technique. The size of the table, |T|, depends on the cardinality of the input
set of keys, S. It is determined as follows. Assume that |T| = 2^x, for a 
positive integer x. Solve [2^x = 2.5*|S|] for x and round down the solution 
to the nearest integer. The result |T| = 2^log2(2.5*|S|) ensures that the size 
of the table is 1.25 to 2.5 times the size of the set of keys.                       
******************************************************************************/


typedef unsigned long long int uint64_t;
typedef long long int int64_t;

/* Global variables. To be substituted with local variables in main function
and passing a pointer to the other functions. By doing this we should be able
to use more than one table at once.*/
uint64_t tab_size;
uint64_t length = 0; 

/* Constants to mark each slot of the hash table. */
typedef enum {
    EMPTY, OCCUPIED, DELETED
} STATE;

/* Dictionary data type: (key, value) pair. The program does not support 
negative keys. ToDo: choose a MAX costant for the positive integers; a 
negative integer can then be mapped as -key + MAX. */
typedef struct dict {
    STATE state : 3; // Three bits variable
    uint64_t key;  // ToDo: use an Union to allow also char type
    int64_t val;
} dict_t;

/* Function prototypes */
dict_t* dict(uint64_t nkeys);
// uint64_t strToKey(char* s);
uint64_t hash(uint64_t key, uint64_t probe);
bool insert(dict_t* table, uint64_t key, int64_t value);
bool search(dict_t* table, uint64_t key);
int64_t get(dict_t* table, uint64_t key);
uint64_t delete(dict_t* table, uint64_t key);
uint64_t len();
uint64_t table_size();
void print_table(dict_t* table) ;
// int cmp(const void *p, const void *q);


/*****************************************************************************
                        Test the implementation                                 
******************************************************************************/
/*
int main() {
    uint64_t i;
    uint64_t slot;

    // Create the hash table
    dict_t* dct = dict(SIZE);
    puts("State values after the initialization of the table");
    for (i = 0; i < SIZE; i++)
        printf("%d ", dct[i].state);
    puts("\n");

    // Populate the table
    int arr[SIZE];  // Auxiliary array to test the search function
    for (i = 0; i < SIZE; i++) {
        arr[i] = rand() % 1000; 
        insert(dct, arr[i], i);  
    }

    // Check if all keys have been entered.
    for (i = 0; i < SIZE; i++) {
        slot = search(dct, arr[i]);
        if (slot == -1) 
            exit(EXIT_FAILURE);
        printf("%-3d found:  slot = %-3zu\n", arr[i], slot);
    } 
    puts("");

    puts("Print the whole table:");
    print_table(dct);
    puts("");

    // Delete from the table all the keys in the odd indexes of the array
    table_size();
    len();
    puts("");
    for (i = 0; i < SIZE; i++) {
        if (i % 2)
            delete(dct, arr[i]);  
    }
    print_table(dct);
    len();

    free(dct);
}
*/

/*****************************************************************************
                         Function definitions
******************************************************************************/

/* Create the hash table. */
dict_t* dict(uint64_t nkeys) {
    if (nkeys > (long double) ULLONG_MAX/2.5) {
        puts("Integer overflow");
        exit(EXIT_FAILURE);
    }
    tab_size = pow(2, floor(log2(2.5 * nkeys)));
    dict_t* table = malloc(tab_size * sizeof(dict_t));
    if (!table) {
        puts("Memory not allocated");
        exit(EXIT_FAILURE);
    }
    // Initialize table
    for (uint64_t i = 0; i < tab_size; i++)
        table[i].state = EMPTY;
    return table;
}

/* 
To be Added. It computes a key from a string. 
The uint64_t can range from 0 to 18.446.744.073.709.551.615. 
Before to use radix-128 make sure of a string maximum length to avoid overflow.

uint64_t strToKey(char* s) {
    int n = strlen(s);  // Remove and use while loop from left to right
    uint64_t key = 0;
    for (int i = n-1; i >= 0; i--) 
        key += pow(26, n-i-1) * (s[i] - 'a' + 1);  // To be changed to allow all ASCII vals (radix-128)
    return key;
}
*/ 


/* Hash function: Double hashing. */
uint64_t hash(uint64_t key, uint64_t probe) {
    uint64_t h1 = key % tab_size;  // min=0, max=|T|-1
    uint64_t h2 = 1 + 2*(key % (tab_size/2));  // odd nums, min=1, max=|T|-1
    return (h1 + probe*h2) % tab_size; 
}

/* Insert a (key, value) pair in the table. If successful,
it returns the slot index, otherwise it returns -1. */
bool insert(dict_t* table, uint64_t key, int64_t value) {
    uint64_t slot, probe = 0;
    do {
        slot = hash(key, probe);
        if (table[slot].state == OCCUPIED && table[slot].key == key) {
            table[slot].val = value;  // Overwrite the value
            return true;
        } else if (table[slot].state != OCCUPIED) {
            table[slot].key = key;
            table[slot].val = value;
            table[slot].state = OCCUPIED;
            length++;
            return true;
        } 
        probe++;
    } while (probe != tab_size);
    puts("Hash table overflow");
    return false;
}

/* Search the table for the key. If successful, 
it returns the true, otherwise returns false. */
bool search(dict_t* table, uint64_t key) {
    uint64_t slot, probe = 0;
    do {
        slot = hash(key, probe);
        if (table[slot].state == EMPTY)
            break;
        else if (table[slot].state == OCCUPIED && table[slot].key == key) 
            return true;
        probe++;
    } while (probe != tab_size);
    printf("Key %llu not found\n", key);
    return false;
}

/* Search the table for key. If present, it returns its value, 
otherwise it returns LLONG_MIN */
int64_t get(dict_t* table, uint64_t key) {
    uint64_t slot, probe = 0;
    do {
        slot = hash(key, probe);
        if (table[slot].state == EMPTY)
            break;
        else if (table[slot].state == OCCUPIED && table[slot].key == key) 
            return table[slot].val;
        probe++;
    } while (probe != tab_size);
    printf("Key %llu not found\n", key);
    return LLONG_MIN;    
};

/* Insert a (key, value) pair in the table. If successful,
it returns the slot index, otherwise it returns -1. */
uint64_t delete(dict_t* table, uint64_t key) {
    uint64_t slot = search(table, key);
    if (slot != -1) {
        table[slot].state = DELETED;
        length--;
        printf("Key %llu deleted \n", key);
    }
    return slot;
}

/* Print and return the number of occupied slots. */
uint64_t len() {
    printf("The length is: %llu\n", length);
    return length;
}

/* Print and return the cardinality of the table. */
uint64_t table_size() {
    printf("The size of the table is: %llu\n", tab_size);
    return tab_size;
}

/* Print the (key, value) pairs of all occupied slots. */
void print_table(dict_t* table) {
    for (uint64_t i = 0; i < tab_size; i++) {
        if (table[i].state == OCCUPIED)
            printf("Table[%llu] = %llu: %lld\n", i, table[i].key, table[i].val);
    }
}

/* 
Function to use in qsort to sort in decreasing order according to keys.
If keys are equal, it sorts in lexicographical order.

int cmp(const void *p, const void *q) {
    dict_t a = *((dict_t*) p);
    dict_t b = *((dict_t*) q);
    if (a.val < b.val)
        return 1;
    else if (a.val > b.val)
        return -1;
    else
        return strcmp((const char*) a.str, (const char*) b.str);
}
*/