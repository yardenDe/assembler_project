#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "memory.h"
#include "outputFiles.h"

/* Changes the file extension of the given file name. */
char *changeFileExtension(char *fileName, char *newExtension) {
    char *newFileName, *base;

    base = strtok(fileName, ".");  /* Separate base name from extension */
    newFileName = malloc(strlen(base) + strlen(newExtension) + 1);
    if (newFileName == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(newFileName, base);   /* Copy base name */
    strcat(newFileName, newExtension);   /* Add new extension */

    return newFileName;
}

/* Creates an object file with machine code and data sections. */
void createObjectFile(char *filename, instructionList *Ilist, dataList *Dlist, int codeLength, int dataLength) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s for writing.\n", filename);
        exit(EXIT_FAILURE);
    }

    /* Write header: code length and data length */
    fprintf(file, "%4d %d\n", codeLength - INITIAL_IC, dataLength);

    while (Ilist != NULL && Ilist->count != 0) {
        /* Write each instruction in octal format */
        fprintf(file, "%04d %05o\n", Ilist->count, Ilist->line);
        Ilist = Ilist->next;
    }

    while (Dlist != NULL) {
        /* Write each data line in octal format */
        if (Dlist->next != NULL) {
            fprintf(file, "%04d %05o\n", Dlist->count + codeLength, Dlist->line);
        }
        Dlist = Dlist->next;
    }
    /* Close the file after writing */
    fclose(file);
}

/* Creates a file listing all entry symbols with their addresses, only if entry symbols exist. */
void cerateEntriesFile(char *filename, symbolTable *symTable) {
    FILE *file = NULL;
    int i;

    for (i = 0; i < symTable->count; i++) {
        if (strcmp(symTable->symbols[i]->type, "entry") == 0) {
            if (file == NULL) {
                file = fopen(filename, "w");
                if (file == NULL) {
                    fprintf(stderr, "Error: Cannot open file %s for writing.\n", filename);
                    exit(EXIT_FAILURE);
                }
            }
            /* Write each entry symbol's name and address */
            fprintf(file, "%s %d\n", symTable->symbols[i]->name, symTable->symbols[i]->address);
        }
    }
    /* Close the file if it was opened */
    if (file != NULL) {
        fclose(file);
    }
}


/* Creates a file listing all external symbols used in the program, only if external symbols exist. */
void cerateExternalsFile(char *filename, instructionList *Ilist, ExternalSymbolArray *extArray) {
    FILE *file;

    if (extArray != NULL && extArray->count > 0) {

        file = fopen(filename, "w");
        if (file == NULL) {
            fprintf(stderr, "Error: Cannot open file %s for writing.\n", filename);
            exit(EXIT_FAILURE);
        }

        while (Ilist != NULL) {
            if (Ilist->symbolOperand != NULL) {
                /* Check if the symbol is external and write it */
                if (isExternalSymbol(extArray, Ilist->symbolOperand)) {
                    fprintf(file, "%s %04d\n", Ilist->symbolOperand, Ilist->count);
                }
            }
            Ilist = Ilist->next;
        }
        /* Close the file after writing */
        fclose(file);
    }
}

/* Creates all necessary output files for the assembler. */
void createOutputFiles(char *sourceFileName, instructionList *Ilist, dataList *Dlist, int codeLength, int dataLength, symbolTable *symTable) {
    char *objectFileName, *entryFileName, *externalFileName;
    ExternalSymbolArray *extArray;

    /* Create file names with appropriate extensions. */
    objectFileName = changeFileExtension(sourceFileName, ".ob");
    entryFileName = changeFileExtension(sourceFileName, ".ent");
    externalFileName = changeFileExtension(sourceFileName, ".ext");

    /* Create an array for external symbols. */
    extArray = initExternalSymbolArray();
    createExternalSymbolsArray(symTable, extArray);

    /* Create the output files. */
    createObjectFile(objectFileName, Ilist, Dlist, codeLength, dataLength);
    cerateEntriesFile(entryFileName, symTable);
    cerateExternalsFile(externalFileName, Ilist, extArray);

    /* Free allocated memory */
    free(objectFileName);
    free(entryFileName);
    free(externalFileName);
    freeExternalSymbolArray(extArray);
}
