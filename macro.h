#ifndef MACRO_H
#define MACRO_H

#include <stdbool.h>

/* Structure representing a macro. */
typedef struct macro {
    char *name;               /* The name of the macro */
    char **lines;             /* Array of lines associated with the macro */
    int lineCount;            /* Number of lines in the macro */
    struct macro *next;       /* Pointer to the next macro in the list */
} macro;

/* Structure representing a line of code. */
typedef struct codeLine {
    char *line;               /* The line of code */
    struct codeLine *next;    /* Pointer to the next line in the list */
} codeLine;

/*
 * Adds a new macro to the macro list.
 *
 * This function allocates memory for a new macro and its name, and adds the macro
 * to the beginning of the macro list.
 *
 * @param head A pointer to the pointer to the head of the macro list.
 * @param name The name of the new macro to be added.
 */
void addMacro(macro **head, char *name);

/* Adds a line to a macro's lines list.
 *
 * This function reallocates memory for the macro's lines list to include a new line,
 * and updates the macro's line count.
 *
 * @param macro A pointer to the macro to which the line will be added.
 * @param line The line to be added to the macro.
 */
void addMacroLine(macro *macro, char *line);

/* Finds a macro by its name in the list.
 *
 * This function searches through the macro list and returns a pointer to the macro
 * with the given name.
 *
 * @param head A pointer to the head of the macro list.
 * @param name The name of the macro to find.
 * @return A pointer to the macro with the given name, or NULL if not found.
 */
macro *findMacro(macro *head, char *name);

/* Checks if a macro name is valid.
 *
 * This function determines if a given name is a valid macro name, i.e., it is not
 * an instruction or directive.
 *
 * @param name The name to be checked.
 * @return true if the name is a valid macro name, false otherwise.
 */
bool isValidMacroName(char *name);

/* Frees all macros and their associated memory.
 *
 * This function iterates through the macro list and deallocates memory for each
 * macro's lines and name, as well as the macro structure itself.
 *
 * @param head A pointer to the head of the macro list.
 */
void freeMacros(macro *head);

/* Adds a new line to the end of a code line list.
 *
 * This function reallocates memory for the code lines list to include a new line,
 * and updates the list with the new line.
 *
 * @param head A pointer to the pointer to the head of the code line list.
 * @param line The line of code to be added.
 */
void addCodeLine(codeLine **head, char *line);

/* Frees all code lines and their associated memory.
 *
 * This function iterates through the code line list and deallocates memory for each
 * line and the code line structure itself.
 *
 * @param head A pointer to the head of the code line list.
 */
void freeLines(codeLine *head);

#endif
