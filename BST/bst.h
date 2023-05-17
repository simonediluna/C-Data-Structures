#ifndef BST_H
#define BST_H

#include <stdbool.h>

typedef struct treeNode_t {
    int key;
    struct treeNode_t* left;
    struct treeNode_t* right;
} treeNode_t;

typedef struct binary_search_tree {
    treeNode_t* root;
} BST_t;

BST_t* bst_create(void);
bool bst_is_empty(BST_t* tree);
treeNode_t* bst_create_node(int key);
static treeNode_t* bst_search(treeNode_t* root, treeNode_t** pprev, int key);
bool bst_is_key_in(BST_t* tree, int key);
void bst_insert(BST_t* tree, int key);
static treeNode_t* bst_minimum(treeNode_t* root, treeNode_t** pprev);
static void bst_transplant(treeNode_t* parent, treeNode_t* child, treeNode_t* grandchild);
void bst_delete(BST_t* tree, int key);
bool bst_is_valid(int* arr, int len);
static treeNode_t* bst_fill_tree(int idx, int* arr, int len);
BST_t* bst_create_tree_from_arr(int* arr, int len);
static void free_tree_subroutine(treeNode_t* root);
void bst_free_tree(BST_t* tree);
static void traverse_subroutine(treeNode_t* root);
void bst_traverse(BST_t* tree);

#endif