#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include "macro.h"

#include "header.h"


/* Adds a new macro to the macro list */
void addMacro(macro **head, char *name) {
    macro *newMacro = malloc(sizeof(macro));
    if (newMacro == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for the macro name */
    newMacro->name = malloc(strlen(name) + 1);
    if (newMacro->name == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for macro name.\n");
        free(newMacro);  /* Free previously allocated memory */
        exit(EXIT_FAILURE);
    }

    /* Copy the name into the newly allocated memory */
    strcpy(newMacro->name, name);
    newMacro->lines = NULL;
    newMacro->lineCount = 0;
    newMacro->next = *head;
    *head = newMacro;
}

/* Add a line to a macro's lines list */
void addMacroLine(macro *macro, char *line) {
    macro->lines = realloc(macro->lines, sizeof(char *) * (macro->lineCount + 1));
    if (macro->lines == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for macro lines.\n");
        exit(EXIT_FAILURE);
    }

    macro->lines[macro->lineCount] = malloc(strlen(line) + 1);
    if (macro->lines[macro->lineCount] == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for macro line.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(macro->lines[macro->lineCount], line);
    macro->lineCount++;
}

/* Finds a macro by its name in the list */
macro *findMacro(macro *head, char *name) {
    macro *current = head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/* Checks if a macro name is valid */
bool isValidMacroName(char *name) {
    if (isInstruction(name) || isDirective(name)) {
        return false;
    }
    return true;
}

/* Frees all macros and their associated memory */
void freeMacros(macro *head) {
    macro *current;
    int i;
    while (head) {
        current = head;
        head = head->next;
        for (i = 0; i < current->lineCount; i++) {
            free(current->lines[i]);
        }
        free(current->lines);
        free(current->name);
        free(current);
    }
}

/* Free the memory allocated for a linked list of code lines */
void freeLines(codeLine *head) {
    while (head != NULL) {
        codeLine *current;
        current = head;
        head = head->next;
        free(current->line);
        free(current);
    }
}