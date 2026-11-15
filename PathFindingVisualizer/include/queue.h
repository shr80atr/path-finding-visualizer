#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "node.h"
#include "utils/constants.h"

#define QUEUE_CAPACITY (GRID_ROWS * GRID_COLS)

typedef struct Queue
{
    Node *items[QUEUE_CAPACITY];

    int front;
    int rear;
    int size;

} Queue;

// Initialize queue
void Queue_Init(Queue *queue);

// Check whether queue is empty
bool Queue_IsEmpty(const Queue *queue);

// Check whether queue is full
bool Queue_IsFull(const Queue *queue);

// Add an item
bool Queue_Enqueue(Queue *queue, Node *node);

// Remove an item
Node *Queue_Dequeue(Queue *queue);

#endif