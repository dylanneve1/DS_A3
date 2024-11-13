#include "bstdb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DOCS 2000000

#define LEFT_DIR 101
#define RIGHT_DIR 102

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

int bst_num_traverses, bst_num_searches, num_duplicates, leftHeight, rightHeight, first, leftIDremaining, rightIDremaining;

// This function counts all nodes in tree
int
countNodes(TNode *node) {
    if (node == NULL) {
        return 0;
    }
    // Count the current node plus nodes in left and right subtrees recursively
    return 1 + countNodes(node->left) + countNodes(node->right);
}

double
getCapacity() {
    int ret = 0;
    for (int i = 0; i < MAX_DOCS; i++) {
    	if (IDtable[i] == 1) {
    		ret++;
    	}
    }
    return ret;
}

// This function frees all TNode and strings inside recursively
void
free_TNode (TNode *node) {
	// If node is NUll return
	// Nothing to do
    if (node == NULL) {
        return;
    }
    // Free left nodes recursively
    free_TNode(node->left);
    // Free right nodes recursively
    free_TNode(node->right);
    // Free name and Author
    free(node->name);
    free(node->author);
    // Free the index
    IDtable[node->doc_id] = 0;
    // Free the main node
    free(node);
}

int
bstdb_init ( void ) {
	srand(time(NULL));
	// First not set
    first = 1;
    // Left and Right IDs (MAX / 2)
    leftIDremaining = MAX_DOCS / 2;
    rightIDremaining = MAX_DOCS / 2;
    // Initialize metrics to zero
    bst_num_traverses = 0;
    bst_num_searches = 0;
    num_duplicates = 0;
    leftHeight = 0;
    rightHeight = 0;
    // Initialize root node to NULL
    root = NULL;
    // Return 1 (Success)
    return 1;
}

///////////////////////////////////////////////
// Midpoint Balanced ID Generation Algorithm //
///////////////////////////////////////////////
int
midpointBalancedGenerationAlgorithm () {
	// Calculate the midpoint
    int midpoint = MAX_DOCS / 2;
    // Frst ID must be midpoint
    if (first) {
    	// First node done
        first = 0;
        // Mark ID as taken
        IDtable[midpoint] = 1;
        return midpoint;
    }
    int doc_id;
    if (leftHeight > rightHeight && rightIDremaining > 0) { // Generate ID to the right
        rightIDremaining--;
        do {
            doc_id = midpoint + rand() % midpoint;
        } while (IDtable[doc_id] == 1);
    } else if (rightHeight > leftHeight && leftIDremaining > 0) { // Generate ID to the left
        leftIDremaining--;
        do {
            doc_id = rand() % midpoint; 
        } while (IDtable[doc_id] == 1);
    } else if (leftIDremaining == 0 || rightIDremaining == 0) { // IDs have run out
        return -1;
    } else { // Weighting is equal, generate a random ID
        do {
            doc_id = rand() % MAX_DOCS; 
        } while (IDtable[doc_id] == 1);
    }
    // Mark ID as taken and return calculated ID
    IDtable[doc_id] = 1;
    return doc_id;
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

	// Initialize depth and sway
    int depth = 0;
    int sway = 0;

    // Calculate DOC_ID with ID generation algorithm
    int doc_id = midpointBalancedGenerationAlgorithm();

    // Smart Selector ran out of indexes
    if (doc_id == -1) {
        return -1;
    }

    // Current and parent pointers
    TNode *current = root;
    TNode *parent = NULL;

    // Find the insertion point
    // Keep going until we find the valid mode to insert
    while (current != NULL) {
        parent = current;
        if (current->doc_id > doc_id) {
            current = current->left;
            // Increment the depth
            depth++;
            if (sway == 0) { // If sway not set, leaning left from root
                sway = LEFT_DIR;
            }
        } else if (current->doc_id < doc_id) {
            current = current->right;
            // Increment the depth
            depth++;
            if (sway == 0) { // If sway not set, leaning right from root
                sway = RIGHT_DIR;
            }
        } else if (current->doc_id == doc_id) {
            // Duplicate found, not good!
            // This means ID is already taken
            // This should never happen
            num_duplicates++;
            // Failed to insert
            return -1;
        }
    }

    // We have our insertion point
    // Alocate and populate new node to be inserted
    TNode *addition = malloc(sizeof(TNode));
    if (!addition) {       
    	// Malloc failed!
        return -1;
    }
    // Populate fields
    addition->doc_id = doc_id;
    addition->word_count = word_count;
    // Initialize left and right pointers to NULL
    addition->left = NULL;
    addition->right = NULL;

    // If name is valid, allocate the space and copy string in
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
            // Failed to insert
            return -1;
        }
    }

    // If author is valid, allocate the space and copy string in
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
            // Failed to insert
            return -1;
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

    // Update heights if they have changed
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
    double capacity = (getCapacity() / MAX_DOCS) * 100;
    printf("STAT\n");
    printf("Avg nodes traversed per search  -> %lf\n",
        (double)bst_num_traverses/bst_num_searches);
    printf("Num duplicate doc_id detected  -> %d\n", num_duplicates);
    if (leftHeight == rightHeight) {
        printf("Balanced  -> TRUE \n");
    } else {
        printf("Balanced  -> FALSE \n");
    }
    printf("Height on Left  -> %d\n", leftHeight);
    printf("Height on Right  -> %d\n", rightHeight);
    printf("Nodes in Tree  -> %d\n", countNodes(root));
    printf("Tree Capacity  -> %.2f", capacity);
    printf("%%\n");
}

void
bstdb_quit ( void ) {
    // This function will run once (and only once) when the program ends. Use
    // it to free any memory you allocated in the course of operating the
    // database.
    free_TNode(root);
    root = NULL;
}
