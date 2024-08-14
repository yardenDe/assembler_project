#ifndef MEMORY_H
#define MEMORY_H



#define MEMORY_LINE 15

/* Structure representing a node in the data list. */
typedef struct dataList {
    int count;                /* Number of items in the data list */
    unsigned short line;      /* Data line value */
    struct dataList *next;    /* Pointer to the next node in the data list */
} dataList;

/* Structure representing a node in the instruction list. */
typedef struct instructionList {
    int count;                /* Number of items in the instruction list */
    char *symbolOperand;      /* Pointer to the symbol operand */
    unsigned short line;      /* Instruction line value */
    struct instructionList *next; /* Pointer to the next node in the instruction list */
} instructionList;

/*
 * Initializes a new data list.
 *
 * This function allocates and initializes a new data list.
 *
 * @return A pointer to the newly created data list.
 */
dataList *initDataList();

/*
 * Allocates and initializes a new data list node.
 *
 * This function allocates memory for a new data list node and initializes it.
 *
 * @return A pointer to the newly created data list node.
 */
dataList *allocateNewDNode();

/*
 * Adds a new node to the end of the data list.
 *
 * This function creates a new data node and appends it to the end of the data list.
 *
 * @param tail A pointer to the pointer to the last node in the data list.
 * @param count The count value for the new node.
 * @param line The line value for the new node.
 */
void addToDataList(dataList **tail, int count, unsigned short line);

/*
 * Frees all nodes in the data list.
 *
 * This function releases the memory allocated for all nodes in the data list.
 *
 * @param list A pointer to the head of the data list to be freed.
 */
void freeDataList(dataList *list);

/*
 * Initializes a new instruction list.
 *
 * This function allocates and initializes a new instruction list.
 *
 * @return A pointer to the newly created instruction list.
 */
instructionList *initInstructionList();

/*
 * Allocates and initializes a new instruction list node.
 *
 * This function allocates memory for a new instruction list node and initializes it.
 *
 * @return A pointer to the newly created instruction list node.
 */
instructionList *allocateNewINode();

/*
 * Adds a new node to the end of the instruction list.
 *
 * This function creates a new instruction node and appends it to the end of the instruction list.
 *
 * @param tail A pointer to the pointer to the last node in the instruction list.
 * @param operand A string representing the symbol operand for the new node.
 * @param count The count value for the new node.
 * @param line The line value for the new node.
 */
void addToInstructionList(instructionList **tail, char *operand, int count, unsigned short line);

/*
 * Frees all nodes in the instruction list.
 *
 * This function releases the memory allocated for all nodes in the instruction list.
 *
 * @param list A pointer to the head of the instruction list to be freed.
 */
void freeInstructionList(instructionList *list);

#endif

