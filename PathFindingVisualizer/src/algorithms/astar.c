#include "astar.h"

#include <stddef.h>
#include <stdlib.h>

// بررسی قابل عبور بودن خانه
static bool IsWalkable(Node *node)
{
    return node != NULL && node->state != NODE_WALL;
}

// محاسبه فاصله Manhattan
static int CalculateHeuristic(Node *node, Node *goal)
{
    int rowDistance = abs(node->row - goal->row);
    int colDistance = abs(node->col - goal->col);

    return rowDistance + colDistance;
}

// پیدا کردن خانه Frontier با کمترین مقدار F
static Node *GetBestFrontier(AStar *astar)
{
    Node *bestNode = NULL;
    int lowestScore = 0;

    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            Node *node = Grid_GetNode(
                astar->grid,
                row,
                col
            );

            if (node->state != NODE_FRONTIER)
                continue;

            int score = node->distance + node->heuristic;

            if (bestNode == NULL || score < lowestScore)
            {
                bestNode = node;
                lowestScore = score;
            }
        }
    }

    return bestNode;
}

// بازسازی مسیر نهایی
static void ReconstructPath(AStar *astar)
{
    Node *current = astar->grid->goalNode;
    Node *start = astar->grid->startNode;

    while (current != NULL && current != start)
    {
        if (current != astar->grid->goalNode)
        {
            current->state = NODE_PATH;
        }

        current = current->previousNode;
    }
}

// بررسی و به‌روزرسانی همسایه
static void UpdateNeighbor(
    AStar *astar,
    Node *current,
    int row,
    int col)
{
    Node *neighbor = Grid_GetNode(
        astar->grid,
        row,
        col
    );

    if (!IsWalkable(neighbor))
        return;

    // خانه‌ای که قبلاً نهایی شده است
    if (neighbor->state == NODE_VISITED)
        return;

    // هزینه رسیدن به همسایه
    int newDistance = current->distance + 1;

    // اگر مسیر جدید بهتر باشد
    if (neighbor->state != NODE_FRONTIER ||
        newDistance < neighbor->distance)
    {
        neighbor->distance = newDistance;

        neighbor->heuristic = CalculateHeuristic(
            neighbor,
            astar->grid->goalNode
        );

        neighbor->previousNode = current;
        neighbor->state = NODE_FRONTIER;
    }
}

// اجرای یک مرحله از الگوریتم
static void AStar_Step(AStar *astar)
{
    Node *current = GetBestFrontier(astar);

    // اگر Frontier خالی باشد، مسیری وجود ندارد
    if (current == NULL)
    {
        astar->isRunning = false;
        astar->isFinished = true;
        astar->pathFound = false;

        return;
    }

    // خانه فعلی بررسی شده است
    current->state = NODE_VISITED;

    // بررسی رسیدن به مقصد
    if (current == astar->grid->goalNode)
    {
        ReconstructPath(astar);

        astar->isRunning = false;
        astar->isFinished = true;
        astar->pathFound = true;

        return;
    }

    int row = current->row;
    int col = current->col;

    // بررسی همسایه بالا
    UpdateNeighbor(
        astar,
        current,
        row - 1,
        col
    );

    // بررسی همسایه پایین
    UpdateNeighbor(
        astar,
        current,
        row + 1,
        col
    );

    // بررسی همسایه چپ
    UpdateNeighbor(
        astar,
        current,
        row,
        col - 1
    );

    // بررسی همسایه راست
    UpdateNeighbor(
        astar,
        current,
        row,
        col + 1
    );
}

// مقداردهی اولیه
void AStar_Init(AStar *astar)
{
    astar->grid = NULL;

    astar->isRunning = false;
    astar->isPaused = false;
    astar->isFinished = false;
    astar->pathFound = false;

    astar->timer = 0.0f;
    astar->stepDelay = 0.08f;
}

// شروع الگوریتم
void AStar_Start(AStar *astar, Grid *grid)
{
    if (astar == NULL || grid == NULL ||
        grid->startNode == NULL || grid->goalNode == NULL)
    {
        return;
    }

    Grid_ClearWallsAndSearch(grid);

    astar->grid = grid;

    astar->isRunning = true;
    astar->isPaused = false;
    astar->isFinished = false;
    astar->pathFound = false;

    astar->timer = 0.0f;

    Node *start = grid->startNode;

    start->distance = 0;

    start->heuristic = CalculateHeuristic(
        start,
        grid->goalNode
    );

    start->previousNode = NULL;
    start->state = NODE_FRONTIER;
}

// به‌روزرسانی الگوریتم
void AStar_Update(AStar *astar, float deltaTime)
{
    if (astar == NULL ||
        !astar->isRunning ||
        astar->isPaused)
    {
        return;
    }

    astar->timer += deltaTime;

    while (astar->timer >= astar->stepDelay &&
           astar->isRunning &&
           !astar->isPaused)
    {
        astar->timer -= astar->stepDelay;

        AStar_Step(astar);
    }
}

// توقف الگوریتم
void AStar_Pause(AStar *astar)
{
    if (astar != NULL && astar->isRunning)
    {
        astar->isPaused = true;
    }
}

// ادامه الگوریتم
void AStar_Resume(AStar *astar)
{
    if (astar != NULL &&
        astar->isRunning &&
        astar->isPaused)
    {
        astar->isPaused = false;
    }
}

// تنظیم سرعت
void AStar_SetSpeed(AStar *astar, float stepsPerSecond)
{
    if (astar == NULL)
        return;

    if (stepsPerSecond < 1.0f)
        stepsPerSecond = 1.0f;

    if (stepsPerSecond > 30.0f)
        stepsPerSecond = 30.0f;

    astar->stepDelay = 1.0f / stepsPerSecond;
}

// دریافت سرعت
float AStar_GetSpeed(const AStar *astar)
{
    if (astar == NULL || astar->stepDelay <= 0.0f)
        return 0.0f;

    return 1.0f / astar->stepDelay;
}

// بررسی وضعیت اجرا
bool AStar_IsRunning(const AStar *astar)
{
    return astar != NULL && astar->isRunning;
}

// بررسی وضعیت توقف
bool AStar_IsPaused(const AStar *astar)
{
    return astar != NULL && astar->isPaused;
}

// بررسی پایان الگوریتم
bool AStar_IsFinished(const AStar *astar)
{
    return astar != NULL && astar->isFinished;
}

// بررسی پیدا شدن مسیر
bool AStar_PathFound(const AStar *astar)
{
    return astar != NULL && astar->pathFound;
}