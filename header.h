#ifndef HEADER_H
#define HEADER_H

#include <stdbool.h>

#define MAX_LINE_LENGTH 80
#define MAX_MACRO_NAME 31
#define OPERATIONS 16
#define INSTRUCTIONS 4
#define REGISTERS 8
#define OPERATIONS_LENGTH 4
#define MAX_LABEL_LENGTH 31

#endif

bool isInstruction(char *name);
bool isDirective(char *name);





