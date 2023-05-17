#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "red_black_tree.h"

/* Allocate memory for a binary search tree. Return the tree instance. */
rbt_t* rbt_create(void) {
    rbt_t* tree = malloc(sizeof(rbt_t));
    assert(tree);
    tree->root = NULL;
    return tree;
}

/* Return true if the tree does not contain any node, otherwise return false. */
bool rbt_is_empty(rbt_t* tree) {
    if (!tree->root)
        return true;
    return false;
}

/* Allocate memory for a new node and initialize 
its attributes. Eventually, return the new node. */
static rbt_node_t* rbt_create_node(int key) {
    rbt_node_t* new_node = malloc(sizeof(rbt_node_t));
    assert(new_node);

    new_node->key = key;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->p = NULL;
    new_node->color = RED;

    return new_node;
}

/* Rotate the subtree rooted in the input node to the left. 
The rotation is done around the link to its right child. */
static void left_rotate(rbt_t* tree, rbt_node_t* root) {
    if (!root->right)
        return;

    rbt_node_t* tmp = root->right;
	root->right = tmp->left;
	if (tmp->left)
		tmp->left->p = root;
	tmp->left = root;
    tmp->p = root->p;
	root->p = tmp;

    if (!tmp->p) {
        tree->root = tmp;
    } else if (tmp->p->left == root) {
        tmp->p->left = tmp;
    } else {
		tmp->p->right = tmp;
    }
}

/* Rotate the subtree rooted in the input node to the right. 
The rotation is done around the link to its left child. */
static void right_rotate(rbt_t* tree, rbt_node_t* root) {
    if (!root->left)
        return;

    rbt_node_t* tmp = root->left;
    root->left = root->left->right;
    if (root->left)
        root->left->p = root;
    tmp->right = root;
    tmp->p = root->p;
    root->p = tmp;

    if (!tmp->p) {
        tree->root = tmp;
    } else if (tmp->p->left == root) {
        tmp->p->left = tmp;
    } else {
        tmp->p->right = tmp;
    }
}

/* Insert a new node in the tree observing 
the BST property. Equal keys are allowed. */
static rbt_node_t* rbt_insert_subroutine(rbt_t* tree, int key) {
    rbt_node_t *new_node, *curr, *prev;
    new_node = rbt_create_node(key);
    curr = tree->root;
    prev = NULL;

    while (curr) {
        prev = curr;
        if (curr->key > key)
            curr = curr->left;
        else
            curr = curr->right;
    }

    if (!prev) {
        tree->root = new_node;
    } else {
        new_node->p = prev;
        if (prev->key > key)
            prev->left = new_node;
        else
            prev->right = new_node;
    }
    
    return new_node;
}

/* Insert subroutine. It preserves the red-black 
properties after a new key is inserted. */
static void insert_fixup(rbt_t* tree, rbt_node_t* ptr) {
	rbt_node_t *parent, *uncle, *grandpa;
    parent = ptr->p;
	while (parent && parent->color == RED) {
		grandpa = parent->p;
		if (parent == grandpa->left) {
			uncle = grandpa->right;
			if (uncle && uncle->color == RED) {
				parent->color = BLACK;
				uncle->color = BLACK;
				grandpa->color = RED;
				ptr = grandpa;
				parent = ptr->p;
			} else {
				if (ptr == parent->right) {
					ptr = parent;
					left_rotate(tree, ptr);
					parent = ptr->p;
					// grandpa = parent->p;	
				}
				parent->color = BLACK;
				grandpa->color = RED;
				right_rotate(tree, grandpa);
			}
		} else {
			uncle = grandpa->left;
			if (uncle && uncle->color == RED) {
				parent->color = BLACK;
				uncle->color = BLACK;
				grandpa->color = RED;
				ptr = grandpa;
				parent = ptr->p;
			} else {
				if (ptr == parent->left) {
					ptr = parent;
					right_rotate(tree, ptr);
					parent = ptr->p;
					// grandpa = parent->p;
				} 
                parent->color = BLACK;
                grandpa->color = RED;
                left_rotate(tree, grandpa);
			}
		}
	}
    if (tree->root->p)
        tree->root = (!parent) ? ptr : parent;
    tree->root->color = BLACK;
}

/* Insert a new key in the tree. */
void rbt_insert(rbt_t* tree, int key) {
    rbt_node_t* new_node = rbt_insert_subroutine(tree, key);
    insert_fixup(tree, new_node);
}

/* Search for the key is in the subtree rooted at the input node. If the key 
is present, it returns a pointer to that node, otherwise it returns NULL. */
static rbt_node_t* rbt_search(rbt_node_t* root, int key) {
    while (root && root->key != key)
        if (root->key > key)
            root = root->left;
        else
            root = root->right;
    return root;
}

/* Search for the minimum key in the subtree rooted at the 
input node and return a pointer to the node storing that key. */
static rbt_node_t* rbt_minimum(rbt_node_t* root) {
    while (root->left)
        root = root->left;
    return root;  
}

