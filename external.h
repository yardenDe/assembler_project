#ifndef EXTERNAL_SYMBOLS_H
#define EXTERNAL_SYMBOLS_H

#include "symbolTable.h"
#include <stdbool.h>


/* Structure representing an external symbol */
typedef struct {
    char *name;  /* Name of the external symbol */
} ExternalSymbol;

/* Structure representing an array of external symbols */
typedef struct {
    int count;                /* Number of external symbols in the array */
    ExternalSymbol *symbols;  /* Pointer to the array of external symbols */
} ExternalSymbolArray;

/*
 * Initializes a new ExternalSymbolArray.
 *
 * Allocates memory for an ExternalSymbolArray and sets its count to 0.
 * The symbols pointer is initialized to NULL.
 *
 * @return A pointer to the newly created ExternalSymbolArray.
 */
ExternalSymbolArray *initExternalSymbolArray();

/*
 * Creates an array of external symbols from a symbol table.
 *
 * This function iterates over the symbol table and adds all symbols of type "external"
 * to the ExternalSymbolArray.
 *
 * @param symTable A pointer to the symbolTable containing symbols.
 * @param extArray A pointer to the ExternalSymbolArray to be populated.
 */
void createExternalSymbolsArray(symbolTable *symTable, ExternalSymbolArray *extArray);

/*
 * Adds a new external symbol to the ExternalSymbolArray.
 *
 * This function reallocates memory for the symbols array to accommodate
 * the new symbol, then adds the symbol to the array.
 *
 * @param extArray A pointer to the ExternalSymbolArray to which the symbol will be added.
 * @param name The name of the external symbol to add.
 */
void addExternalSymbol(ExternalSymbolArray *extArray, const char *name);

/*
 * Checks if a symbol is present in the ExternalSymbolArray.
 *
 * This function searches for the given symbol name in the ExternalSymbolArray.
 *
 * @param extArray A pointer to the ExternalSymbolArray to be searched.
 * @param name The name of the symbol to search for.
 * @return True if the symbol is found, otherwise false.
 */
bool isExternalSymbol(ExternalSymbolArray *extArray, const char *name);

/*
 * Frees the memory allocated for an ExternalSymbolArray.
 *
 * This function frees the memory for each external symbol's name, the symbols array,
 * and the ExternalSymbolArray itself.
 *
 * @param extArray A pointer to the ExternalSymbolArray to be freed.
 */
void freeExternalSymbolArray(ExternalSymbolArray *extArray);


#endif
