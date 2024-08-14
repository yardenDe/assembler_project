
#ifndef PROCESSORUTILS_H
#define PROCESSORUTILS_H

#include <stdbool.h>

/* Check if the line is an instruction line.
 *
 * This function checks if the given line starts with an instruction.
 *
 * @param line The line to check.
 * @return true if the line is an instruction line, false otherwise.
 */
bool isInstructionLine(char *line);

/* Check if the line is a directive line.
 *
 * This function checks if the given line starts with a directive.
 *
 * @param line The line to check.
 * @return true if the line is a directive line, false otherwise.
 */
bool isDirectiveLine(char *line);

/* Check if the line is a comment line (starts with ';').
 *
 * This function checks if the given line starts with a comment character.
 *
 * @param s The line to check.
 * @return true if the line is a comment line, false otherwise.
 */
bool isNoteLine(char *s);

/* Check if the line is empty (contains only whitespace).
 *
 * This function checks if the given line contains only whitespace characters.
 *
 * @param s The line to check.
 * @return true if the line is empty, false otherwise.
 */
bool isEmptyLine(char *s);

/* Validate direct addressing method.
 *
 * This function checks if the given string represents a valid direct register.
 *
 * @param s The string to check.
 * @return true if the string represents a valid direct register, false otherwise.
 */
bool isDirectRegister(char *s);

/* Validate indirect addressing method.
 *
 * This function checks if the given string represents a valid indirect register.
 *
 * @param s The string to check.
 * @return true if the string represents a valid indirect register, false otherwise.
 */
bool isInDirectRegister(char *s);

/* Validate register (direct or indirect).
 *
 * This function checks if the given string represents a valid register, either direct or indirect.
 *
 * @param s The string to check.
 * @return true if the string represents a valid register, false otherwise.
 */
bool isRegister(char *s);

/* Check if the line contains a symbol.
 *
 * This function checks if the given line contains a symbol and extracts it.
 *
 * @param line The line to check.
 * @param symbol The symbol extracted from the line.
 * @return true if a symbol is found, false otherwise.
 */
bool isSymbol(char *line, char *symbol);

/* Check if the line is a valid label (not an instruction or directive).
 *
 * This function checks if the given line is a valid label that is neither an instruction nor a directive.
 *
 * @param s The line to check.
 * @return true if the line is a valid label, false otherwise.
 */
bool isValidLabel(char *s);

/* Check if the line is a data or string directive (.data, .string).
 *
 * This function checks if the given line is a data or string directive.
 *
 * @param s The line to check.
 * @return true if the line is a data or string directive, false otherwise.
 */
bool isDataOrString(char *s);

/* Check if the line is an external directive (.extern).
 *
 * This function checks if the given line is an external directive.
 *
 * @param s The line to check.
 * @return true if the line is an external directive, false otherwise.
 */
bool isExternalLine(const char *s);

/* Validate numeric character with possible '+' or '-' sign.
 *
 * This function checks if the given string represents a valid numeric value.
 *
 * @param str The string to check.
 * @return true if the string represents a valid numeric value, false otherwise.
 */
bool isNumeric(const char *str);

/* Skip left white spaces in line.
 *
 * This function removes leading whitespace characters from the given line.
 *
 * @param s The line to process.
 */
void ignoreLeftWhiteSpaces(char *s);

/* Remove symbol from line.
 *
 * This function removes the symbol from the beginning of the given line.
 *
 * @param line The line to process.
 */
void removeSymbolFromLine(char *line);

/* Remove directive from the line (e.g., ".data").
 *
 * This function removes the directive from the beginning of the given line.
 *
 * @param line The line to process.
 */
void removeDirectiveFromLine(char *line);

/* Returns number of operands that given operation requires.
 *
 * This function returns the number of operands required by the given operation.
 *
 * @param name The name of the operation.
 * @return The number of operands required.
 */
int estimatedOperands(char *name);

/* Count the number of commas in a line.
 *
 * This function counts the number of commas in the given line.
 *
 * @param line The line to process.
 * @return The number of commas in the line.
 */
int expectedCommas(char *line);

/* Validate and format operands in a line.
 *
 * This function validates and formats operands in the given line based on the expected number of operands.
 *
 * @param estOperands The expected number of operands.
 * @param line The line containing operands.
 * @param sourceOperand A pointer to store the source operand.
 * @param destOperand A pointer to store the destination operand.
 * @return true if the operands are valid, false otherwise.
 */
bool validateOperands(int estOperands, char *line, char **sourceOperand, char **destOperand);



#endif
