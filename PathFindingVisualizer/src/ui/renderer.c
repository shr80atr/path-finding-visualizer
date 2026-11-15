#include "raylib.h"

#include "ui/renderer.h"
#include "utils/constants.h"
#include "utils/colors.h"


// تعیین رنگ هر خانه
static Color GetNodeColor(const Node *node)
{
    if (node->isStart)
        return COLOR_START;

    if (node->isGoal)
        return COLOR_GOAL;

    switch (node->state)
    {
        case NODE_WALL:
            return COLOR_WALL;

        case NODE_VISITED:
            return COLOR_VISITED;

        case NODE_FRONTIER:
            return COLOR_FRONTIER;

        case NODE_PATH:
            return COLOR_PATH;

        case NODE_EMPTY:
        default:
            return WHITE;
    }
}


// رسم نشانگر Start و Goal
static void DrawMarker(int x, int y)
{
    int centerX = x + CELL_SIZE / 2;

    // پایه پرچم
    DrawLine(
        centerX - 7,
        y + 12,
        centerX - 7,
        y + 40,
        WHITE
    );

    // پرچم
    Vector2 p1 = {
        centerX - 6,
        y + 13
    };

    Vector2 p2 = {
        centerX + 11,
        y + 21
    };

    Vector2 p3 = {
        centerX - 6,
        y + 29
    };

    DrawTriangle(p1, p2, p3, WHITE);
}


// رسم یک خانه
void Renderer_DrawCell(const Node *node)
{
    int x = GRID_X + node->col * CELL_SIZE;
    int y = GRID_Y + node->row * CELL_SIZE;

    Rectangle cell = {
        (float)x + 1,
        (float)y + 1,
        CELL_SIZE - 2,
        CELL_SIZE - 2
    };

    Color color = GetNodeColor(node);

    // رنگ پس زمینه خانه
    DrawRectangleRec(cell, color);

    // خطوط اطراف خانه
    DrawRectangleLinesEx(
        cell,
        GRID_LINE_THICKNESS,
        COLOR_GRID_LINE
    );

    // نمایش پرچم Start و Goal
    if (node->isStart || node->isGoal)
    {
        DrawMarker(x, y);
    }
}


// رسم شماره ردیف ها
void Renderer_DrawRowNumbers(void)
{
    for (int row = 0; row < GRID_ROWS; row++)
    {
        int y = GRID_Y + row * CELL_SIZE;

        DrawText(
            TextFormat("%d", row),
            38,
            y + 16,
            18,
            COLOR_LABEL
        );
    }
}


// رسم Grid
void Renderer_DrawGrid(const Grid *grid)
{
    Renderer_DrawRowNumbers();

    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            const Node *node = &grid->nodes[row][col];

            Renderer_DrawCell(node);
        }
    }
}


// رسم یک آیتم از راهنمای رنگ ها
static void DrawLegendItem(
    int x,
    int y,
    Color color,
    const char *label
)
{
    DrawRectangle(x, y, 22, 22, color);

    DrawRectangleLines(
        x,
        y,
        22,
        22,
        COLOR_GRID_LINE
    );

    DrawText(
        label,
        x + 30,
        y + 1,
        19,
        COLOR_TEXT
    );
}


// رسم Legend
void Renderer_DrawLegend(void)
{
    int y = GRID_Y + GRID_ROWS * CELL_SIZE + 18;

    DrawLegendItem(12,  y, COLOR_START,    "Start");
    DrawLegendItem(115, y, COLOR_GOAL,     "Goal");
    DrawLegendItem(210, y, COLOR_WALL,     "Wall");
    DrawLegendItem(300, y, COLOR_FRONTIER, "Frontier");
    DrawLegendItem(435, y, COLOR_VISITED,  "Visited");
    DrawLegendItem(555, y, COLOR_PATH,     "Path");
}