#include <stdio.h>
#include <string.h>
#include "secondRun.h"
#include "machineCode.h"
#include "processorUtils.h"


/* Updates the symbol type to "entry" in the symbol table. */
void updateSymbolToEntry(symbolTable *symTable, char *name) {
    Symbol *symbol = findSymbol(symTable, name);  /* Find the symbol in the symbol table */

    if (symbol != NULL) {
        strcpy(symbol->type, "entry");  /* Update the type to "entry" */
    } else {
        fprintf(stderr, "Entry symbol not found in symbol table: %s\n", name);
    }
}

/* Processes a line containing .entry directive and updates the symbol type in the symbol table. */
void processEntryLine(char *line, symbolTable *symTable) {
    char *token;

    removeDirectiveFromLine(line);
    token = strtok(line, " \t\n");
    while (token) {
        /* Update each symbol to entry */
        updateSymbolToEntry(symTable, token);
        token = strtok(NULL, " \t\n");
    }
}

/* Checks if a line contains the .entry directive. */
bool isEntryLine(char *line) {
    char *token;
    char tempLine[MAX_LINE_LENGTH + 1];

    /* Copy the original line to a temporary buffer to avoid modifying the original */
    strcpy(tempLine, line);

    token = strtok(tempLine, " \t\n");  /* Tokenize the copied line */
    ignoreLeftWhiteSpaces(token);  /* Remove leading white spaces */

    if (strcmp(token, ".entry") == 0) {  /* Check if the token is ".entry" */
        return true;
    }
    return false;
}

/* Updates the addresses of operands in the instruction list based on the symbol table. */
void updateOperandsAddress(instructionList *list, symbolTable *symTable) {
    Symbol *symbol;

    while (list != NULL) {
        if (list->symbolOperand != NULL) {
            symbol = findSymbol(symTable, list->symbolOperand);  /* Find the symbol in the table */

            if (symbol != NULL) {
                list->line = writeLabelAddress(symbol);  /* Update the line with the symbol's address */
            } else {
                fprintf(stderr, "Symbol not found for operand: %s\n", list->symbolOperand);
            }
        }
        list = list->next;  /* Move to the next instruction */
    }
}

/* Performs the second pass of the assembler. */
void secondAssemblerPass(char *outputFileName, symbolTable *symTable, instructionList **Itail) {
    FILE *file;
    char line[MAX_LINE_LENGTH + 1];

    file = fopen(outputFileName, "r");  /* Open the output file from the first pass */
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", outputFileName);
        return;
    }

    /* Read each line from the file */
    while (fgets(line, sizeof(line), file)) {
        if (ferror(file)) {
            fprintf(stderr, "Error reading file 2pass: %s\n", outputFileName);
            break;
        }

        if (isNoteLine(line) || isEmptyLine(line)) { continue; }  /* Skip comments and empty lines */
        if (isEntryLine(line)) {
            processEntryLine(line, symTable);  /* Process .entry lines */
        }
    }
    updateOperandsAddress(*Itail, symTable);  /* Update operand addresses based on the symbol table */
}
