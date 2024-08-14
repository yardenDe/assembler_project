#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "machineCode.h"
#include "processorUtils.h"
#include "symbolTable.h"


/*
 * This function clear the most significant bit of a
 * 16-bit number, effectively limiting it to a 15-bit value.
 */
unsigned short word15bits(unsigned short line) {
    unsigned short cmp;
    cmp = (1 << 15);
    cmp = ~cmp;
    return line & cmp;
}

/*
 * Convert a string representation of a number to an unifned short,
 * handling multiple digits and a leading minus sign
 */
unsigned short convertStringToShort(const char *str) {
    unsigned short result = 0;
    int sign = 1;

    /* Skip non-digit and non-minus characters */
    while (*str && !isdigit(*str) && *str != '-') {
        str++;
    }

    /* Check if the number is negative */
    if (*str == '-') {
        sign = -1;
        str++;
    }

    /* Convert each digit to the corresponding integer value */
    while (*str) {
        if (isdigit(*str)) {
            result = result * 10 + (*str - '0');
        }
        str++;
    }
    return result * sign;
}

/* Sets a bit at a specific position. */
unsigned short setBit(int position) {
    return 1 << (position);
}

/* Shifts bits to the left by a given position. */
unsigned short shiftBits(unsigned short line, int position) {
    return line << (position);
}

/* Retrieves the opcode value corresponding to a given operation name. */
unsigned short getOpCode(const char *name) {
    unsigned short code = OPERATIONS+1; /* Invalid value to validate return value */

    if (strcmp(name, "mov") == 0) { code = 0; } else if (strcmp(name, "cmp") == 0) { code = 1; } else if (
        strcmp(name, "add") == 0) { code = 2; } else if (strcmp(name, "sub") == 0) { code = 3; } else if (
        strcmp(name, "lea") == 0) { code = 4; } else if (strcmp(name, "clr") == 0) { code = 5; } else if (
        strcmp(name, "not") == 0) { code = 6; } else if (strcmp(name, "inc") == 0) { code = 7; } else if (
        strcmp(name, "dec") == 0) { code = 8; } else if (strcmp(name, "jmp") == 0) { code = 9; } else if (
        strcmp(name, "bne") == 0) { code = 10; } else if (strcmp(name, "red") == 0) { code = 11; } else if (
        strcmp(name, "prn") == 0) { code = 12; } else if (strcmp(name, "jsr") == 0) { code = 13; } else if (
        strcmp(name, "rts") == 0) { code = 14; } else if (strcmp(name, "stop") == 0) { code = 15; }

    if (code > OPERATIONS) {
        fprintf(stderr, "Error - invalid opcode name\n");
    }
    code <<= OP_C_POSITION;
    return code;
}


/* Writes a register operand value, adjusting bit positions for source or destination. */
unsigned short writeRegister(char *operand, int isSource) {
    unsigned short line = convertStringToShort(operand);

    if (isSource) {
        line = shiftBits(line, S_REG_POSITION);
    } else {
        line = shiftBits(line, D_REG_POSITION);
    }
    line |= setBit(A_BIT);
    return line;
}

/* Writes an operand line based on its addressing mode. */
unsigned short writeOperandLine(char *operand, int mode, int sourceFlag) {
    unsigned short line = 0;

    /* For DIRECT mode, the value will be resolved in the second pass */
    if (mode == DIRECT) {
        return line;
    }
    if (mode == INDIRECT_REG || mode == DIRECT_REG) {
        line = convertStringToShort(operand);
        if (sourceFlag) {
            line = shiftBits(line, S_REG_POSITION);
        } else {
            line = shiftBits(line, D_REG_POSITION);
        }
    } else if (mode == IMMEDIATE) {
        line = convertStringToShort(operand); /* Skip the '#' character */
        line = shiftBits(line, VAL_POSITION);
    }
    line |= setBit(A_BIT);
    line = word15bits(line);
    return line;
}

/* Determines the addressing mode of an operand. */
int addressingMode(char *operand) {
    if (operand == NULL) {
        return -1;
    }
    if (*operand == '#') { return IMMEDIATE; }
    if (isInDirectRegister(operand)) { return INDIRECT_REG; }
    if (isDirectRegister(operand)) { return DIRECT_REG; }
    return DIRECT; /* Operand already validated - must be symbol */
}

/* Creates the binary representation of an instruction line. */
unsigned short writeOpCode(char *operation, int sourceMode, int destMode) {
    unsigned short line = getOpCode(operation);

    if (sourceMode >= 0) {
        line |= setBit(S_POSITION + sourceMode);
    }
    if (destMode >= 0) {
        line |= setBit(D_POSITION + destMode);
    }

    line |= setBit(A_BIT);

    return line;
}

/* Adds an instruction line to the instruction list. */
void addInstructionLine(char *operation, char *sourceOperand, char *destOperand, int IC, instructionList **tail) {
    int sourceMode, destMode;
    unsigned short line = 0;

    sourceMode = addressingMode(sourceOperand);
    destMode = addressingMode(destOperand);

    line = writeOpCode(operation, sourceMode, destMode);
    addToInstructionList(tail, NULL, IC, line);

    if (sourceOperand != NULL) {
        if (isRegister(sourceOperand) && isRegister(destOperand)) {
            line = writeRegister(sourceOperand, SOURCE_FLAG);
            line |= writeRegister(destOperand, DEST_FLAG);

            addToInstructionList(tail, NULL, ++IC, line);
        } else {
            line = writeOperandLine(sourceOperand, sourceMode, SOURCE_FLAG);

            if (sourceMode != DIRECT) { sourceOperand = NULL; }
            addToInstructionList(tail, sourceOperand, ++IC, line);

            line = writeOperandLine(destOperand, destMode, DEST_FLAG);

            if (destMode != DIRECT) { destOperand = NULL; }
            addToInstructionList(tail, destOperand, ++IC, line);
        }
    } else if (destOperand != NULL) {
        line = writeOperandLine(destOperand, destMode, DEST_FLAG);
        if (destMode != DIRECT) { destOperand = NULL; }
        addToInstructionList(tail, destOperand, ++IC, line);
    }
}

/* Writes the address of a label into a binary instruction. */
unsigned short writeLabelAddress(Symbol *symbol) {
    unsigned short line;
    line = (unsigned short) symbol->address;
    line = shiftBits(line, VAL_POSITION);
    if (strcmp(symbol->type, "external") == 0) {
        line |= setBit(E_BIT);
    } else {
        line |= setBit(R_BIT);
    }
    return line;
}