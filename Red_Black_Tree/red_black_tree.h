#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stdbool.h>

typedef enum {
    RED, BLACK
} COLOR;

typedef enum {
    PREORDER, INORDER, POSTORDER
} TRAVERSE_ORDER;

typedef struct rbt_node_t {
	int key;
    COLOR color: 1;  // Bit field
	struct rbt_node_t *p, *left, *right;
} rbt_node_t;

typedef struct red_black_tree {
    rbt_node_t* root;
} rbt_t;

rbt_t* rbt_create(void);
bool rbt_is_empty(rbt_t* tree);
static rbt_node_t* rbt_create_node(int key);
static void left_rotate(rbt_t* tree, rbt_node_t* root);
static void right_rotate(rbt_t* tree, rbt_node_t* root);

static rbt_node_t* rbt_insert_subroutine(rbt_t* tree, int key);
static void insert_fixup(rbt_t* tree, rbt_node_t* ptr);
void rbt_insert(rbt_t* tree, int key);

static rbt_node_t* rbt_search(rbt_node_t* root, int key);
static rbt_node_t* rbt_minimum(rbt_node_t* root);

static void rbt_transplant(rbt_t* tree, rbt_node_t* child, rbt_node_t* grandchild);
static rbt_node_t* rbt_delete_subroutine(rbt_t* tree, int key, COLOR* removed_color, rbt_node_t** parent);
static void rbt_delete_fixup(rbt_t* tree, rbt_node_t* to_be_fixed, rbt_node_t* parent);
void rbt_delete(rbt_t* tree, int key);

static void preorder_traverse(rbt_node_t* root);
static void inorder_traverse(rbt_node_t* root);
static void postorder_traverse(rbt_node_t* root);
void rbt_traverse(rbt_t* tree, TRAVERSE_ORDER order);

static void free_tree_subroutine(rbt_node_t* root);
void rbt_free_tree(rbt_t* tree);

/* Create RB tree from an array of keys and another of colors. */
static int rbt_check_properties(unsigned int idx, int* keys, char* colors, int len);
static void rbt_is_valid(unsigned int len, int* keys, char* colors);
static rbt_node_t* rbt_fill_tree(unsigned int idx, int* keys, char* colors, unsigned int len);
rbt_t* build_rbt_from_arr(unsigned int len, int* keys, char* colors);

#endif