#ifndef DFS_H
#define DFS_H

#include <stdbool.h>
#include "grid.h"
#include "stack.h"

typedef struct DFS
{
    Grid *grid;
    Stack stack;

    bool isRunning;
    bool isPaused;
    bool isFinished;
    bool pathFound;

    float timer;
    float stepDelay;

} DFS;

void DFS_Init(DFS *dfs);
void DFS_Start(DFS *dfs, Grid *grid);
void DFS_Update(DFS *dfs, float deltaTime);

void DFS_Pause(DFS *dfs);
void DFS_Resume(DFS *dfs);
void DFS_SetSpeed(DFS *dfs, float stepsPerSecond);

bool DFS_IsRunning(const DFS *dfs);
bool DFS_IsPaused(const DFS *dfs);
bool DFS_IsFinished(const DFS *dfs);
bool DFS_PathFound(const DFS *dfs);

float DFS_GetSpeed(const DFS *dfs);

#endif