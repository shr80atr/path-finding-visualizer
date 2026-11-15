#ifndef ASTAR_H
#define ASTAR_H

#include <stdbool.h>
#include "grid.h"

typedef struct AStar
{
    Grid *grid;

    bool isRunning;
    bool isPaused;
    bool isFinished;
    bool pathFound;

    float timer;
    float stepDelay;

} AStar;

// Initialization
void AStar_Init(AStar *astar);

// Start algorithm
void AStar_Start(AStar *astar, Grid *grid);

// Update algorithm
void AStar_Update(AStar *astar, float deltaTime);

// Pause and Resume
void AStar_Pause(AStar *astar);
void AStar_Resume(AStar *astar);

// Speed control
void AStar_SetSpeed(AStar *astar, float stepsPerSecond);
float AStar_GetSpeed(const AStar *astar);

// Status
bool AStar_IsRunning(const AStar *astar);
bool AStar_IsPaused(const AStar *astar);
bool AStar_IsFinished(const AStar *astar);
bool AStar_PathFound(const AStar *astar);

#endif