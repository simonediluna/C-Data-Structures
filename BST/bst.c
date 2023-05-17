/* 
* This is an implementation the binary search tree data structure. For the sake
* of flexibility, it uses one structure to define the characteristics of the
* tree nodes and another structure to define the BST. This makes it possible
* to have multiple instances of the BST type at the same time and to avoid the
* need to use a double pointer paramenter for the insert and delete functions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>
#include "bst.h"

/* Allocate memory for a binary search tree. Return the tree instance. */
BST_t* bst_create(void) {
    BST_t* tree = malloc(sizeof(BST_t));
    assert(tree);
    tree->root = NULL;
    return tree;
}

/* Return true if the tree does not contain any node, otherwise return false. */
bool bst_is_empty(BST_t* tree) {
    if (!tree->root)
        return true;
    return false;
}

/* Allocate memory for a new node and initialize 
its attributes. Eventually, return the new node. */
treeNode_t* bst_create_node(int key) {
    treeNode_t* new_node = malloc(sizeof(treeNode_t));
    assert(new_node);

    new_node->key = key;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

/* If the seached key is in the tree, it returns a pointer to that node, 
otherwise it returns NULL. The 2nd argument is an output parameter,
if different from NULL, it stores a pointer to the parent of the found node. */
static treeNode_t* bst_search(treeNode_t* root, treeNode_t** pprev, int key) {
    treeNode_t* prev = NULL;

    while (root && root->key != key) {
        prev = root;
        if (root->key > key)
            root = root->left;
        else
            root = root->right;
    }
    if (pprev)
        *pprev = prev;

    return root;
}

/* Search for the key in the subtree rooted at 1st argument. 
Return true if the key is found, else false. */
bool bst_is_key_in(BST_t* tree, int key) {
    if (bst_search(tree->root, NULL, key))
        return true;
    return false;
}

/* Insert a new node in the tree. Equal keys are allowed. */
void bst_insert(BST_t* tree, int key) {
    treeNode_t *new_node, *curr, *prev;
    new_node = bst_create_node(key);
    curr = tree->root;
    prev = NULL;

    while (curr) {
        prev = curr;
        if (curr->key > key)
            curr = curr->left;
        else if (curr->key < key)
            curr = curr->right;
        // Randomly choose left or right in case of equal keys
        else if (rand() % 2)
            curr = curr->left;
        else
            curr = curr->right;
    }

    if (!prev)
        tree->root = new_node;
    else if (prev->key > key)
        prev->left = new_node;
    else 
        prev->right = new_node;
}

/* Search for the min key in the subtree rooted at the 1st argument and return 
a pointer to the node storing that key. The 2nd argument is an output parameter,
if different from NULL, it stores a pointer to the parent of the minimum. */
static treeNode_t* bst_minimum(treeNode_t* root, treeNode_t** pprev) {
    treeNode_t* parent = (pprev) ? *pprev : NULL;

    while (root->left) {
        parent = root;
        root = root->left;
    }
    if (pprev)
        *pprev = parent;

    return root;  
}

/* Link parent to grandchild and free child. */
static void bst_transplant(treeNode_t* parent, treeNode_t* child, treeNode_t* grandchild) {
    if (parent->left == child) 
        parent->left = grandchild;
    else
        parent->right = grandchild;

    free(child);
}

/* Search for the key in the tree and, if present, remove it. */
void bst_delete(BST_t* tree, int key) {
    treeNode_t *parent, *child, *grandchild, *succ, *succ_parent;

    // Search the key and assign the node to remove to the variable `child`.
    child = bst_search(tree->root, &parent, key);

    if (!child) {
        printf("Key %d was not found.\n", key);
    } else if (child->left && child->right) {
        /* Overwrite the child's key with the key of its successor and
        then remove the successor. Successor cannot have left child. */
        succ_parent = child;
        succ = bst_minimum(child->right, &succ_parent);
        child->key = succ->key;
        bst_transplant(succ_parent, succ, succ->right); 
    } else {
        grandchild = (child->left) ? child->left : child->right;
        if (!parent) {  
            // The node to remove is the root and it has at most one child
            free(child);
            tree->root = grandchild;
        } else {  
            // The node to remove is either a leaf or it has at most one child
            bst_transplant(parent, child, grandchild);
        }
    }

}

/* Check if the input array observes the BST property. */
bool bst_is_valid(int* arr, int len) {
    for (int i = 0; i < len; i++) {
        int left = 2*i + 1, right = 2*i + 2;
        if ((left < len && arr[left] > arr[i]) || 
            (right < len && arr[right] < arr[i] && arr[right] != INT_MIN))
            return false;
    }
    return true;
}

/* Subroutine of binary_tree_from_arr. */
static treeNode_t* bst_fill_tree(int idx, int* arr, int len) {
    if (idx >= len || arr[idx] == INT_MIN)
        return NULL;

    treeNode_t* new_node = malloc(sizeof(treeNode_t));
    assert(new_node);
    
    new_node->key = arr[idx];
    new_node->left = bst_fill_tree(2*idx + 1, arr, len);
    new_node->right = bst_fill_tree(2*idx + 2, arr, len);

    return new_node;
}

/* Given a valid array of int return the relative BST, else exit the program. */
BST_t* bst_create_tree_from_arr(int* arr, int len) {
    if (!arr || len < 1)
        return NULL;

    if (!bst_is_valid(arr, len)) {
        puts("The tree is not a valid BST.");
        return NULL;
    }

    BST_t* tree = bst_create();
    tree->root = bst_fill_tree(0, arr, len);
    return tree;
}

static void free_tree_subroutine(treeNode_t* root) {
    if (!root) return;
    free_tree_subroutine(root->left);
    free_tree_subroutine(root->right);
    free(root);
}

/* Free all nodes in the tree. */
void bst_free_tree(BST_t* tree) {
    if (!tree) {
        puts("The tree does not exist.");
    } else {
        free_tree_subroutine(tree->root);
        tree->root = NULL;
    }
}

static void traverse_subroutine(treeNode_t* root) {
    if (!root) return;
    traverse_subroutine(root->left);
    printf("%d ", root->key);
    traverse_subroutine(root->right);
}

/* Inorder traverse. */
void bst_traverse(BST_t* tree) {
    if (!tree)
        puts("The tree does not exist.");
    else if (!tree->root)
        puts("The tree is empty.");
    else
        traverse_subroutine(tree->root);
}