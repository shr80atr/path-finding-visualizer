#include "dijkstra.h"
#include <stddef.h>


// بررسی قابل عبور بودن خانه
static bool IsWalkable(const Node *node)
{
    return node != NULL && node->state != NODE_WALL;
}


// پیدا کردن خانه Frontier با کمترین فاصله
static Node *GetClosestFrontier(Dijkstra *dijkstra)
{
    Node *closestNode = NULL;

    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            Node *node = &dijkstra->grid->nodes[row][col];

            if (node->state != NODE_FRONTIER)
                continue;

            if (closestNode == NULL ||
                node->distance < closestNode->distance)
            {
                closestNode = node;
            }
        }
    }

    return closestNode;
}


// بازسازی مسیر نهایی
static void ReconstructPath(Dijkstra *dijkstra)
{
    Node *current = dijkstra->grid->goalNode;

    while (current != NULL &&
           current != dijkstra->grid->startNode)
    {
        current->state = NODE_PATH;
        current = current->previousNode;
    }
}


// بررسی و به‌روزرسانی همسایه
static void UpdateNeighbor(
    Dijkstra *dijkstra,
    Node *current,
    int row,
    int col
)
{
    Node *neighbor = Grid_GetNode(dijkstra->grid, row, col);

    if (!IsWalkable(neighbor))
        return;

    // خانه‌ای که قبلاً نهایی شده، دوباره بررسی نمی‌شود
    if (neighbor->state == NODE_VISITED)
        return;

    // هزینه حرکت بین دو خانه در این نسخه برابر 1 است
    float newDistance = current->distance + 1.0f;

    // اگر فاصله جدید بهتر باشد، آن را ثبت می‌کنیم
    if (neighbor->distance < 0.0f ||
        newDistance < neighbor->distance)
    {
        neighbor->distance = newDistance;
        neighbor->previousNode = current;
        neighbor->state = NODE_FRONTIER;
    }
}


// اجرای یک مرحله از Dijkstra
static void Dijkstra_Step(Dijkstra *dijkstra)
{
    Node *current = GetClosestFrontier(dijkstra);

    // هیچ خانه قابل بررسی باقی نمانده است
    if (current == NULL)
    {
        dijkstra->isRunning = false;
        dijkstra->isPaused = false;
        dijkstra->isFinished = true;
        dijkstra->pathFound = false;
        return;
    }

    // خانه فعلی نهایی می‌شود
    current->state = NODE_VISITED;

    // آیا به مقصد رسیده‌ایم؟
    if (current == dijkstra->grid->goalNode)
    {
        ReconstructPath(dijkstra);

        dijkstra->isRunning = false;
        dijkstra->isPaused = false;
        dijkstra->isFinished = true;
        dijkstra->pathFound = true;

        return;
    }

    // بررسی چهار همسایه
    UpdateNeighbor(
        dijkstra,
        current,
        current->row - 1,
        current->col
    );

    UpdateNeighbor(
        dijkstra,
        current,
        current->row + 1,
        current->col
    );

    UpdateNeighbor(
        dijkstra,
        current,
        current->row,
        current->col - 1
    );

    UpdateNeighbor(
        dijkstra,
        current,
        current->row,
        current->col + 1
    );
}


// مقداردهی اولیه
void Dijkstra_Init(Dijkstra *dijkstra)
{
    dijkstra->grid = NULL;

    dijkstra->isRunning = false;
    dijkstra->isPaused = false;
    dijkstra->isFinished = false;
    dijkstra->pathFound = false;

    dijkstra->timer = 0.0f;
    dijkstra->stepDelay = 0.08f;
}


// شروع الگوریتم
void Dijkstra_Start(Dijkstra *dijkstra, Grid *grid)
{
    if (dijkstra == NULL ||
        grid == NULL ||
        grid->startNode == NULL ||
        grid->goalNode == NULL)
    {
        return;
    }

    Grid_ClearWallsAndSearch(grid);

    dijkstra->grid = grid;

    dijkstra->timer = 0.0f;

    dijkstra->isRunning = true;
    dijkstra->isPaused = false;
    dijkstra->isFinished = false;
    dijkstra->pathFound = false;

    Node *start = grid->startNode;

    start->distance = 0.0f;
    start->state = NODE_FRONTIER;
}


// به‌روزرسانی انیمیشن
void Dijkstra_Update(Dijkstra *dijkstra, float deltaTime)
{
    if (dijkstra == NULL ||
        !dijkstra->isRunning ||
        dijkstra->isPaused)
    {
        return;
    }

    dijkstra->timer += deltaTime;

    while (dijkstra->timer >= dijkstra->stepDelay &&
           dijkstra->isRunning)
    {
        dijkstra->timer -= dijkstra->stepDelay;

        Dijkstra_Step(dijkstra);
    }
}


// توقف موقت
void Dijkstra_Pause(Dijkstra *dijkstra)
{
    if (dijkstra == NULL ||
        !dijkstra->isRunning ||
        dijkstra->isFinished)
    {
        return;
    }

    dijkstra->isPaused = true;
}


// ادامه اجرا
void Dijkstra_Resume(Dijkstra *dijkstra)
{
    if (dijkstra == NULL ||
        !dijkstra->isRunning ||
        dijkstra->isFinished)
    {
        return;
    }

    dijkstra->isPaused = false;
}


// تنظیم سرعت
void Dijkstra_SetSpeed(Dijkstra *dijkstra, float stepsPerSecond)
{
    if (dijkstra == NULL)
        return;

    if (stepsPerSecond < 1.0f)
        stepsPerSecond = 1.0f;

    if (stepsPerSecond > 30.0f)
        stepsPerSecond = 30.0f;

    dijkstra->stepDelay = 1.0f / stepsPerSecond;
}


// وضعیت اجرا
bool Dijkstra_IsRunning(const Dijkstra *dijkstra)
{
    return dijkstra != NULL && dijkstra->isRunning;
}


// وضعیت توقف
bool Dijkstra_IsPaused(const Dijkstra *dijkstra)
{
    return dijkstra != NULL && dijkstra->isPaused;
}


// وضعیت پایان
bool Dijkstra_IsFinished(const Dijkstra *dijkstra)
{
    return dijkstra != NULL && dijkstra->isFinished;
}


// آیا مسیر پیدا شده است؟
bool Dijkstra_PathFound(const Dijkstra *dijkstra)
{
    return dijkstra != NULL && dijkstra->pathFound;
}


// دریافت سرعت فعلی
float Dijkstra_GetSpeed(const Dijkstra *dijkstra)
{
    if (dijkstra == NULL || dijkstra->stepDelay <= 0.0f)
        return 0.0f;

    return 1.0f / dijkstra->stepDelay;
}