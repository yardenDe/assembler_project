#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"
#include "macro.h"

/* Array of instruction names */
const char *instructions[] = {
    "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
    "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};

/* Array of directive names */
const char *directives[] = {
    ".data", ".string", ".entry", ".extern"
};

/* Check if a given name is a valid instruction */
bool isInstruction(char *name) {
    int i;
    for (i = 0; i < sizeof(instructions) / sizeof(instructions[0]); i++) {
        if (strcmp(name, instructions[i]) == 0) {
            return true;
        }
    }
    return false;
}

/* Check if a given name is a valid directive */
bool isDirective(char *name) {
    int i;
    for (i = 0; i < sizeof(directives) / sizeof(directives[0]); i++) {
        if (strcmp(name, directives[i]) == 0) {
            return true;
        }
    }
    return false;
}


/* Handle macro definition and store its lines */
bool handleMacro(FILE *sourceFile, macro **macroList, char *macroName) {
    char line[MAX_LINE_LENGTH + 1];

    /* Validate macro name */
    if (!isValidMacroName(macroName)) {
        fprintf(stderr, "Invalid macro name: %s\n", macroName);
        return false;
    }

    /* Add macro to macro list */
    addMacro(macroList, macroName);

    /* Read lines until "endmacr" is encountered */
    while (fgets(line, sizeof(line), sourceFile)) {
        char end_macro[MAX_MACRO_NAME];

        /* Check for end of macro */
        if (sscanf(line, "%s", end_macro) == 1 && strcmp(end_macro, "endmacr") == 0) {
            return true;
        }

        /* Check for line length exceeding limit */
        if (strlen(line) > MAX_LINE_LENGTH) {
            fprintf(stderr, "Macro line too long: %s\n", line);
            return false;
        }

        /* Add line to macro's line list */
        addMacroLine(*macroList, line);
    }
    return false;
}

/* Add a line to the end of a linked list of code lines */
void addLine(codeLine **head, char *line) {
    codeLine *newLine;

    newLine = malloc(sizeof(codeLine));
    if (newLine == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    newLine->line = malloc(strlen(line) + 1);
    if (newLine->line == NULL) {
        fprintf(stderr, "Memory allocation failed for line\n");
        free(newLine);
        exit(EXIT_FAILURE);
    }

    strcpy(newLine->line, line);
    newLine->next = NULL;

    /* Append new line to the end of the list */
    if (*head == NULL) {
        *head = newLine;
    } else {
        codeLine *current;
        current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newLine;
    }
}

/* Write expanded code lines to an output file */
bool writeExpandedFile(codeLine *codeList, FILE *outputFile, char *outputFileName) {
    codeLine *current;

    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error creating output file: %s\n", outputFileName);
        return false;
    }

    current = codeList;
    while (current != NULL) {
        fprintf(outputFile, "%s", current->line);
        current = current->next;
    }

    fclose(outputFile);
    return true;
}

/* Checks if a given line contains a macro definition. */
bool isMacroLine(char *line) {
    char macro[MAX_MACRO_NAME];
    if (sscanf(line, "%4s", macro) == 1) {
        if (strcmp(macro, "macr") == 0) {
            return true;
        }
    }
    return false;
}

/* Removes the "macr" keyword from the beginning of the line. */
void removeMacrFromLine(char *line) {
    char *temp;

    temp = strchr(line, 'r');
    if (temp) {
        strcpy(line, ++temp);
    }
}

/* Expand macros in the source file and write expanded code to an output file */
bool expandMacros(FILE *sourceFile, FILE *outputFile, char *outputFileName) {
    macro *macroList = NULL;
    codeLine *codeList = NULL;
    char line[MAX_LINE_LENGTH + 2];
    char currentWord[MAX_MACRO_NAME];

    /* Read each line from the source file */
    while (fgets(line, sizeof(line), sourceFile)) {
        int i;
        char macroName[MAX_MACRO_NAME];

        /* Check for line length exceeding limit */
        if (strlen(line) > MAX_LINE_LENGTH + 1) {
            fprintf(stderr, "Error: Line too long: %s\n", line);
            return false;
        }

        /* Read the first word from the line */
        sscanf(line, "%s", currentWord);

        /* Handle macro definition */
        if ( isMacroLine(line) ) {
            removeMacrFromLine(line);
            /* Extract macro name */
            if (sscanf(line , "%s", macroName) != 1) {
                fprintf(stderr, "Invalid macro definition line: %s\n", line);
                return false;
            }

            /* Process and store the macro */
            if (!handleMacro(sourceFile, &macroList, macroName)) {
                fprintf(stderr, "Handling macro failed: %s\n", macroName);
                return false;
            }
        } else {
            /* Expand macros or add regular lines to code list */
            macro *macro = findMacro(macroList, currentWord);
            if (macro) {
                /* Add expanded macro lines to code list */
                for (i = 0; i < macro->lineCount; i++) {
                    addLine(&codeList, macro->lines[i]);
                }
            } else {
                addLine(&codeList, line);
            }
        }
    }

    /* Write expanded code to output file */
    if (!writeExpandedFile(codeList, outputFile, outputFileName)) {
        return false;
    }

    /* Clean up allocated memory */
    freeMacros(macroList);
    freeLines(codeList);
    return true;
}
