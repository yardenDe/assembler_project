#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "firstRun.h"
#include "processorUtils.h"
#include "machineCode.h"


int errors = 0;

 /* Updates the addresses of data symbols in the symbol table by adding the instruction counter (IC). */
void updateSymbolAddress(int IC, symbolTable *symTable) {
    int i;
    for (i = 0; i < symTable->count; i++) {
        if (strcmp(symTable->symbols[i]->type, "data") == 0) {
            symTable->symbols[i]->address += IC;
        }
    }
}

/* Parses a line with external symbols and adds them to the symbol table. */
void handleExternalLine(char *line, symbolTable *symTable) {
    char *token;
    ignoreLeftWhiteSpaces(line);
    removeDirectiveFromLine(line);

    token = strtok(line, " \t\n");
    while (token) {
        addSymbol(symTable, token, "external", 0);
        token = strtok(NULL, " \t\n");
    }
}

/* Parses valid values from a data line and stores them in an array. */
void parseDataArray(char *line, unsigned short **content, int *dataCount) {
    int commas;
    char *token, *whitespace;

    commas = expectedCommas(line);
    token = strtok(line, ",");
    *content = (unsigned short *) malloc(sizeof(unsigned short) * (commas + 1));

    /* Validate successful allocating memory for data array */
    if (*content != NULL) {
        while (token) {
            /* Remove leading whitespace from the token */
            ignoreLeftWhiteSpaces(token);

            whitespace = token; /* Check for trailing whitespace */
            while (*whitespace) {
                if(isspace(*whitespace) ) {break; }
                whitespace++;
            }

            if (*whitespace) {
                if (!isEmptyLine(whitespace)) {
                    fprintf(stderr, "Error - Comma expected\n");
                    errors++;
                    return;
                }
            }

            /* Convert token to numeric and store it */
            if (isNumeric(token)) {
                (*content)[*dataCount] = atoi(token);
                (*dataCount)++;
            }
            token = strtok(NULL, ",");
        }

        /* Validate the number of data items */
        if (commas + 1 != *dataCount) {
            errors++;
            fprintf(stderr, "Error - invalid data format\n");
        }
        return;
    }
    errors++;
    fprintf(stderr, "Memory allocation failed\n");
}

/* Parses a string line and stores characters in an array. */
void parseStringArray(char *line, unsigned short **content, int *dataCount) {
    int length, i;
    char *startQuote, *endQuote;

    /* Remove leading whitespace from the line */
    ignoreLeftWhiteSpaces(line);

    /* Find the starting and ending quote of the string */
    startQuote = strchr(line, '"');
    endQuote = strrchr(line, '"');

    /* Validate string format */
    if (startQuote == NULL || endQuote == NULL || startQuote == endQuote) {
        errors++;
        fprintf(stderr, "Error - Invalid string format\n");
        return;
    }

    length = endQuote - startQuote - 1;
    *content = (unsigned short *) malloc((length + 1) * sizeof(unsigned short));
    /* Validate allocating memory for string content */
    if (*content == NULL) {
        errors++;
        fprintf(stderr, "Error - Memory allocation failed\n");
        return;
    }

    for (i = 0; i < length; i++) {
        /* Ensure each character is alphabetic */
        if (!isalpha(startQuote[i + 1])) {
            errors++;
            fprintf(stderr, "Error - Invalid string format\n");
            return;
        }
        /* Store each character */
        (*content)[i] = (unsigned short) startQuote[i + 1];
    }

    (*content)[length] = 0;  /* Null-terminate the string */
    *dataCount = length + 1; /* Set the data count to the length of the string plus the null terminator */
}

