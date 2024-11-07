#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"  // Ensure this file includes your BST structure and function prototypes

// Function to display menu
void display_menu() {
    printf("\n--- Binary Search Tree Tester ---\n");
    printf("1. Create BST from a string\n");
    printf("2. Insert a character into BST\n");
    printf("3. Search for a character in BST\n");
    printf("4. Print BST in sorted order\n");
    printf("5. Delete entire BST\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

// Main function to test BST
int main() {
    Tree_Node* root = NULL;
    int choice;
    char data[1000];  // Buffer to store input string or character

    while (1) {
        display_menu();
        scanf("%d", &choice);
        getchar();  // Clear newline from buffer

        switch (choice) {
            case 1:  // Create BST from a string
                printf("Enter a string to create BST: ");
                fgets(data, sizeof(data), stdin);
                data[strcspn(data, "\n")] = '\0';  // Remove trailing newline

                if (root != NULL) {
                    printf("Deleting existing tree...\n");
                    tree_delete(root);
                    root = NULL;
                }
                root = create_bst(data);
                printf("BST created successfully from input string.\n");
                break;

            case 2:  // Insert a character
                printf("Enter a character to insert: ");
                scanf(" %c", &data[0]);
                getchar();  // Clear newline from buffer
                tree_insert(&root, data[0]);
                printf("Character '%c' inserted.\n", data[0]);
                break;

            case 3:  // Search for a character
                printf("Enter a character to search: ");
                scanf(" %c", &data[0]);
                getchar();  // Clear newline from buffer
                Tree_Node* result = tree_search(root, data[0]);
                if (result != NULL) {
                    printf("Character '%c' found in the BST.\n", data[0]);
                } else {
                    printf("Character '%c' not found in the BST.\n", data[0]);
                }
                break;

            case 4:  // Print BST in sorted order
                printf("BST in sorted order: ");
                tree_print_sorted(root);
                printf("\n");
                break;

            case 5:  // Delete entire BST
                tree_delete(root);
                root = NULL;
                printf("BST deleted.\n");
                break;

            case 6:  // Exit
                printf("Exiting...\n");
                tree_delete(root);
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}
