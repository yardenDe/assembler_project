#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"


/* Initializes the instruction list with a single node. */
instructionList *initInstructionList() {
    return allocateNewINode(); /* Initialize with a single node */
}

/* Allocates and initializes a new instruction list node. */
instructionList *allocateNewINode() {

    /* Allocate memory for new node */
    instructionList *newNode = malloc(sizeof(instructionList));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize node fields */
    newNode->count = 0;
    newNode->line = 0;
    newNode->symbolOperand = NULL;
    newNode->next = NULL;
    return newNode;
}

/* Adds a new line to the instruction list and allocates a new node. */
void addToInstructionList(instructionList **tail,char *operand, int count,unsigned short line) {

    /* Set the values of the current tail node */
    (*tail)->count = count;
    (*tail)->line = line;

    /* Allocate and copy the operand string if it is not NULL */
    if (operand != NULL) {
        (*tail)->symbolOperand = malloc(strlen(operand)+1);
        if ((*tail)->symbolOperand == NULL) {
            fprintf(stderr, "Memory allocation failed for symbolOperand\n");
            exit(EXIT_FAILURE);
        }
        strcpy((*tail)->symbolOperand, operand);
    }


    /* Allocate the next node */
    (*tail)->next = allocateNewINode();

    /* Move the tail to the next node */
    *tail = (*tail)->next;
}

/* Frees all nodes in the instruction list and their symbol operands. */
void freeInstructionList(instructionList *head) {
    instructionList *current = head;
    instructionList *next;

    while (current != NULL) {
        next = current->next;
        free(current->symbolOperand); /* Free the symbol operand if allocated */
        free(current);
        current = next;
    }
    head = NULL;
}