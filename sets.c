#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    NIL, DELETED, VALID
} state;

typedef struct {
    state state : 2; // Two bits variable
    int value;
} number;

typedef struct {
    int* table;
} set_t;


/* 
Personal implementation used on leetcode to solve the following problem: 
https://leetcode.com/problems/contains-duplicate/description/

#define EMPTY 0
#define OCCUPIED 1
#define MAX 1000000000

typedef struct {
    int state, key;
} dict_t;

int hash(int key, int probe, int tab_size) {
    int h1 = key % tab_size;
    int h2 = 1 + 2*(key % tab_size/2);
    return (h1 + h2*probe) % tab_size; 
}

bool insert(dict_t* tab, int key, int tab_size) { // insert and search at the same time
    if (key < 0)
        key = -key + MAX;    
    int slot, probe = 0;
    do {
        slot = hash(key, probe, tab_size);
        if (tab[slot].state == EMPTY) {
            tab[slot].state = OCCUPIED;
            tab[slot].key = key;
            return false;
        } else if (tab[slot].key == key) {
            return true;
        } else
            probe++;  
    } while (probe < tab_size);
    return false;
}

bool containsDuplicate(int* nums, int numsSize){
    int dict_size = 2*numsSize;
    dict_t* dct = calloc(dict_size, sizeof(dict_t));
    for (int i = 0; i < numsSize; i++) {
        if (insert(dct, nums[i], dict_size))
            return true;
    }
    return false;
}
*/