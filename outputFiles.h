#ifndef OUTPUTFILES_H
#define OUTPUTFILES_H

#include "symbolTable.h"
#include "external.h"

/* Changes the file extension of the given file name.
 * The caller is responsible for freeing the allocated memory
 * @param fileName The original file name.
 * @param newExtension The new file extension .
 * @return A new string with the file name and the new extension.
 */
char *changeFileExtension(char *fileName, char *newExtension);

/* Creates an object file with machine code and data sections. */
/*
 * @param filename The name of the object file to create.
 * @param Ilist The list of instructions to write.
 * @param Dlist The list of data to write.
 * @param codeLength The length of the code section.
 * @param dataLength The length of the data section.
 */
void createObjectFile(char *filename, instructionList *Ilist, dataList *Dlist, int codeLength, int dataLength);

/* Creates a file listing all entry symbols with their addresses. */
/*
 * @param filename The name of the entries file to create.
 * @param symTable The symbol table containing symbols and their properties.
 */
void cerateEntriesFile(char *filename, symbolTable *symTable);

/* Creates a file listing all external symbols used in the program. */
/*
 * @param filename The name of the externals file to create.
 * @param symTable The symbol table containing symbols and their properties.
 * @param Ilist The list of instructions to write.
 * @param extArray The array of external symbols.
 */
void cerateExternalsFile(char *filename, instructionList *Ilist, ExternalSymbolArray *extArray);

/* Creates all necessary output files for the assembler. */
/*
 * @param sourceFileName The source file name without extension.
 * @param Ilist The list of instructions.
 * @param Dlist The list of data.
 * @param codeLength The length of the code section.
 * @param dataLength The length of the data section.
 * @param symTable The symbol table containing symbols and their properties.
 */
void createOutputFiles(char *sourceFileName, instructionList *Ilist, dataList *Dlist, int codeLength, int dataLength, symbolTable *symTable);

#endif
