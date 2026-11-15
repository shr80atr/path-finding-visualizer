#ifndef GRID_H
#define GRID_H

#include <stdbool.h>

#include "node.h"
#include "utils/constants.h"


typedef struct Grid
{
    Node nodes[GRID_ROWS][GRID_COLS];

    Node *startNode;
    Node *goalNode;

} Grid;


// Initialize the grid
void Grid_Init(Grid *grid);

// Reset the entire grid
void Grid_Reset(Grid *grid);

// Clear walls and search states
void Grid_ClearWallsAndSearch(Grid *grid);

// Get a node by row and column
Node *Grid_GetNode(Grid *grid, int row, int col);

// Check whether a position is inside the grid
bool Grid_IsInside(int row, int col);

// Set the start node
void Grid_SetStart(Grid *grid, int row, int col);

// Set the goal node
void Grid_SetGoal(Grid *grid, int row, int col);

// Toggle a wall
void Grid_ToggleWall(Grid *grid, int row, int col);

// Erase a cell
void Grid_EraseCell(Grid *grid, int row, int col);

#endif