#include "grid.h"
#include <stddef.h>


void Grid_Init(Grid *grid)
{
    // Initialize all nodes
    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            Node_Init(&grid->nodes[row][col], row, col);
        }
    }

    // Set default start and goal positions
    grid->startNode = &grid->nodes[7][2];
    grid->goalNode = &grid->nodes[7][17];

    grid->startNode->isStart = true;
    grid->goalNode->isGoal = true;
}


void Grid_Reset(Grid *grid)
{
    Grid_Init(grid);
}


void Grid_ClearWallsAndSearch(Grid *grid)
{
    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            Node *node = &grid->nodes[row][col];

            Node_ResetSearch(node);
        }
    }
}


Node *Grid_GetNode(Grid *grid, int row, int col)
{
    if (!Grid_IsInside(row, col))
    {
        return NULL;
    }

    return &grid->nodes[row][col];
}


bool Grid_IsInside(int row, int col)
{
    return row >= 0 &&
           row < GRID_ROWS &&
           col >= 0 &&
           col < GRID_COLS;
}


void Grid_SetStart(Grid *grid, int row, int col)
{
    Node *node = Grid_GetNode(grid, row, col);

    if (node == NULL || node->isGoal || node->state == NODE_WALL)
    {
        return;
    }

    grid->startNode->isStart = false;

    node->isStart = true;
    grid->startNode = node;

    Node_ResetSearch(node);
}


void Grid_SetGoal(Grid *grid, int row, int col)
{
    Node *node = Grid_GetNode(grid, row, col);

    if (node == NULL || node->isStart || node->state == NODE_WALL)
    {
        return;
    }

    grid->goalNode->isGoal = false;

    node->isGoal = true;
    grid->goalNode = node;

    Node_ResetSearch(node);
}


void Grid_ToggleWall(Grid *grid, int row, int col)
{
    Node *node = Grid_GetNode(grid, row, col);

    if (node == NULL || node->isStart || node->isGoal)
    {
        return;
    }

    if (node->state == NODE_WALL)
    {
        node->state = NODE_EMPTY;
    }
    else
    {
        node->state = NODE_WALL;
    }

    Node_ResetSearch(node);
}


void Grid_EraseCell(Grid *grid, int row, int col)
{
    Node *node = Grid_GetNode(grid, row, col);

    if (node == NULL || node->isStart || node->isGoal)
    {
        return;
    }

    node->state = NODE_EMPTY;

    Node_ResetSearch(node);
}