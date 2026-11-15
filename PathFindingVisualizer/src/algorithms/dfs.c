#include "dfs.h"
#include <stddef.h>

// بررسی قابل عبور بودن خانه
static bool IsWalkable(const Node *node)
{
    return node != NULL && node->state != NODE_WALL;
}


// اضافه کردن همسایه به Stack
static void VisitNeighbor(
    DFS *dfs,
    Node *current,
    int row,
    int col
)
{
    Node *neighbor = Grid_GetNode(dfs->grid, row, col);

    if (!IsWalkable(neighbor))
        return;

    // جلوگیری از اضافه شدن خانه تکراری
    if (neighbor->distance >= 0)
        return;

    neighbor->distance = current->distance + 1;
    neighbor->previousNode = current;
    neighbor->state = NODE_FRONTIER;

    Stack_Push(&dfs->stack, neighbor);
}


// بازسازی مسیر نهایی
static void ReconstructPath(DFS *dfs)
{
    Node *current = dfs->grid->goalNode;

    while (current != NULL && current != dfs->grid->startNode)
    {
        current->state = NODE_PATH;
        current = current->previousNode;
    }
}


// اجرای یک مرحله از DFS
static void DFS_Step(DFS *dfs)
{
    if (Stack_IsEmpty(&dfs->stack))
    {
        dfs->isRunning = false;
        dfs->isPaused = false;
        dfs->isFinished = true;
        dfs->pathFound = false;
        return;
    }

    Node *current = Stack_Pop(&dfs->stack);

    if (current == NULL)
        return;

    current->state = NODE_VISITED;

    // بررسی رسیدن به مقصد
    if (current == dfs->grid->goalNode)
    {
        ReconstructPath(dfs);

        dfs->isRunning = false;
        dfs->isPaused = false;
        dfs->isFinished = true;
        dfs->pathFound = true;

        return;
    }

    /*
       ترتیب اضافه کردن همسایه‌ها برعکس ترتیب
       مورد نظر است؛ چون Stack از نوع LIFO است.

       در این پیاده‌سازی اولویت حرکت:
       بالا، راست، پایین، چپ
    */

    VisitNeighbor(
        dfs,
        current,
        current->row,
        current->col - 1
    );

    VisitNeighbor(
        dfs,
        current,
        current->row + 1,
        current->col
    );

    VisitNeighbor(
        dfs,
        current,
        current->row,
        current->col + 1
    );

    VisitNeighbor(
        dfs,
        current,
        current->row - 1,
        current->col
    );

    // بررسی خالی شدن Stack
    if (Stack_IsEmpty(&dfs->stack))
    {
        dfs->isRunning = false;
        dfs->isPaused = false;
        dfs->isFinished = true;
        dfs->pathFound = false;
    }
}


// مقداردهی اولیه DFS
void DFS_Init(DFS *dfs)
{
    dfs->grid = NULL;

    Stack_Init(&dfs->stack);

    dfs->isRunning = false;
    dfs->isPaused = false;
    dfs->isFinished = false;
    dfs->pathFound = false;

    dfs->timer = 0.0f;
    dfs->stepDelay = 0.08f;
}


// شروع الگوریتم DFS
void DFS_Start(DFS *dfs, Grid *grid)
{
    if (dfs == NULL || grid == NULL ||
        grid->startNode == NULL || grid->goalNode == NULL)
        return;

    Grid_ClearWallsAndSearch(grid);

    dfs->grid = grid;

    Stack_Init(&dfs->stack);

    dfs->timer = 0.0f;

    dfs->isRunning = true;
    dfs->isPaused = false;
    dfs->isFinished = false;
    dfs->pathFound = false;

    Node *start = grid->startNode;

    start->distance = 0.0f;
    start->state = NODE_FRONTIER;

    Stack_Push(&dfs->stack, start);
}


// به‌روزرسانی انیمیشن DFS
void DFS_Update(DFS *dfs, float deltaTime)
{
    if (dfs == NULL || !dfs->isRunning || dfs->isPaused)
        return;

    dfs->timer += deltaTime;

    while (dfs->timer >= dfs->stepDelay && dfs->isRunning)
    {
        dfs->timer -= dfs->stepDelay;

        DFS_Step(dfs);
    }
}


// توقف موقت
void DFS_Pause(DFS *dfs)
{
    if (dfs == NULL || !dfs->isRunning || dfs->isFinished)
        return;

    dfs->isPaused = true;
}


// ادامه اجرا
void DFS_Resume(DFS *dfs)
{
    if (dfs == NULL || !dfs->isRunning || dfs->isFinished)
        return;

    dfs->isPaused = false;
}


// تنظیم سرعت
void DFS_SetSpeed(DFS *dfs, float stepsPerSecond)
{
    if (dfs == NULL)
        return;

    if (stepsPerSecond < 1.0f)
        stepsPerSecond = 1.0f;

    if (stepsPerSecond > 30.0f)
        stepsPerSecond = 30.0f;

    dfs->stepDelay = 1.0f / stepsPerSecond;
}


// بررسی وضعیت اجرا
bool DFS_IsRunning(const DFS *dfs)
{
    return dfs != NULL && dfs->isRunning;
}


// بررسی وضعیت توقف
bool DFS_IsPaused(const DFS *dfs)
{
    return dfs != NULL && dfs->isPaused;
}


// بررسی اتمام الگوریتم
bool DFS_IsFinished(const DFS *dfs)
{
    return dfs != NULL && dfs->isFinished;
}


// بررسی پیدا شدن مسیر
bool DFS_PathFound(const DFS *dfs)
{
    return dfs != NULL && dfs->pathFound;
}


// دریافت سرعت فعلی
float DFS_GetSpeed(const DFS *dfs)
{
    if (dfs == NULL || dfs->stepDelay <= 0.0f)
        return 0.0f;

    return 1.0f / dfs->stepDelay;
}