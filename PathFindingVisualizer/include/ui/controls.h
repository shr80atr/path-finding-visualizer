#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdbool.h>

#include "bfs.h"
#include "dfs.h"
#include "dijkstra.h"
#include "astar.h"

typedef enum AlgorithmType
{
    ALGORITHM_BFS,
    ALGORITHM_DFS,
    ALGORITHM_DIJKSTRA,
    ALGORITHM_ASTAR
} AlgorithmType;

bool Controls_HandleInput(
    BFS *bfs,
    DFS *dfs,
    Dijkstra *dijkstra,
    AStar *astar,
    AlgorithmType *selectedAlgorithm
);

void Controls_Draw(
    const BFS *bfs,
    const DFS *dfs,
    const Dijkstra *dijkstra,
    const AStar *astar,
    AlgorithmType selectedAlgorithm
);

#endif