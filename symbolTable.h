#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "header.h"

#define MAX_TYPE_LENGTH 10

/* Structure representing a symbol in the symbol table. */
typedef struct {
    char name[MAX_LABEL_LENGTH];  /* The name of the symbol */
    int address;                  /* The address associated with the symbol */
    char type[MAX_TYPE_LENGTH];   /* The type of the symbol */
} Symbol;

/* Structure representing the symbol table. */
typedef struct {
    Symbol **symbols; /* Array of pointers to symbols */
    int count;        /* Number of symbols in the table */
} symbolTable;

/*
 * Initializes a new symbol table.
 *
 * This function allocates memory for a new symbol table and initializes it.
 *
 * @return A pointer to the newly created symbol table.
 */
symbolTable *initSymbolTable();

/*
 * Frees memory allocated for the symbol table.
 *
 * This function deallocates all memory used by the symbol table, including
 * individual symbols and the table itself.
 *
 * @param symTable A pointer to the symbol table to be freed.
 */
void freeSymbolTable(symbolTable *symTable);

/*
 * Adds a new symbol to the symbol table.
 *
 * This function creates a new symbol and adds it to the symbol table.
 *
 * @param symTable A pointer to the symbol table.
 * @param name The name of the symbol to be added.
 * @param type The type of the symbol.
 * @param address The address associated with the symbol.
 */
void addSymbol(symbolTable *symTable, char *name, char *type, int address);

/*
 * Finds a symbol by its name in the symbol table.
 *
 * This function searches for a symbol with the given name in the symbol table.
 *
 * @param symTable A pointer to the symbol table.
 * @param name The name of the symbol to search for.
 * @return A pointer to the symbol if found, or NULL if not found.
 */
Symbol* findSymbol(symbolTable *symTable, char *name);


void updateSymbolType(symbolTable *symTable, char *name);

void printSymbolTable(symbolTable *symTable); /* temporary debug */
#endif
