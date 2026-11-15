#ifndef BFS_H
#define BFS_H

#include <stdbool.h>
#include "grid.h"
#include "queue.h"

typedef struct BFS
{
    Grid *grid;
    Queue queue;

    bool isRunning;
    bool isPaused;
    bool isFinished;
    bool pathFound;

    float timer;
    float stepDelay;
} BFS;

void BFS_Init(BFS *bfs);
void BFS_Start(BFS *bfs, Grid *grid);
void BFS_Update(BFS *bfs, float deltaTime);

void BFS_Pause(BFS *bfs);
void BFS_Resume(BFS *bfs);
void BFS_SetSpeed(BFS *bfs, float stepsPerSecond);

bool BFS_IsRunning(const BFS *bfs);
bool BFS_IsPaused(const BFS *bfs);
bool BFS_IsFinished(const BFS *bfs);
bool BFS_PathFound(const BFS *bfs);

float BFS_GetSpeed(const BFS *bfs);

#endif