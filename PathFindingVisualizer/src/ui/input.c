#include "input.h"
#include "utils/constants.h"

#include "raylib.h"


// Convert mouse position to grid coordinates
static bool GetGridPosition(int *row, int *col)
{
    Vector2 mousePosition = GetMousePosition();

    int x = (int)mousePosition.x;
    int y = (int)mousePosition.y;

    // Check whether mouse is inside the grid
    if (x < GRID_X || y < GRID_Y)
    {
        return false;
    }

    *col = (x - GRID_X) / CELL_SIZE;
    *row = (y - GRID_Y) / CELL_SIZE;

    return Grid_IsInside(*row, *col);
}


// Handle keyboard input
static void HandleKeyboard(EditMode *mode)
{
    if (IsKeyPressed(KEY_W))
    {
        *mode = EDIT_WALL;
    }

    if (IsKeyPressed(KEY_S))
    {
        *mode = EDIT_START;
    }

    if (IsKeyPressed(KEY_G))
    {
        *mode = EDIT_GOAL;
    }

    if (IsKeyPressed(KEY_E))
    {
        *mode = EDIT_ERASE;
    }
}


// Handle mouse input
static void HandleMouse(Grid *grid, EditMode mode)
{
    int row;
    int col;

    if (!GetGridPosition(&row, &col))
    {
        return;
    }

    // Left mouse button
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        switch (mode)
        {
            case EDIT_WALL:
                Grid_ToggleWall(grid, row, col);
                break;

            case EDIT_START:
                Grid_SetStart(grid, row, col);
                break;

            case EDIT_GOAL:
                Grid_SetGoal(grid, row, col);
                break;

            case EDIT_ERASE:
                Grid_EraseCell(grid, row, col);
                break;
        }
    }

    // Right mouse button erases walls
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        Grid_EraseCell(grid, row, col);
    }
}


// Main input handler
void Input_Handle(Grid *grid, EditMode *mode)
{
    HandleKeyboard(mode);

    HandleMouse(grid, *mode);
}