/*
* assembler.c
 *
 * Description:
 * This program is a basic assembler for a hypothetical machine.
 * It reads assembly source files, processes them through two passes
 * of the assembler, and generates output files with the machine code.
 *
 * Author: Yarden Deshe
 * Mmn 14
 *
 * Compilation:
 * - To compile this program, use a C compiler like `gcc`:
 *   gcc -o assembler assembler.c
 *
 * Usage:
 * - To run the program, execute the compiled binary with one or more source files:
 *   ./assembler sourcefile1.asm sourcefile2.asm
 */


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "preProcessor.h"
#include "firstRun.h"
#include "secondRun.h"
#include "outputFiles.h"



/*
void printBinary15Bits(short value) {
    int i;
    char binaryString[16];
    binaryString[15] = '\0';


    for (i = 0; i < 15; i++) {
        binaryString[i] = (value & (1 << (14 - i))) ? '1' : '0';
    }
    printf("%s", binaryString);
}


void printDetailedListItems(instructionList *Ilist, dataList *Dlist, int codeLength) {
    instructionList *currentInstruction = Ilist;
    dataList *currentData = Dlist;

    printf("Detailed Instruction List:\n");
    printf("Address\tBinary\t\tOctal\tValue\tOperand\n");
    if (currentInstruction == NULL) {
        printf("Instruction List is empty.\n");
    } else {
        while (currentInstruction != NULL) {
            if (currentInstruction->line != 0) {
                printf("%04d\t", currentInstruction->count);
                printBinary15Bits((short)currentInstruction->line);
                printf("\t%05o\t%05o\t%s\n",
                    currentInstruction->line, currentInstruction->line,
                    currentInstruction->symbolOperand ? currentInstruction->symbolOperand : "NULL");
            } else {
                printf("NULL\n");
            }
            currentInstruction = currentInstruction->next;
        }
    }


    printf("\nDetailed Data List:\n");
    printf("Address\tBinary\t\tOctal\tValue\n");
    if (currentData == NULL) {
        printf("Data List is empty.\n");
    } else {
        while (currentData->next != NULL) {
            printf("%04d\t", currentData->count + codeLength);
            printBinary15Bits((short)currentData->line);
            printf("\t%05o\t%05o\n",
                (currentData->count + codeLength), currentData->line);

            currentData = currentData->next;
        }
    }
}


void printLists(instructionList *Ilist, dataList *Dlist, int codeLength) {
    instructionList *currentInstruction = Ilist;
    dataList *currentData = Dlist;

    printf("Instruction List:\n");
    printf("Address\tValue\n");
    while (currentInstruction != NULL && currentInstruction->count != 0) {
        printf("%04d\t%05o\n", currentInstruction->count, currentInstruction->line);
        currentInstruction = currentInstruction->next;
    }

    printf("\nData List:\n");
    printf("Address\tValue\n");
    while (currentData != NULL && currentData->count != 0) {
        printf("%04d\t%05o\n", currentData->count + codeLength, currentData->line);
        currentData = currentData->next;
    }
}
*/


/*
 * Main function to execute the program.
 * @param argc The number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return 0 if successful, otherwise 1.
 */
int main(int argc, char *argv[]) {
    int i;

    /* Check if at least one input file is provided */
    if (argc < 2) {
        printf("Usage: %s <input file 1> [<input file 2> ...]\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++) {

        int IC = INITIAL_IC, DC = INITIAL_DC;
        char *outputFileName;
        FILE *sourceFile, *outputFile = NULL;
        symbolTable *symTable;
        instructionList *Ihead, *Itail;
        dataList *Dhead, *Dtail;

        /* Open the source file */
        sourceFile = fopen(argv[i], "r");
        if (sourceFile == NULL) {
            printf("Error opening source file: %s\n", argv[i]);
            continue;
        }

        /* Prepare output file name with changed extension and expand macros */
        outputFileName = changeFileExtension(argv[i], ".am");
        if (!expandMacros(sourceFile, outputFile, outputFileName)) {
            fprintf(stderr, "Error expanding macros for file: %s\n", argv[i]);
        }
        else {
            /* Initialize symbol table and lists */
            symTable = initSymbolTable();
            Ihead = initInstructionList();
            Itail = Ihead;
            Dhead = initDataList();
            Dtail = Dhead;

            /* Perform the first assembler pass */
            firstAssemblerPass( outputFileName, symTable, &Itail, &Dtail, &IC, &DC);

            Itail = Ihead;
            Dtail = Dhead;

            /* Perform the second assembler pass */
            secondAssemblerPass( outputFileName, symTable,  &Itail);

            Itail = Ihead;

            /* Create the output files and print the lists */
            createOutputFiles(argv[i], Itail, Dtail, IC, DC, symTable);
            /*
            printDetailedListItems(Ihead, Dhead, IC);  //todo delete debug only
*/
            /* Free the memory allocated */
            freeInstructionList(Ihead);
            freeDataList(Dhead);
            printSymbolTable(symTable); //todo delete debug only
            freeSymbolTable(symTable);

        }
        /* Close the source file and output file */
        fclose(sourceFile);

        free(outputFileName);
    }
    return 0;
}
