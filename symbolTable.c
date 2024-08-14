#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"


/* todo - to delete temporary debug */
void printSymbolTable(symbolTable *symTable) {
    int i;

    printf("------------------------------------------\n");
    printf("%-20s %-10s %-10s\n", "Name", "Address", "Type");
    printf("------------------------------------------\n");
    for (i = 0; i < symTable->count; i++) {
        printf("%-20s %-10d %-10s\n", symTable->symbols[i]->name, symTable->symbols[i]->address,
               symTable->symbols[i]->type);
    }
    printf("------------------------------------------\n");
}


/* Initializes a new symbol table */
symbolTable *initSymbolTable() {
    symbolTable *table = malloc(sizeof(symbolTable));
    if (table == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    table->count = 0;
    table->symbols = NULL;

    return table;
}


/* Adds a new symbol to the symbol table */
void addSymbol(symbolTable *symTable, char *name, char *type, int address) {
    Symbol *newSymbol;
    if (symTable == NULL) {
        fprintf(stderr, "Error: Symbol table is NULL.\n");
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for the new symbol */
    newSymbol = malloc(sizeof(Symbol));
    if (newSymbol == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    /* Copy the symbol's name and type into allocated memory */
    strncpy(newSymbol->name, name, MAX_LABEL_LENGTH - 1);
    newSymbol->name[MAX_LABEL_LENGTH - 1] = '\0';
    newSymbol->address = address;
    strncpy(newSymbol->type, type, MAX_TYPE_LENGTH - 1);
    newSymbol->type[MAX_TYPE_LENGTH - 1] = '\0';

    /* Reallocate memory for the symbols array */
    symTable->symbols = realloc(symTable->symbols, (symTable->count + 1) * sizeof(Symbol *));
    if (symTable->symbols == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    /* Add the new symbol to the symbols array */
    symTable->symbols[symTable->count] = newSymbol;
    symTable->count++;
}

/* Finds a symbol by its name in the symbol table. */
Symbol* findSymbol(symbolTable *symTable, char *name) {
    int i;
    if (symTable == NULL) {
        fprintf(stderr, "Error: Symbol table is NULL.\n");
        return NULL;
    }

    /* Search for the symbol with the given name */
    for (i = 0; i < symTable->count; i++) {
        if (strcmp(symTable->symbols[i]->name, name) == 0) {
            return symTable->symbols[i];
        }
    }

    return NULL;
}

/* Frees all memory allocated for the symbol table */
void freeSymbolTable(symbolTable *symTable) {
    int i;

    if (symTable == NULL) {
        fprintf(stderr, "Error: Symbol table is NULL.\n");
        return;
    }

    /* Free memory allocated for each symbol */
    for (i = 0; i < symTable->count; i++) {
        free(symTable->symbols[i]);
    }

    /* Free memory allocated for the symbols array */
    free(symTable->symbols);
    symTable->count = 0;

}


