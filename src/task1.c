#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

//=== TASK1 ===================================================================
//  Insert and sort "FLOCCINAUCINIHILIPILIFICATION" in Binary Search Tree
//=============================================================================
int main() {
    // Data
    char data[] = "FLOCCINAUCINIHILIPILIFICATION";
    printf("Unsorted String: %s\n", data);
    // Create BST
    Tree_Node *root = create_bst(data);
    // Print nodes
    printf("Sorted String: ");
    tree_print_sorted(root);
    // Newline
    printf("\n");
    // Delete all nodes
    tree_delete(root);
    return 0;
}