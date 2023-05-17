#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "red_black_tree.h"

#define SIZE 8
#define SIZE1 15

void test_function(char* func) {
    unsigned int pad;
    char str[80] = {'\0'};
    sprintf(str, "Test `%s`.", func);
    pad = 40 - strlen(str)/2;
    for (int i = 0; i < 80; i++) printf("%s", "=");
    printf("\n%*s%s\n", pad, "", str);
    for (int i = 0; i < 80; i++) printf("%s", "=");
    puts("");
}

void print_arrays(int keys[], char* colors, int len) {
    puts("Build a RB tree from the following array:");
    for (int i = 0; i < len; i++) 
        (keys[i] != INT_MIN) ? printf("%d(%c) ", keys[i], colors[i]) : printf("%s ", "null");
    puts("");
}

int main() {
    int arr[SIZE] = {11, 2, 14, 1, 7, 15, 5, 8};
    rbt_t* tree = rbt_create();

    /* Test insertion */
    test_function("Insertion");

    puts("Create an RBT and perform a preorder traverse:");
    rbt_traverse(tree, PREORDER);
    puts("");

    for (int i = 0; i < SIZE; i++) {
        printf("Insert key %d\n", arr[i]);
        rbt_insert(tree, arr[i]);
    }
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");

    printf("Insert key %d\n", 4);
    rbt_insert(tree, 4);
    printf("Insert again key %d\n", 8);
    rbt_insert(tree, 8);  // Duplicate
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");

    /* Test Deletition */
    test_function("Deletition");

    printf("Delete key %d\n", 8);
    rbt_delete(tree, 8);  // Instance of case 0: Delete a black node with only one red child
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");

    printf("Delete key %d\n", 1);
    rbt_delete(tree, 1);  // Instance of case 3 and 4: Delete red node with two children and red successor
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");

    printf("Delete key %d\n", 8);
    rbt_delete(tree, 8);  // Instance of case 4: Delete black node with black right sibiling
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");

    printf("Delete key %d\n", 2);
    rbt_delete(tree, 2);  // Instance of case 2: Delete black node with black right sibiling
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");

    printf("Delete key %d\n", 5);
    rbt_delete(tree, 5);  // Instance of case 0: Delete red node
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");

    printf("Delete key %d\n", 4);
    rbt_delete(tree, 4);  // Instance of case 1: Delete black node with red right sibiling
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");

    printf("Delete key %d\n", 14);
    rbt_delete(tree, 14);  // Instance of case 3 and 4: Delete black root with black left sibiling
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");   

    printf("Delete key %d\n", 11);
    rbt_delete(tree, 11);  // Instance of case 3 and 4: Delete black root with black left sibiling
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");

    printf("Delete key %d\n", 15);
    rbt_delete(tree, 15);  // Delete black root 
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");

    printf("Delete key %d\n", 7);
    rbt_delete(tree, 7);  // Delete black root 
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");       

    rbt_free_tree(tree);
    free(tree);
    tree = NULL;

    /* Test build_rbt_from_arr */
    test_function("Build RBT from array"); 
    int keys[SIZE1] = {10, 5, 15, -5, 7, 13, 20, -10, -3, 6, 8, 11, 16, 18, 25};
    char colors1[SIZE1] = {'B', 'B', 'R', 'B', 'B', 'B', 'B', 'R', 'R', 'R', 'R', 'B', 'B', 'B', 'B'}; 

    print_arrays(keys, colors1, SIZE1);
    tree = build_rbt_from_arr(SIZE1, keys, colors1);
    puts("\nPreorder traverse of the tree:");
    rbt_traverse(tree, PREORDER);
    puts("\n");

    rbt_free_tree(tree);
    free(tree);
    tree = NULL;

    // RED ROOT
    // char colors2[SIZE1] = {'R', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'};
    // print_arrays(keys, colors2, SIZE1);
    // tree = build_rbt_from_arr(SIZE1, keys, colors2);

    // DIFFERENT BH
    // char colors3[SIZE1] = {'B', 'R', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'};
    // print_arrays(keys, colors3, SIZE1);
    // tree = build_rbt_from_arr(SIZE1, keys, colors3);  

    // ADJACENT RED NODES
    char colors3[SIZE1] = {'B', 'R', 'R', 'R', 'B', 'R', 'R', 'B', 'B', 'R', 'R', 'B', 'B', 'B', 'B'};
    print_arrays(keys, colors3, SIZE1);
    tree = build_rbt_from_arr(SIZE1, keys, colors3);

    rbt_free_tree(tree);
    free(tree);
    tree = NULL;          

    return 0;
}