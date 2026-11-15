#ifndef NODE_H
#define NODE_H

#include <stdbool.h>

// Possible states of a node
typedef enum NodeState
{
    NODE_EMPTY = 0,
    NODE_WALL,
    NODE_VISITED,
    NODE_FRONTIER,
    NODE_PATH

} NodeState;


// Node structure
typedef struct Node
{
    int row;
    int col;

    NodeState state;

    bool isStart;
    bool isGoal;

    float distance;
    float heuristic;

    struct Node *previousNode;

} Node;


// Initialize a node
void Node_Init(Node *node, int row, int col);

// Reset search-related properties
void Node_ResetSearch(Node *node);

#endif