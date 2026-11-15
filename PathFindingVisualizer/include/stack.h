#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "node.h"
#include "utils/constants.h"

#define STACK_CAPACITY (GRID_ROWS * GRID_COLS)

typedef struct Stack
{
    Node *items[STACK_CAPACITY];
    int top;
} Stack;

void Stack_Init(Stack *stack);
bool Stack_IsEmpty(const Stack *stack);
bool Stack_IsFull(const Stack *stack);
bool Stack_Push(Stack *stack, Node *node);
Node *Stack_Pop(Stack *stack);

#endif