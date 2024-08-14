#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

/* Initializes a data list with a single node. */
dataList *initDataList() {
    return allocateNewDNode(); /* Initialize with a single node */
}

/* Allocates and initializes a new node for the data list. */
dataList *allocateNewDNode() {

    /* Allocate memory for new node */
    dataList *newNode = malloc(sizeof(dataList));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize the new node's fields */
    newNode->count = 0;
    newNode->line = 0;
    newNode->next = NULL;
    return newNode;
}

/* Adds a new node to the end of the data list. */
void addToDataList(dataList **tail, int count, unsigned short line) {
    /* Set the values of the current tail node */
    (*tail)->count = count;
    (*tail)->line = line;

    /* Allocate the next node */
    (*tail)->next = allocateNewDNode();

    /* Move the tail to the next node */
    *tail = (*tail)->next;
}

/* Frees all nodes in the data list. */
void freeDataList(dataList *head) {
    dataList *current = head;  /* Pointer to traverse the list */
    dataList *next;            /* Pointer to hold the next node */

    while (current != NULL) {
        next = current->next;  /* Save the next node */
        free(current);         /* Free the memory of the current node */
        current = next;        /* Move to the next node */
    }
    head = NULL;               /* Set the head pointer to NULL after freeing the list */
}

