#include "node.h"
#include <stddef.h>

void Node_Init(Node *node, int row, int col)
{
    node->row = row;
    node->col = col;

    node->state = NODE_EMPTY;

    node->isStart = false;
    node->isGoal = false;

    node->distance = -1.0f;
    node->heuristic = 0.0f;

    node->previousNode = NULL;
}


void Node_ResetSearch(Node *node)
{
    // Preserve walls and clear other search states
    if (node->state != NODE_WALL)
    {
        node->state = NODE_EMPTY;
    }

    node->distance = -1.0f;
    node->heuristic = 0.0f;

    node->previousNode = NULL;
}