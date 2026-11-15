#include "stack.h"
#include <stddef.h>

void Stack_Init(Stack *stack)
{
    stack->top = -1;
}

bool Stack_IsEmpty(const Stack *stack)
{
    return stack->top == -1;
}

bool Stack_IsFull(const Stack *stack)
{
    return stack->top == STACK_CAPACITY - 1;
}

bool Stack_Push(Stack *stack, Node *node)
{
    if (Stack_IsFull(stack))
        return false;

    stack->items[++stack->top] = node;

    return true;
}

Node *Stack_Pop(Stack *stack)
{
    if (Stack_IsEmpty(stack))
        return NULL;

    return stack->items[stack->top--];
}