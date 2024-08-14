#ifndef FIRSTRUN_H
#define FIRSTRUN_H

#include "memory.h"
#include "symbolTable.h"

/*
 * Processes the first pass of the assembler to validate file content and prepare data and instruction lists.
 *
 * This function reads the content of the specified file, processes each line to handle symbols, directives,
 * and instructions, and updates the symbol table, instruction list, and data list accordingly. It also
 * calculates the initial instruction counter (IC) and data counter (DC) values.
 *
 * @param outputFileName The name of the file to process.
 * @param symTable Pointer to the symbol table to be updated.
 * @param Itail Pointer to the instruction list tail to be updated.
 * @param Dtail Pointer to the data list tail to be updated.
 * @param ICInitial Pointer to store the initial instruction counter value.
 * @param DCInitial Pointer to store the initial data counter value.
 */
void firstAssemblerPass(char *outputFileName, symbolTable *symTable, instructionList **Itail,
                        dataList **Dtail, int *ICInitial, int *DCInitial);

#endif
