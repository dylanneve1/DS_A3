#include "bstdb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DOCS 1000000
#define LEFT_DIR 101
#define RIGHT_DIR 102

// Write your submission in this file
//
// A main function and some profiling tools have already been set up to test
// your code in the task2.c file. All you need to do is fill out this file
// with an appropriate Binary Search Tree implementation.
//
// The input data will be books. A book is comprised of a title and a word
// count. You should store both these values in the tree along with a unique
// integer ID which you will generate.
//
// We are aiming for speed here. A BST based database should be orders of
// magnitude faster than a linked list implementation if the BST is written
// correctly.
//
// We have provided an example implementation of what a linked list based
// solution to this problem might look like in the db/listdb.c file. If you
// are struggling to understand the problem or what one of the functions
// below ought to do, consider looking at that file to see if it helps your
// understanding.
//
// There are 6 functions you need to look at. Each is provided with a comment
// which explains how it should behave. The functions are:
//
//  + bstdb_init
//  + bstdb_add
//  + bstdb_get_word_count
//  + bstdb_get_name
//  + bstdb_stat
//  + bstdb_quit
//
// Do not rename these functions or change their arguments/return types.
// Otherwise the profiler will not be able to find them. If you think you
// need more functionality than what is provided by these 6 functions, you
// may write additional functions in this file.

typedef struct TNode {
	int doc_id;
    char *name;
    char *author;
    int word_count;
    struct TNode *left;
    struct TNode *right;
} TNode ;

TNode *root;

int IDtable[MAX_DOCS] = { 0 };

int bst_num_traverses, bst_num_searches, num_duplicates, leftHeight, rightHeight;

void getBalance() {
	
}

void
free_TNode (TNode *node) {
	// This function frees all TNode and strings inside recursively
    if (node == NULL) {
    	return;
    }
    free_TNode(node->left);
    free_TNode(node->right);
    free(node->name);
    free(node->author);
    IDtable[node->doc_id] = 0;
    free(node);
}

int
bstdb_init ( void ) {
	// This function will run once (and only once) when the database first
	// starts. Use it to allocate any memory you want to use or initialize 
	// some globals if you need to. Function should return 1 if initialization
	// was successful and 0 if something went wrong.
	bst_num_traverses = 0;
	bst_num_searches = 0;
	num_duplicates = 0;
	leftHeight = 0;
	rightHeight = 0;
	root = NULL;
	return 1;
}

int
bstdb_add ( char *name, int word_count, char *author ) {
	// This function should create a new node in the binary search tree, 
	// populate it with the name, word_count and author of the arguments and store
	// the result in the tree.
	//
	// This function should also generate and return an identifier that is
	// unique to this document. A user can find the stored data by passing
	// this ID to one of the two search functions below.
	//
	// How you generate this ID is up to you, but it must be an integer. Note
	// that this ID should also form the keys of the nodes in your BST, so
	// try to generate them in a way that will result in a balanced tree.
	//
	// If something goes wrong and the data cannot be stored, this function
	// should return -1. Otherwise it should return the ID of the new node
	// Keep track of current and parent

	int depth = 0;
	int sway = 0;

	int doc_id = rand() % MAX_DOCS;
	while (IDtable[doc_id] == 1) {
		doc_id = rand() % MAX_DOCS;
	}
	IDtable[doc_id] = 1;

	// Current and parent pointers
	TNode *current = root;
	TNode *parent = NULL;

	// Find the insertion point
	while (current != NULL) {
		parent = current;
		if (current->doc_id > doc_id) {
			current = current->left;
			depth++;
			if (sway == 0) {
				sway = LEFT_DIR;
			}
		} else if (current->doc_id < doc_id) {
			current = current->right;
			depth++;
			if (sway == 0) {
				sway = RIGHT_DIR;
			}
		} else if (current->doc_id == doc_id) {
			// Duplicate found, not good!
			num_duplicates++;
			return -1;
		}
	}

	TNode *addition = malloc(sizeof(TNode));
    if (!addition) {       
        return -1;
    }
    addition->doc_id = doc_id;
    addition->word_count = word_count;
    addition->left = NULL;
    addition->right = NULL;

    if (name != NULL) {
    	size_t len_name = strlen(name)+1;
    	addition->name = calloc(len_name, sizeof(char));
    	if (addition->name) {
        	// if calloc was successful, copy the filename into the node
        	strcpy( addition->name, name );
    	} else {
        	// if calloc failed, release any memory that was allocated and 
        	// report an error by returning NULL
        	free_TNode(addition);
        	addition = NULL;
    	}
	}

    if (author != NULL) {
    	size_t len_author = strlen(author)+1;
    	addition->author = calloc(len_author, sizeof(char));
    	if (addition->author) {
        	// if calloc was successful, copy the filename into the node
        	strcpy( addition->author, author );
    	} else {
        	// if calloc failed, release any memory that was allocated and 
        	// report an error by returning NULL
        	free_TNode(addition);
        	addition = NULL;
    	}
	}

    if (parent == NULL) {
    	// Tree was empty
    	root = addition;
    } else if (parent->doc_id >= doc_id) {
    	parent->left = addition;
    } else {
    	parent->right = addition;
    }

    if (sway == LEFT_DIR && depth > leftHeight) {
    	leftHeight = depth;
    } else if (sway == RIGHT_DIR && depth > rightHeight) {
    	rightHeight = depth;
    }

	return doc_id;
}