/* Link the child's parent to the child's child. */
static void rbt_transplant(rbt_t* tree, rbt_node_t* child, rbt_node_t* grandchild) {
    if (!child->p)
        tree->root = grandchild;
    else if (child->p->left == child) 
        child->p->left = grandchild;
    else
        child->p->right = grandchild;

    if (grandchild)
        grandchild->p = child->p;

    free(child);
}

/* Search for the key in the tree and, if present, remove it. */
static rbt_node_t* rbt_delete_subroutine(
    rbt_t* tree, int key, COLOR* removed_color, rbt_node_t** parent
) {
    rbt_node_t *child, *grandchild, *succ;
    rbt_node_t* to_be_fixed = NULL;  // child of the node we are about to delete.

    // Search for the key and assign the node to remove to the variable `child`.
    child = rbt_search(tree->root, key);

    if (!child) {
        printf("Key %d was not found.\n", key);
    } else if (child->left && child->right) {
        /* Overwrite the child's key with the key of its successor and
        then remove the successor. Successor cannot have left child. */
        succ = rbt_minimum(child->right);
        child->key = succ->key;
        to_be_fixed = succ->right;
        *removed_color = succ->color;
        *parent = succ->p;
        rbt_transplant(tree, succ, succ->right);
    } else { // The node to remove has at most one child.
        grandchild = (child->left) ? child->left : child->right;
        to_be_fixed = grandchild;
        *removed_color = child->color;
        *parent = child->p;
        rbt_transplant(tree, child, grandchild);
    }

    return to_be_fixed;  // Can be NULL
}

