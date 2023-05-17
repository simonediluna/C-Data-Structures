#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "bst.h"

#define ARR1_SIZE 5
#define ARR2_SIZE 11

void test_function(char* func) {
    unsigned int pad;
    char str[80] = {'\0'};
    sprintf(str, "Test the `%s` function.", func);
    pad = 40 - strlen(str)/2;
    for (int i = 0; i < 80; i++) printf("%s", "=");
    printf("\n%*s%s\n", pad, "", str);
    for (int i = 0; i < 80; i++) printf("%s", "=");
    puts("");
}

void print_array(int arr[], int len) {
    for (int i = 0; i < len; i++) 
        (arr[i] != INT_MIN) ? printf("%d ", arr[i]) : printf("%s ", "null");
    puts("");
}

int main() {
    BST_t* tree;
    int arr1[ARR1_SIZE] = {1,2,3,4,5};
    int arr2[ARR2_SIZE] = {10, 5, 20, 0, 7, INT_MIN, 30, INT_MIN, INT_MIN, 6, 8};

    test_function("bst_create_tree_from_arr");

    printf("%s\n\t", "Build a BST from the following array:");
    print_array(arr1, ARR1_SIZE);
    tree = bst_create_tree_from_arr(arr1, ARR1_SIZE);
    printf("%s", "Traverse the tree:\n\t");
    bst_traverse(tree);
    puts("");

    printf("%s\n\t", "Build a BST from the following array:");
    print_array(arr2, ARR2_SIZE);
    tree = bst_create_tree_from_arr(arr2, ARR2_SIZE);
    printf("%s", "Traverse the tree:\n\t");
    bst_traverse(tree);
    puts("\n");

    test_function("bst_free_tree");
    bst_free_tree(tree);
    printf("%s", "Traverse the tree:\n\t");
    bst_traverse(tree);
    puts("\n");

    test_function("bst_insert");
    /* The memory for the BST is already allocated due to the 
    `bst_create_tree_from_arr` function. */
    for (int i = 0; i < ARR2_SIZE; i++) {
        if (arr2[i] != INT_MIN)
            bst_insert(tree, arr2[i]);
    }
    printf("%s", "Traverse the tree:\n\t");
    bst_traverse(tree);
    puts("\n");

    test_function("bst_delete");
    printf("%s\n\t", "Delete key 87:");
    bst_delete(tree, 87);
    for (int i = 0; i < ARR2_SIZE; i++) {
        if (arr2[i] != INT_MIN) {
            printf("Delete the key %d:\n\t", arr2[i]);
            bst_delete(tree, arr2[i]);
            printf("%s", "Traverse the tree:\n\t");
            bst_traverse(tree);
            puts("");
        }
    }

    free(tree);
}