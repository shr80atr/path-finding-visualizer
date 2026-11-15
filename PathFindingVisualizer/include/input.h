#ifndef INPUT_H
#define INPUT_H

#include "grid.h"

// Different grid editing modes
typedef enum EditMode
{
    EDIT_WALL,
    EDIT_START,
    EDIT_GOAL,
    EDIT_ERASE

} EditMode;


// Handle mouse and keyboard input
void Input_Handle(Grid *grid, EditMode *mode);

#endif