#ifndef SECINDRUN_H
#define SECINDRUN_H

#include "symbolTable.h"
#include "memory.h"

/*
 * Performs the second pass of the assembler.
 *
 * This function reads the output file from the first pass, processes lines with .entry directives
 * to update the symbol types in the symbol table, and updates the addresses of operands in
 * the instruction list.
 *
 * @param outputFileName The name of the output file from the first pass.
 * @param symTable The symbol table used for updating entry symbols and finding symbol addresses.
 * @param Itail A pointer to the pointer of the last node in the instruction list. This will be updated as needed.
 */
void secondAssemblerPass(char *outputFileName, symbolTable *symTable, instructionList **Itail);

#endif