/* Restore the red-black tree properties when the removed node is BLACK. */
static void rbt_delete_fixup(
    rbt_t* tree, rbt_node_t* to_be_fixed, rbt_node_t* parent
) {
    rbt_node_t* sibling;  // to_be_fixed's sibling

    while (tree->root != to_be_fixed && 
          (!to_be_fixed || to_be_fixed->color == BLACK)) {
        
        // The node to be fixed is the left child of its parent.
        if (to_be_fixed == parent->left) {
            sibling = parent->right;  // Cannot be NULL

            // Case 1: sibling is RED
            if (sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                left_rotate(tree, parent);
                sibling = parent->right;
            }
            // Case 2: sibling is BLACK and both its children are BLACK
            if ((!sibling->left || sibling->left->color == BLACK) && 
                (!sibling->right || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    to_be_fixed = parent;
                    parent = to_be_fixed->p;

            // Cases 3 and 4
            } else { 
                // Case 3: sibling is BLACK, left child is RED, right child is BLACK
                if (!sibling->right || sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    right_rotate(tree, sibling);
                    sibling = parent->right;
                }
                // Case 4: sibling is BLACK, right child is RED
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->right->color = BLACK;
                left_rotate(tree, parent);
                to_be_fixed = tree->root;
            }

        } else { // The node to be fixed is the right child of its parent.
            sibling = parent->left; // Cannot be NULL

            // Case 1: sibling is RED
            if (sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                right_rotate(tree, parent);
                sibling = parent->left;
            }
            // Case 2: sibling is BLACK and both its children are BLACK
            if ((!sibling->left || sibling->left->color == BLACK) && 
                (!sibling->right || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    to_be_fixed = parent;
                    parent = to_be_fixed->p;

            // Cases 3 and 4
            } else {
                // Case 3: sibling is BLACK, left child is BLACK, right child is RED
                if (!sibling->left || sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    left_rotate(tree, sibling);
                    sibling = parent->left;
                }
                // Case 4: sibling is BLACK, left child is RED
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->left->color = BLACK;
                right_rotate(tree, parent);
                to_be_fixed = tree->root;
            }       
        }

    }

    if (to_be_fixed)
        to_be_fixed->color = BLACK;
}

void rbt_delete(rbt_t* tree, int key) {
    COLOR removed_color;  // Color of the node to delete.
    rbt_node_t* to_be_fixed;  // Child of the node to delete. Can be NULL.
    rbt_node_t* parent;  // Parent of the node to delete. Needed when to_be_fixed is NULL.

    to_be_fixed = rbt_delete_subroutine(tree, key, &removed_color, &parent);
    if (removed_color == BLACK) {
        rbt_delete_fixup(tree, to_be_fixed, parent);
    }
}

/* Preorder traverse. */
static void preorder_traverse(rbt_node_t* root) {
    if (!root)
         printf("%s ", "null");
    else {
        printf("%d(%c) ", root->key, (root->color == RED) ? 'R' : 'B');
        preorder_traverse(root->left);
        preorder_traverse(root->right);
    }
}

/* Inorder traverse. */
static void inorder_traverse(rbt_node_t* root) {
    if (!root)
         printf("%s ", "null");
    else {
        inorder_traverse(root->left);
        printf("%d(%c) ", root->key, (root->color == RED) ? 'R' : 'B');
        inorder_traverse(root->right);
    }
}

/* Postorder traverse. */
static void postorder_traverse(rbt_node_t* root) {
    if (!root)
         printf("%s(B) ", "null");
    else {
        postorder_traverse(root->left);
        postorder_traverse(root->right);
        printf("%d(%c) ", root->key, (root->color == RED) ? 'R' : 'B');
    }
}

/* Traverse the tree using a specific order: PREORDER, INORDER, POSTORDER. */
void rbt_traverse(rbt_t* tree, TRAVERSE_ORDER order) {
    if (!tree)
        puts("The tree does not exist.");
    else if (!tree->root)
        puts("The tree is empty.");
    else if (order == PREORDER)
        preorder_traverse(tree->root);
    else if (order == INORDER)
        inorder_traverse(tree->root);
    else
        postorder_traverse(tree->root);
}

static void free_tree_subroutine(rbt_node_t* root) {
    if (!root) return;
    free_tree_subroutine(root->left);
    free_tree_subroutine(root->right);
    free(root);
}

/* Free all nodes in the tree. */
void rbt_free_tree(rbt_t* tree) {
    if (!tree) {
        puts("The tree does not exist.");
    } else {
        free_tree_subroutine(tree->root);
        tree->root = NULL;
    }
}


/**
Auxiliary functions to build an arbitrary red-black tree. It is not possible to build
all valid red-black binary search trees by simply using insertion and deletion sequences.
*/

/* Subroutine of the function `rbt_is_valid`.
It checks 4 out of 5 red-black tree properties.*/
static int rbt_check_properties(
    unsigned int idx, int* keys, char* colors, int len
) {
    if (idx >= len || keys[idx] == INT_MIN)
        return 0;

    if (colors[idx] != 'R' && colors[idx] != 'B') {
        printf("%s\n\t %s\n", "Red-black properties violated:", 
            "Every node must be either red ('R') or black ('B').");
        exit(EXIT_FAILURE);  
    }

    unsigned int parent = (idx == 0) ? -1 * (idx - 1) / 2 : (idx - 1) / 2;
    if (colors[idx] == 'R' && colors[parent] == 'R') {
        printf("Two adjacent nodes cannot both be red. Check indexes %u and %u.\n", parent, idx);
        exit(EXIT_FAILURE);
    }

    int bh_left = rbt_check_properties(2*idx + 1, keys, colors, len);
    int bh_right = rbt_check_properties(2*idx + 2, keys, colors, len);
    if (bh_left != bh_right) {
        printf("%s\n\t%s %s\n", 
            "Red-black properties violated:", 
            "For each node, all the paths to descendant",
            "leaves must contain the same number of black nodes.");
        exit(EXIT_FAILURE);
    }

    return bh_left + (colors[idx] == 'B');  // Or right
}

/* Subroutine of the function `build_rbt_from_arr`. It checks
the black-root property and the validity of the arguments*/
static void rbt_is_valid(unsigned int len, int* keys, char* colors) {
    if (!keys || !colors) {
        puts("Invalid arguments");
        exit(EXIT_FAILURE);
    }
    if (colors[0] == 'R') {
        puts("Red-black properties violated:\n\t root cannot be red.");
        exit(EXIT_FAILURE);       
    }

    unsigned int n = strlen(colors);
    if (len != n) {
        printf("The two arrays have different lengths: %u vs %u.\n", len, n);
        exit(EXIT_FAILURE);
    }

    rbt_check_properties(0, keys, colors, len);
}

/* Subroutine of the function `build_rbt_from_arr`. It populates the RB tree. */
static rbt_node_t* rbt_fill_tree(
    unsigned int idx, int* keys, char* colors, unsigned int len
) {
    if (idx >= len || keys[idx] == INT_MIN)
        return NULL;

    rbt_node_t* new_node = malloc(sizeof *new_node);
    assert(new_node);
    
    new_node->key = keys[idx];
    new_node->color = (colors[idx] == 'R') ? RED : BLACK;

    new_node->left = rbt_fill_tree(2*idx + 1, keys, colors, len);
    if (new_node->left)
        new_node->left->p = new_node;

    new_node->right = rbt_fill_tree(2*idx + 2, keys, colors, len);
    if (new_node->right)
        new_node->right->p = new_node;    

    return new_node;
}

/* It allows to construct every possible valid instance of RBT starting 
from an array of keys (INT_MIN for null nodes) and an array of 
associated colors ('R' for red; 'B' for balck). */
rbt_t* build_rbt_from_arr(unsigned int len, int* keys, char* colors) {
    rbt_is_valid(len, keys, colors);

    rbt_t* tree = malloc(sizeof *tree);
    assert(tree);

    tree->root = rbt_fill_tree(0, keys, colors, len);
    tree->root->p = NULL;

    return tree;
}