#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdbool.h>
#include "grid.h"

typedef struct Dijkstra
{
    Grid *grid;

    bool isRunning;
    bool isPaused;
    bool isFinished;
    bool pathFound;

    float timer;
    float stepDelay;

} Dijkstra;

void Dijkstra_Init(Dijkstra *dijkstra);
void Dijkstra_Start(Dijkstra *dijkstra, Grid *grid);
void Dijkstra_Update(Dijkstra *dijkstra, float deltaTime);

void Dijkstra_Pause(Dijkstra *dijkstra);
void Dijkstra_Resume(Dijkstra *dijkstra);
void Dijkstra_SetSpeed(Dijkstra *dijkstra, float stepsPerSecond);

bool Dijkstra_IsRunning(const Dijkstra *dijkstra);
bool Dijkstra_IsPaused(const Dijkstra *dijkstra);
bool Dijkstra_IsFinished(const Dijkstra *dijkstra);
bool Dijkstra_PathFound(const Dijkstra *dijkstra);

float Dijkstra_GetSpeed(const Dijkstra *dijkstra);

#endif