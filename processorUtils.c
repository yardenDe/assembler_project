#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"


/* Check if the line is an instruction line */
bool isInstructionLine(char *line) {
    char word[MAX_LINE_LENGTH];
    if (sscanf(line, "%s", word)) {
        return isInstruction(word);
    }
    return false;
}

/* Check if the line is a directive line */
bool isDirectiveLine(char *line) {
    char word[MAX_LINE_LENGTH];
    if (sscanf(line, "%s", word)) {
        return isDirective(word);
    }
    return false;
}

/* Check if the line is a comment line (starts with ';') */
bool isNoteLine(char *s) {
    return s[0] == ';';
}

/* Check if the line is empty (contains only whitespace) */
bool isEmptyLine(char *s) {
    int i;
    for (i = 0; i < strlen(s); i++) {
        if (!isspace(s[i])) {
            return false;
        }
    }
    return true;
}

/* Array of registers names */
char *registers[REGISTERS] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
};

/* Validate direct addressing method */
bool isDirectRegister(char *s) {
    int i;
    for (i = 0; i < REGISTERS; i++) {
        if (strcmp(s, registers[i]) == 0) {
            return true;
        }
    }
    return false;
}

/* Validate indirect addressing method */
bool isInDirectRegister(char *s) {
    if (s[0] == '*') {
        return isDirectRegister(s + 1);
    }
    return false;
}

/* Validate register (direct or indirect) */
bool isRegister(char *s) {
    return isDirectRegister(s) || isInDirectRegister(s);
}


/* Check if the line contains a symbol */
bool isSymbol(char *line, char *symbol) {
    sscanf(line, "%s", symbol);

    if (symbol[strlen(symbol) - 1] == ':') {
        symbol = strtok(symbol, ":");  /* For adding symbol to symbolTable without the colon */
        return true;
    }
    return false;
}

/* Check if the line is a valid label (not an instruction or directive) */
bool isValidLabel(char *s) {
    return !isInstruction(s) && !isDirective(s);
}

/* Check if the line is a data or string directive (.data, .string) */
bool isDataOrString(char *s) {
    char word[MAX_LINE_LENGTH];
    if (sscanf(s, "%s", word)) {
        if (strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0) {
            return true;
        }
    }
    return false;
}

/* Check if the line is an external directive (.extern) */
bool isExternalLine(const char *s) {
    char word[MAX_LINE_LENGTH];
    if (sscanf(s, "%s", word) == 1) {
        return strcmp(word, ".extern") == 0;
    }
    return false;
}

/* Validate numeric character with possible '+' or '-' sign */
bool isNumeric(const char *str) {
    if (str == NULL || *str == '\0') {
        return false;
    }
    if (*str == '+' || *str == '-') {
        str++;
    }
    while (*str) {
        if (!isdigit(*str) && !isspace(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

/* Skip left white spaces in line */
void ignoreLeftWhiteSpaces(char *s) {
    char *temp;
    if (s == NULL || *s == '\0') {
        return;
    }

    temp = s;
    while (isspace(*temp)) { temp++; }
    strcpy(s, temp);
}

/* Remove symbol from line */
void removeSymbolFromLine(char *line) {
    char *temp;
    temp = strchr(line, ':');
    strcpy(line, ++temp); /* Skip the colon */
}

/* Remove directive from the line (e.g., ".data") */
void removeDirectiveFromLine(char *line) {
    char *temp;
    temp = strchr(line, '.');
    while (!isspace(*temp)) { temp++; }
    ignoreLeftWhiteSpaces(temp);
    strcpy(line, temp);
}

/* Returns number of operands that given operation requires */
int estimatedOperands(char *name) {
    if (strcmp(name, "mov") == 0 || strcmp(name, "cmp") == 0 || strcmp(name, "add") == 0 || strcmp(name, "sub") == 0 ||
        strcmp(name, "lea") == 0) {
        return 2;
    }
    if (strcmp(name, "clr") == 0 || strcmp(name, "not") == 0 || strcmp(name, "inc") == 0 || strcmp(name, "dec") == 0 ||
        strcmp(name, "jmp") == 0 || strcmp(name, "bne") == 0 || strcmp(name, "red") == 0 || strcmp(name, "prn") == 0 ||
        strcmp(name, "jsr") == 0) {
        return 1;
    }
    return 0;
}

/* Count the number of commas in a line */
int expectedCommas(char *line) {
    int commas = 0;
    char *temp = strchr(line, ',');
    while (temp != NULL) {
        commas++;
        temp = strchr(temp + 1, ',');
    }
    return commas;
}


/* Validate and format operands in a line */
bool validateOperands(int estOperands, char *line, char **sourceOperand, char **destOperand) {
    int operands = 0;
    char *token;

    token = strtok(line, ",\n");

    while (token) {
        ignoreLeftWhiteSpaces(token);

        if (!isEmptyLine(token + strlen(token))) {
            fprintf(stderr, "Error - Comma expected\n");
            return false;
        }

        operands++;

        if (estOperands == 1) {
            *destOperand = token;
        } else {
            if (operands == 1) {
                *sourceOperand = token;
            } else {
                *destOperand = token;
                ignoreLeftWhiteSpaces(*destOperand);
            }
        }
        token = strtok(NULL, ",\n");
    }
    if (operands > estOperands) {
        fprintf(stderr, "Error - too many operands\n");
        return false;
    }

    return operands == estOperands;
}