int
bstdb_get_word_count ( int doc_id ) {
	// This is a search function. It should traverse the binary search tree
	// and return the word_count of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return -1
	bst_num_searches++; // Increment searches
	TNode *current = root;
	while (current != NULL) {
		if (current->doc_id > doc_id) {
			current = current->left;
			bst_num_traverses++;
		} else if (current->doc_id < doc_id) {
			current = current->right;
			bst_num_traverses++;
		} else {
			return current->word_count;
		}
	}
	return -1;
}

char*
bstdb_get_name ( int doc_id ) {
	// This is a search function. It should traverse the binary search tree
	// and return the name of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return NULL or 0
	bst_num_searches++; // Increment searches
	TNode *current = root;
	while (current != NULL) {
		if (current->doc_id > doc_id) {
			current = current->left;
			bst_num_traverses++;
		} else if (current->doc_id < doc_id) {
			current = current->right;
			bst_num_traverses++;
		} else {
			return current->name;
		}
	}
	return NULL;
}

void
bstdb_stat ( void ) {
	// Use this function to show off! It will be called once after the 
	// profiler ends. The profiler checks for execution time and simple errors,
	// but you should use this function to demonstrate your own innovation.
	//
	// Suggestions for things you might want to demonstrate are given below,
	// but in general what you choose to show here is up to you. This function
	// counts for marks so make sure it does something interesting or useful.
	//
	//  + Check if your tree is balanced and print the result
	//
	//  + Does the number of nodes in the tree match the number you expect
	//    based on the number of insertions you performed?
	//
	//  + How many nodes on average did you need to traverse in order to find
	//    a search result? 
	//
	//  + Can you prove that there are no accidental duplicate document IDs
	//    in the tree?
	printf("STAT\n");
    printf("Avg traversals per search  -> %lf\n",
        (double)bst_num_traverses/bst_num_searches);
    printf("Num duplicate doc_id detected  -> %d\n", num_duplicates);
    if (leftHeight == rightHeight) {
    	printf("Balanced  -> TRUE \n");
    } else {
    	printf("Balanced  -> FALSE \n");
    }
    printf("Height on Left  -> %d\n", leftHeight);
    printf("Height on Right  -> %d\n", rightHeight);
}

void
bstdb_quit ( void ) {
	// This function will run once (and only once) when the program ends. Use
	// it to free any memory you allocated in the course of operating the
	// database.
	free_TNode(root);
	root = NULL;
}
