#include "raylib.h"

#include "grid.h"
#include "ui/renderer.h"
#include "ui/controls.h"
#include "utils/constants.h"
#include "utils/colors.h"
#include "input.h"
#include "dijkstra.h"
#include "astar.h"

int main(void)
{
    // Initialize Grid
    Grid grid;
    Grid_Init(&grid);

    // Initialize BFS
    BFS bfs;
    BFS_Init(&bfs);

    // Initialize DFS
    DFS dfs;
    DFS_Init(&dfs);

    // Initialize Dijkstra
    Dijkstra dijkstra;
    Dijkstra_Init(&dijkstra);

    AStar astar;
    AStar_Init(&astar);

    // Default selected algorithm
    AlgorithmType selectedAlgorithm = ALGORITHM_BFS;

    EditMode mode = EDIT_WALL;

    // Initialize Window
    InitWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        WINDOW_TITLE);

    SetTargetFPS(TARGET_FPS);

    // Main Loop
    while (!WindowShouldClose())
    {
        // Allow grid editing only when no algorithm is running
        if (!BFS_IsRunning(&bfs) &&
            !DFS_IsRunning(&dfs) &&
            !Dijkstra_IsRunning(&dijkstra) &&
            !AStar_IsRunning(&astar))
        {
            Input_Handle(&grid, &mode);
        }

        // Handle controls
        bool startRequested = Controls_HandleInput(
            &bfs,
            &dfs,
            &dijkstra,
            &astar,
            &selectedAlgorithm);

        // Start selected algorithm
        if (startRequested)
        {
            switch (selectedAlgorithm)
            {
            case ALGORITHM_BFS:
                BFS_Start(&bfs, &grid);
                break;

            case ALGORITHM_DFS:
                DFS_Start(&dfs, &grid);
                break;

            case ALGORITHM_DIJKSTRA:
                Dijkstra_Start(&dijkstra, &grid);
                break;

            case ALGORITHM_ASTAR:
                AStar_Start(&astar, &grid);
                break;
            }
        }

        // Update algorithms
        BFS_Update(&bfs, GetFrameTime());
        DFS_Update(&dfs, GetFrameTime());
        Dijkstra_Update(&dijkstra, GetFrameTime());
        AStar_Update(&astar, GetFrameTime());

        BeginDrawing();

        ClearBackground(COLOR_BACKGROUND);

        Renderer_DrawGrid(&grid);
        Renderer_DrawLegend();

        Controls_Draw(
            &bfs,
            &dfs,
            &dijkstra,
            &astar,
            selectedAlgorithm);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}