#ifndef MACHINE_CODE_H
#define MACHINE_CODE_H

#include "memory.h"
#include "symbolTable.h"

/* Addressing modes */
#define IMMEDIATE 0
#define DIRECT 1
#define INDIRECT_REG 2
#define DIRECT_REG 3

/* Bit positions in the instruction word */
#define S_POSITION 7
#define D_POSITION 3
#define OP_C_POSITION 11
#define VAL_POSITION 3

#define S_REG_POSITION 6
#define D_REG_POSITION 3

/* Bit flags */
#define E_BIT 0
#define R_BIT 1
#define A_BIT 2

/* Operand flags */
#define SOURCE_FLAG 1
#define DEST_FLAG 0

/*
 * Adds an instruction line to the instruction list.
 *
 * This function processes an instruction, including its operation and operands, and adds
 * it to the provided instruction list. It also updates the instruction counter.
 *
 * @param operation The name of the operation (e.g., "mov", "add").
 * @param sourceOperand The source operand for the instruction, if any.
 * @param destOperand The destination operand for the instruction, if any.
 * @param IC The current instruction counter value.
 * @param tail A pointer to the pointer of the last node in the instruction list. This will be updated to point to the newly added node.
 */
void addInstructionLine(char *operation, char *sourceOperand, char *destOperand, int IC, instructionList **tail);

/*
 * Writes the address of a symbol to a 15-bit word.
 *
 * This function converts a symbol's address into a 15-bit representation suitable for machine code.
 *
 * @param symbol The symbol whose address is to be written.
 * @return The 15-bit address of the symbol.
 */
unsigned short writeLabelAddress(Symbol *symbol);

/*
 * Converts a line to a 15-bit word representation.
 *
 * This function processes a line and returns its 15-bit representation for use in machine code.
 *
 * @param line The 15-bit representation of the line.
 * @return The 15-bit word.
 */
unsigned short word15bits(unsigned short line);

#endif /* MACHINE_CODE_H */
