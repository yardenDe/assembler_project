#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include <stdbool.h>

/*
 * Expands macros in the source file and writes the result to the output file.
 *
 * This function processes the source file to replace macros with their
 * definitions and writes the expanded content to the output file.
 *
 * @param source_file A pointer to the source file to be processed. This file should
 *                    contain the code with macros to be expanded.
 * @param output_file A pointer to the output file where the expanded code will be written.
 * @param file_name   A string containing the name of the source file. This is used for
 *                    error reporting or debugging purposes.
 *
 * @return Returns `true` if the expansion was successful and the output file was written
 *         without errors. Returns `false` otherwise.
 */
bool expandMacros(FILE *source_file, FILE *output_file, char *);

#endif
