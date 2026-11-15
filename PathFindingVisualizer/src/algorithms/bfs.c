#include "bfs.h"

#include <stddef.h>

// Check whether a node can be visited
static bool IsWalkable(const Node *node)
{
    return node != NULL && node->state != NODE_WALL;
}

// Add a valid neighbor to the queue
static void VisitNeighbor(
    BFS *bfs,
    Node *current,
    int row,
    int col)
{
    Node *neighbor = Grid_GetNode(bfs->grid, row, col);

    if (!IsWalkable(neighbor))
    {
        return;
    }

    // Already discovered
    if (neighbor->distance >= 0)
    {
        return;
    }

    neighbor->distance = current->distance + 1;
    neighbor->previousNode = current;
    neighbor->state = NODE_FRONTIER;

    Queue_Enqueue(&bfs->queue, neighbor);
}

// Reconstruct the final path
static void ReconstructPath(BFS *bfs)
{
    Node *current = bfs->grid->goalNode;

    while (current != NULL && current != bfs->grid->startNode)
    {
        current->state = NODE_PATH;
        current = current->previousNode;
    }
}

// Execute one BFS step
static void BFS_Step(BFS *bfs)
{
    if (Queue_IsEmpty(&bfs->queue))
    {
        bfs->isRunning = false;
        bfs->isFinished = true;
        bfs->isPaused = false;
        bfs->pathFound = false;
        return;
    }

    Node *current = Queue_Dequeue(&bfs->queue);

    if (current == NULL)
    {
        bfs->isPaused = false;
        return;
    }

    current->state = NODE_VISITED;

    // Check whether the goal has been reached
    if (current == bfs->grid->goalNode)
    {
        ReconstructPath(bfs);

        bfs->isRunning = false;
        bfs->isFinished = true;
        bfs->isPaused = false;
        bfs->pathFound = true;

        return;
    }

    // Up
    VisitNeighbor(
        bfs,
        current,
        current->row - 1,
        current->col);

    // Down
    VisitNeighbor(
        bfs,
        current,
        current->row + 1,
        current->col);

    // Left
    VisitNeighbor(
        bfs,
        current,
        current->row,
        current->col - 1);

    // Right
    VisitNeighbor(
        bfs,
        current,
        current->row,
        current->col + 1);

    // No more nodes to explore
    if (Queue_IsEmpty(&bfs->queue))
    {
        bfs->isRunning = false;
        bfs->isFinished = true;
        bfs->pathFound = false;
    }
}

// Initialize BFS
void BFS_Init(BFS *bfs)
{
    bfs->grid = NULL;
    Queue_Init(&bfs->queue);

    bfs->isRunning = false;
    bfs->isPaused = false;
    bfs->isFinished = false;
    bfs->pathFound = false;

    bfs->timer = 0.0f;
    bfs->stepDelay = 0.08f;
}

// Start BFS
void BFS_Start(BFS *bfs, Grid *grid)
{
    if (bfs == NULL || grid == NULL ||
        grid->startNode == NULL || grid->goalNode == NULL)
        return;

    Grid_ClearWallsAndSearch(grid);

    bfs->grid = grid;
    Queue_Init(&bfs->queue);

    bfs->timer = 0.0f;
    bfs->isRunning = true;
    bfs->isPaused = false;
    bfs->isFinished = false;
    bfs->pathFound = false;

    Node *start = grid->startNode;

    start->distance = 0.0f;
    start->state = NODE_FRONTIER;

    Queue_Enqueue(&bfs->queue, start);
}

// Update BFS animation
void BFS_Update(BFS *bfs, float deltaTime)
{
    if (bfs == NULL || !bfs->isRunning || bfs->isPaused)
        return;

    bfs->timer += deltaTime;

    while (bfs->timer >= bfs->stepDelay && bfs->isRunning)
    {
        bfs->timer -= bfs->stepDelay;
        BFS_Step(bfs);
    }
}

void BFS_Pause(BFS *bfs)
{
    if (bfs == NULL || !bfs->isRunning || bfs->isFinished)
        return;

    bfs->isPaused = true;
}

void BFS_Resume(BFS *bfs)
{
    if (bfs == NULL || !bfs->isRunning || bfs->isFinished)
        return;

    bfs->isPaused = false;
}

void BFS_SetSpeed(BFS *bfs, float stepsPerSecond)
{
    if (bfs == NULL)
        return;

    // محدود کردن سرعت به بازه 1 تا 30 مرحله در ثانیه
    if (stepsPerSecond < 1.0f)
        stepsPerSecond = 1.0f;

    if (stepsPerSecond > 30.0f)
        stepsPerSecond = 30.0f;

    bfs->stepDelay = 1.0f / stepsPerSecond;
}

bool BFS_IsRunning(const BFS *bfs)
{
    return bfs != NULL && bfs->isRunning;
}

bool BFS_IsPaused(const BFS *bfs)
{
    return bfs != NULL && bfs->isPaused;
}

bool BFS_IsFinished(const BFS *bfs)
{
    return bfs != NULL && bfs->isFinished;
}

bool BFS_PathFound(const BFS *bfs)
{
    return bfs != NULL && bfs->pathFound;
}

float BFS_GetSpeed(const BFS *bfs)
{
    if (bfs == NULL || bfs->stepDelay <= 0.0f)
        return 0.0f;

    return 1.0f / bfs->stepDelay;
}