/* Writes data line values to the data list. */
void writeDataToList(int count, unsigned short *content, dataList **tail, int DC) {
    int i;
    /* Convert data to 15-bit format and Add to the data list */
    for (i = 0; i < count; i++) {
        content[i] = word15bits(content[i]);
        addToDataList(tail, DC + i, content[i]);
    }
}
/* Processes a line containing data or string directives and updates the data list. */
void processDataLine(char *line, int *DC, dataList **tail) {
    char directive[MAX_LINE_LENGTH];
    unsigned short *content = NULL;
    int dataCount  = 0;

    /* Remove leading whitespace, directive from the line */
    ignoreLeftWhiteSpaces(line);
    sscanf(line, "%s", directive);
    removeDirectiveFromLine(line);

    /* Handle different types of directives */
    if (strcmp(directive, ".data") == 0) {
        parseDataArray(line, &content, &dataCount);
        writeDataToList(dataCount, content, tail, *DC);
    } else if (strcmp(directive, ".string") == 0) {
        parseStringArray(line, &content, &dataCount);
        writeDataToList(dataCount, content, tail, *DC);
    }

    /* Free allocated memory */
    if (content) {
        free(content);
    }
    /* Update the data counter */
    (*DC) += dataCount;

}

/* Processes a line with an operation and updates the instruction list. */
void processInstrctionline(char *line, instructionList **tail, int *IC) {

    char operation[MAX_LINE_LENGTH];
    char *sourceOperand = NULL, *destOperand = NULL;
    int estOperands;
    sscanf(line, "%s", operation); /* Extract the operation from the line */

    ignoreLeftWhiteSpaces(line);
    while (!isspace(*line)) { line++; } /* Remove operation from line */
    estOperands = estimatedOperands(operation);

    /* Validate operands and add to instruction list */
    if (validateOperands(estOperands, line, &sourceOperand, &destOperand)) {

        /* Add line to instruction line list*/
        addInstructionLine(operation, sourceOperand, destOperand, *IC, tail);

        /* Check for registers*/
        if (estOperands > 1 && isRegister(sourceOperand) && isRegister(destOperand)) {
            (*IC)++;
        } else {
            (*IC) += estOperands;
        }
    } else {
        errors++;
        fprintf(stderr, "Error - invalid operands in line: %s\n", line);
    }
    /* Increment instruction counter for the next instruction. */
    (*IC)++;
}

/* Processes the first pass of the assembler to validate file content and prepare data and instruction lists. */
void firstAssemblerPass(char *outputFileName, symbolTable *symTable, instructionList **Itail,
                        dataList **Dtail, int *ICInitial, int *DCInitial) {
    FILE * file;
    int IC = 100, DC = 0;
    char line[MAX_LINE_LENGTH + 1];

    /* Open the file for reading */
    file = fopen(outputFileName, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", outputFileName);
        return;
    }

    /* Process each line in the file */
    while (fgets(line, sizeof(line), file)) {
        char symbol[MAX_LABEL_LENGTH];
        bool symbolFlag = false;

        /* Skipping comment or empty line */
        if (isNoteLine(line) || isEmptyLine(line)) {
            continue;
        }

        /* Handle label symbols */
        if (isSymbol(line, symbol)) {
            symbolFlag = true;

            if (!isValidLabel(symbol)) {
                errors++;
                fprintf(stderr, "Error - invalid label: %s\n", symbol);
                continue;
            }
            removeSymbolFromLine(line);
        }

        if (isDirectiveLine(line)) {
            if (isExternalLine(line)) {
                handleExternalLine(line, symTable);
            } else if (isDataOrString(line)) {

                if (symbolFlag) {
                    addSymbol(symTable, symbol, "data", DC);
                }

                processDataLine(line, &DC, Dtail);
            }
        } else if (isInstructionLine(line)) {
            if (symbolFlag) {
                addSymbol(symTable, symbol, "code", IC);
            }

            processInstrctionline(line, Itail, &IC);
        } else {
            errors++;
            fprintf(stderr, "Error - invalid format input: %s\n", line);
        }
    }

    *ICInitial = IC;  /* Set the initial instruction counter */
    *DCInitial = DC;  /* Set the initial data counter */

    /* Update symbol addresses*/
    updateSymbolAddress(IC, symTable);

    fclose(file);

}