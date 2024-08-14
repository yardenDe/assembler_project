#include "external.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Initialize a new external symbol array */
ExternalSymbolArray *initExternalSymbolArray() {
    ExternalSymbolArray *extArray;
    extArray = malloc(sizeof(ExternalSymbolArray));
    if (extArray == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    /* Initialize the array with default values: count set to 0 and symbols pointer set to NULL */
    extArray->count = 0;
    extArray->symbols = NULL;
    return extArray;
}

/* Function to add a new symbol to the external symbol array */
void addExternalSymbol(ExternalSymbolArray *extArray, const char *name) {

    /* Reallocate memory for the array to accommodate one more symbol */
    extArray->symbols = realloc(extArray->symbols, sizeof(ExternalSymbol) * (extArray->count + 1));
    if (extArray->symbols == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    /* Allocate and copy the symbol name */
    extArray->symbols[extArray->count].name = malloc(strlen(name)+1);
    if (extArray->symbols[extArray->count].name == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(extArray->symbols[extArray->count].name, name);
    extArray->count++;
}

/* Function to create an array of external symbols from a symbol table */
void createExternalSymbolsArray(symbolTable *symTable, ExternalSymbolArray *extArray) {
    Symbol *symbol;
    int i;
    for (i = 0; i < symTable->count; i++) {
        symbol = symTable->symbols[i];
        if (strcmp(symbol->type, "external") == 0) {
            addExternalSymbol(extArray, symbol->name);
        }
    }
}

/* Function to check if a symbol is in the external symbol array */
bool isExternalSymbol(ExternalSymbolArray *extArray, const char *name) {
    int i;
    for (i = 0; i < extArray->count; i++) {
        if (strcmp(extArray->symbols[i].name, name) == 0) {
            return true;
        }
    }
    return false;
}

/* Function to free the memory of the external symbol array */
void freeExternalSymbolArray(ExternalSymbolArray *extArray) {
    int i;
    for (i = 0; i < extArray->count; i++) {
        free(extArray->symbols[i].name);  /* Free the symbol name memory */
    }
    free(extArray->symbols);               /* Free the symbols array */
    free(extArray);                        /* Free the array structure */
}
