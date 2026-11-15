#include "queue.h"
#include <stddef.h>

void Queue_Init(Queue *queue)
{
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}

bool Queue_IsEmpty(const Queue *queue)
{
    return queue->size == 0;
}

bool Queue_IsFull(const Queue *queue)
{
    return queue->size == QUEUE_CAPACITY;
}

bool Queue_Enqueue(Queue *queue, Node *node)
{
    if (Queue_IsFull(queue))
    {
        return false;
    }

    queue->items[queue->rear] = node;

    queue->rear = (queue->rear + 1) % QUEUE_CAPACITY;

    queue->size++;

    return true;
}

Node *Queue_Dequeue(Queue *queue)
{
    if (Queue_IsEmpty(queue))
    {
        return NULL;
    }

    Node *node = queue->items[queue->front];

    queue->front = (queue->front + 1) % QUEUE_CAPACITY;

    queue->size--;

    return node;
}