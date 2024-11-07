#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

//=== FUNCTION ================================================================
//         Name: tree_insert
//  Description: Insert new node onto tree, if first node initialize tree
//=============================================================================
void tree_insert(Tree_Node** root, char data){
    if (*root == NULL) { // AKA First In Tree
        *root = malloc(sizeof(struct Tree_Node));
        root[0]->data = data;
        root[0]->left = NULL;
        root[0]->right = NULL;
    } else if (root[0]->data >= data) {
        // If root[0]->data >= data (allow duplicates on left)
        tree_insert(&root[0]->left, data);
    } else if (root[0]->data < data) {
        // If root[0]->data < data, insert on right
        tree_insert(&root[0]->right, data);
    }
}

//=== FUNCTION ================================================================
//         Name: create_bst
//  Description: Pass a string of characters to be inserted into a tree
//=============================================================================
Tree_Node* create_bst (char data[]){
    // Create NULL pointer for new tree
    Tree_Node* root = NULL;
    // For each data point, insert into tree
    for (int i = 0; data[i] != '\0'; i++) {
        tree_insert(&root, data[i]);
    }
    // Return root node
    return root;
}

//=== FUNCTION ================================================================
//         Name: tree_search
//  Description: Search the tree for a specific value and return the node
//=============================================================================
Tree_Node* tree_search(Tree_Node* root, char data){
    // Ensure valud pointer
    if (root != NULL) {
        // Check root node and if matches return
        if (root->data == data) {
            return root;
        }
        // If root->data > data then recursively check all left nodes
        // otherwise recursively check all right nodes
        if (root->data > data) {
            return tree_search(root->left, data);
        } else if (root->data < data) {
            return tree_search(root->right, data);
        }
    }
    return NULL;
}

//=== FUNCTION ================================================================
//         Name: tree_print_sorted
//  Description: Prints the tree in sorted order until reaches NULL pointer
//=============================================================================
void tree_print_sorted(Tree_Node* root){
    // For recursion, stop when NULL
    if (root != NULL) {
        // Recursively print all left nodes
        tree_print_sorted(root->left);
        // Print root node
        printf("%c", root->data);
        // Recursively print all right nodes
        tree_print_sorted(root->right);
    }
}

//=== FUNCTION ================================================================
//         Name: tree_delete
//  Description: Deletes each node in the tree recursively until NULL poiunter
//=============================================================================
void tree_delete(Tree_Node* root){
    // For recursion, stop when NULL
    if (root != NULL) {
        // Recursively delete all left nodes
        tree_delete(root->left);
        // Recursively delete all right nodes
        tree_delete(root->right);
        // Delete root node
        free(root);
    }
}
