#include "raylib.h"

#include "ui/controls.h"
#include "utils/constants.h"
#include "astar.h"

#include <stddef.h>
#include <stdbool.h>

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 36

#define PAUSE_BUTTON_X 72
#define RESUME_BUTTON_X 184
#define BUTTON_Y 850

#define SLIDER_X 450
#define SLIDER_Y 868
#define SLIDER_WIDTH 300

#define DROPDOWN_X 950
#define DROPDOWN_Y 850
#define DROPDOWN_WIDTH 150
#define DROPDOWN_HEIGHT 30

#define START_BUTTON_X 750
#define START_BUTTON_Y 810
#define START_BUTTON_WIDTH 85

#define MIN_SPEED 1.0f
#define MAX_SPEED 30.0f

static bool isDraggingSlider = false;
static bool isDropdownOpen = false;

// بررسی قرار گرفتن موس روی محدوده
static bool IsMouseOver(Rectangle rect)
{
    return CheckCollisionPointRec(
        GetMousePosition(),
        rect);
}

// بررسی فعال بودن هر الگوریتم
static bool IsAnyAlgorithmRunning(
    const BFS *bfs,
    const DFS *dfs,
    const Dijkstra *dijkstra,
    const AStar *astar)
{
    return BFS_IsRunning(bfs) ||
           DFS_IsRunning(dfs) ||
           Dijkstra_IsRunning(dijkstra) ||
           AStar_IsRunning(astar);
}

// تنظیم سرعت الگوریتم انتخاب‌شده
static void SetSelectedSpeed(
    BFS *bfs,
    DFS *dfs,
    Dijkstra *dijkstra,
    AStar *astar,
    AlgorithmType algorithm,
    float speed)
{
    switch (algorithm)
    {
    case ALGORITHM_BFS:
        BFS_SetSpeed(bfs, speed);
        break;

    case ALGORITHM_DFS:
        DFS_SetSpeed(dfs, speed);
        break;

    case ALGORITHM_DIJKSTRA:
        Dijkstra_SetSpeed(dijkstra, speed);
        break;

    case ALGORITHM_ASTAR:
        AStar_SetSpeed(astar, speed);
        break;
    }
}

static float GetSelectedSpeed(
    const BFS *bfs,
    const DFS *dfs,
    const Dijkstra *dijkstra,
    const AStar *astar,
    AlgorithmType algorithm)
{
    switch (algorithm)
    {
    case ALGORITHM_BFS:
        return BFS_GetSpeed(bfs);

    case ALGORITHM_DFS:
        return DFS_GetSpeed(dfs);

    case ALGORITHM_DIJKSTRA:
        return Dijkstra_GetSpeed(dijkstra);

    case ALGORITHM_ASTAR:
        return AStar_GetSpeed(astar);
    }

    return 0.0f;
}

static void PauseSelectedAlgorithm(
    BFS *bfs,
    DFS *dfs,
    Dijkstra *dijkstra,
    AStar *astar,
    AlgorithmType algorithm)
{
    switch (algorithm)
    {
    case ALGORITHM_BFS:
        BFS_Pause(bfs);
        break;

    case ALGORITHM_DFS:
        DFS_Pause(dfs);
        break;

    case ALGORITHM_DIJKSTRA:
        Dijkstra_Pause(dijkstra);
        break;

    case ALGORITHM_ASTAR:
        AStar_Pause(astar);
        break;
    }
}

static void ResumeSelectedAlgorithm(
    BFS *bfs,
    DFS *dfs,
    Dijkstra *dijkstra,
    AStar *astar,
    AlgorithmType algorithm)
{
    switch (algorithm)
    {
    case ALGORITHM_BFS:
        BFS_Resume(bfs);
        break;

    case ALGORITHM_DFS:
        DFS_Resume(dfs);
        break;

    case ALGORITHM_DIJKSTRA:
        Dijkstra_Resume(dijkstra);
        break;

    case ALGORITHM_ASTAR:
        AStar_Resume(astar);
        break;
    }
}

static bool IsSelectedAlgorithmPaused(
    const BFS *bfs,
    const DFS *dfs,
    const Dijkstra *dijkstra,
    const AStar *astar,
    AlgorithmType algorithm)
{
    switch (algorithm)
    {
    case ALGORITHM_BFS:
        return BFS_IsPaused(bfs);

    case ALGORITHM_DFS:
        return DFS_IsPaused(dfs);

    case ALGORITHM_DIJKSTRA:
        return Dijkstra_IsPaused(dijkstra);

    case ALGORITHM_ASTAR:
        return AStar_IsPaused(astar);
    }

    return false;
}

// تبدیل موقعیت موس به سرعت
static float GetSpeedFromMouse(void)
{
    Vector2 mouse = GetMousePosition();

    float position = (mouse.x - SLIDER_X) / SLIDER_WIDTH;

    if (position < 0.0f)
        position = 0.0f;

    if (position > 1.0f)
        position = 1.0f;

    return MIN_SPEED + position * (MAX_SPEED - MIN_SPEED);
}

// مدیریت ورودی کنترل‌ها
// مدیریت ورودی کنترل‌ها
bool Controls_HandleInput(
    BFS *bfs,
    DFS *dfs,
    Dijkstra *dijkstra,
    AStar *astar,
    AlgorithmType *selectedAlgorithm)
{
    if (bfs == NULL || dfs == NULL ||
        dijkstra == NULL || astar == NULL || selectedAlgorithm == NULL)
    {
        return false;
    }

    // بررسی وضعیت الگوریتم‌ها
    bool running = IsAnyAlgorithmRunning(
        bfs,
        dfs,
        dijkstra,
        astar);

    bool paused = IsSelectedAlgorithmPaused(
        bfs,
        dfs,
        dijkstra,
        astar,
        *selectedAlgorithm);

    // دریافت وضعیت کلیک موس
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    Vector2 mouse = GetMousePosition();

    // محدوده دکمه‌ها
    Rectangle pauseButton = {
        PAUSE_BUTTON_X,
        BUTTON_Y,
        BUTTON_WIDTH,
        BUTTON_HEIGHT};

    Rectangle resumeButton = {
        RESUME_BUTTON_X,
        BUTTON_Y,
        BUTTON_WIDTH,
        BUTTON_HEIGHT};

    Rectangle startButton = {
        START_BUTTON_X,
        START_BUTTON_Y,
        START_BUTTON_WIDTH,
        BUTTON_HEIGHT};

    Rectangle dropdown = {
        DROPDOWN_X,
        DROPDOWN_Y,
        DROPDOWN_WIDTH,
        DROPDOWN_HEIGHT};

    // =====================================
    // 1. مدیریت Dropdown
    // =====================================

    if (mousePressed && !running)
    {
        // اگر روی خود Dropdown کلیک شد
        if (IsMouseOver(dropdown))
        {
            isDropdownOpen = !isDropdownOpen;
            return false;
        }

        // اگر منو باز است، بررسی گزینه‌های آن
        if (isDropdownOpen)
        {
            Rectangle bfsOption = {
                DROPDOWN_X,
                DROPDOWN_Y - DROPDOWN_HEIGHT,
                DROPDOWN_WIDTH,
                DROPDOWN_HEIGHT};

            Rectangle dfsOption = {
                DROPDOWN_X,
                DROPDOWN_Y - DROPDOWN_HEIGHT * 2,
                DROPDOWN_WIDTH,
                DROPDOWN_HEIGHT};

            Rectangle dijkstraOption = {
                DROPDOWN_X,
                DROPDOWN_Y - DROPDOWN_HEIGHT * 3,
                DROPDOWN_WIDTH,
                DROPDOWN_HEIGHT};

            Rectangle astarOption = {
                DROPDOWN_X,
                DROPDOWN_Y - DROPDOWN_HEIGHT * 4,
                DROPDOWN_WIDTH,
                DROPDOWN_HEIGHT};

            if (CheckCollisionPointRec(mouse, bfsOption))
            {
                *selectedAlgorithm = ALGORITHM_BFS;
            }
            else if (CheckCollisionPointRec(mouse, dfsOption))
            {
                *selectedAlgorithm = ALGORITHM_DFS;
            }
            else if (CheckCollisionPointRec(mouse, dijkstraOption))
            {
                *selectedAlgorithm = ALGORITHM_DIJKSTRA;
            }
            else if (CheckCollisionPointRec(mouse, astarOption))
            {
                *selectedAlgorithm = ALGORITHM_ASTAR;
            }

            // بعد از انتخاب یا کلیک خارج از منو، آن را ببند
            isDropdownOpen = false;

            return false;
        }
    }

    // =====================================
    // 2. مدیریت دکمه Start
    // =====================================

    if (mousePressed && !running)
    {
        if (CheckCollisionPointRec(mouse, startButton))
        {
            isDropdownOpen = false;
            return true;
        }
    }

    // =====================================
    // 3. مدیریت دکمه Pause
    // =====================================

    if (mousePressed && running && !paused)
    {
        if (CheckCollisionPointRec(mouse, pauseButton))
        {
            PauseSelectedAlgorithm(
                bfs,
                dfs,
                dijkstra,
                astar,
                *selectedAlgorithm);

            return false;
        }
    }

    // =====================================
    // 4. مدیریت دکمه Resume
    // =====================================

    if (mousePressed && running && paused)
    {
        if (CheckCollisionPointRec(mouse, resumeButton))
        {
            ResumeSelectedAlgorithm(
                bfs,
                dfs,
                dijkstra,
                astar,
                *selectedAlgorithm);

            return false;
        }
    }

    // =====================================
    // 5. مدیریت Speed Slider
    // =====================================

    Rectangle sliderHitbox = {
        SLIDER_X - 10,
        SLIDER_Y - 12,
        SLIDER_WIDTH + 20,
        24};

    // شروع Drag کردن Slider
    if (mousePressed && CheckCollisionPointRec(mouse, sliderHitbox))
    {
        isDraggingSlider = true;
    }

    // تغییر سرعت هنگام Drag
    if (isDraggingSlider && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        float speed = GetSpeedFromMouse();

        SetSelectedSpeed(
            bfs,
            dfs,
            dijkstra,
            astar,
            *selectedAlgorithm,
            speed);
    }

    // پایان Drag
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        isDraggingSlider = false;
    }

    return false;
}

// رسم دکمه
static void DrawControlButton(
    Rectangle rect,
    const char *label,
    bool enabled,
    bool hovered)
{
    Color buttonColor;

    if (!enabled)
        buttonColor = LIGHTGRAY;
    else if (hovered)
        buttonColor = DARKBLUE;
    else
        buttonColor = BLUE;

    DrawRectangleRounded(
        rect,
        0.2f,
        8,
        buttonColor);

    int textWidth = MeasureText(label, 18);

    DrawText(
        label,
        (int)(rect.x + (rect.width - textWidth) / 2),
        (int)(rect.y + 9),
        18,
        enabled ? WHITE : GRAY);
}

// رسم Slider
static void DrawSpeedSlider(
    const BFS *bfs,
    const DFS *dfs,
    const Dijkstra *dijkstra,
    const AStar *astar,
    AlgorithmType algorithm)
{
    float speed = GetSelectedSpeed(
        bfs,
        dfs,
        dijkstra,
        astar,
        algorithm);

    float normalizedSpeed =
        (speed - MIN_SPEED) / (MAX_SPEED - MIN_SPEED);

    float knobX = SLIDER_X + normalizedSpeed * SLIDER_WIDTH;

    DrawText(
        "Speed:",
        365,
        857,
        18,
        DARKGRAY);

    DrawRectangle(
        SLIDER_X,
        SLIDER_Y - 3,
        SLIDER_WIDTH,
        6,
        LIGHTGRAY);

    DrawRectangle(
        SLIDER_X,
        SLIDER_Y - 3,
        (int)(knobX - SLIDER_X),
        6,
        BLUE);

    DrawCircle(
        (int)knobX,
        SLIDER_Y,
        10,
        BLUE);

    DrawText(
        TextFormat("%d steps/sec", (int)(speed + 0.5f)),
        SLIDER_X + SLIDER_WIDTH + 20,
        857,
        18,
        DARKGRAY);
}

// رسم Dropdown
static void DrawAlgorithmDropdown(AlgorithmType algorithm)
{
    Rectangle dropdown = {
        DROPDOWN_X,
        DROPDOWN_Y,
        DROPDOWN_WIDTH,
        DROPDOWN_HEIGHT};

    DrawRectangleRec(dropdown, WHITE);
    DrawRectangleLinesEx(dropdown, 1, DARKGRAY);

    const char *selectedText;

    switch (algorithm)
    {
    case ALGORITHM_BFS:
        selectedText = "BFS";
        break;

    case ALGORITHM_DFS:
        selectedText = "DFS";
        break;

    case ALGORITHM_DIJKSTRA:
        selectedText = "Dijkstra";
        break;

    case ALGORITHM_ASTAR:
        selectedText = "A*";
        break;

    default:
        selectedText = "Unknown";
        break;
    }

    DrawText(
        selectedText,
        DROPDOWN_X + 12,
        DROPDOWN_Y + 6,
        18,
        DARKGRAY);

    DrawText(
        isDropdownOpen ? "^" : "v",
        DROPDOWN_X + DROPDOWN_WIDTH - 25,
        DROPDOWN_Y + 5,
        20,
        DARKGRAY);

    if (isDropdownOpen)
    {
        Rectangle bfsOption = {
            DROPDOWN_X,
            DROPDOWN_Y - DROPDOWN_HEIGHT,
            DROPDOWN_WIDTH,
            DROPDOWN_HEIGHT};

        Rectangle dfsOption = {
            DROPDOWN_X,
            DROPDOWN_Y - DROPDOWN_HEIGHT * 2,
            DROPDOWN_WIDTH,
            DROPDOWN_HEIGHT};

        Rectangle dijkstraOption = {
            DROPDOWN_X,
            DROPDOWN_Y - DROPDOWN_HEIGHT * 3,
            DROPDOWN_WIDTH,
            DROPDOWN_HEIGHT};

        Rectangle astarOption = {
            DROPDOWN_X,
            DROPDOWN_Y - DROPDOWN_HEIGHT * 4,
            DROPDOWN_WIDTH,
            DROPDOWN_HEIGHT};

        DrawRectangleRec(bfsOption, WHITE);
        DrawRectangleLinesEx(bfsOption, 1, DARKGRAY);
        DrawText("BFS", DROPDOWN_X + 12, (int)bfsOption.y + 6, 18, DARKGRAY);

        DrawRectangleRec(dfsOption, WHITE);
        DrawRectangleLinesEx(dfsOption, 1, DARKGRAY);
        DrawText("DFS", DROPDOWN_X + 12, (int)dfsOption.y + 6, 18, DARKGRAY);

        DrawRectangleRec(dijkstraOption, WHITE);
        DrawRectangleLinesEx(dijkstraOption, 1, DARKGRAY);
        DrawText("Dijkstra", DROPDOWN_X + 12, (int)dijkstraOption.y + 6, 18, DARKGRAY);

        DrawRectangleRec(astarOption, WHITE);
        DrawRectangleLinesEx(astarOption, 1, DARKGRAY);

        DrawText(
            "A*",
            DROPDOWN_X + 12,
            (int)astarOption.y + 6,
            18,
            DARKGRAY);
    }
}

// رسم تمام کنترل‌ها
void Controls_Draw(
    const BFS *bfs,
    const DFS *dfs,
    const Dijkstra *dijkstra,
    const AStar *astar,
    AlgorithmType selectedAlgorithm)
{
    // بررسی معتبر بودن اشاره‌گرها
    if (bfs == NULL || dfs == NULL || dijkstra == NULL || astar == NULL)
        return;

    // بررسی وضعیت اجرای الگوریتم‌ها
    bool running = IsAnyAlgorithmRunning(
        bfs,
        dfs,
        dijkstra,
        astar);

    // بررسی وضعیت Pause الگوریتم انتخاب‌شده
    bool paused = IsSelectedAlgorithmPaused(
        bfs,
        dfs,
        dijkstra,
        astar,
        selectedAlgorithm);

    // محدوده دکمه Pause
    Rectangle pauseButton = {
        PAUSE_BUTTON_X,
        BUTTON_Y,
        BUTTON_WIDTH,
        BUTTON_HEIGHT};

    // محدوده دکمه Resume
    Rectangle resumeButton = {
        RESUME_BUTTON_X,
        BUTTON_Y,
        BUTTON_WIDTH,
        BUTTON_HEIGHT};

    // محدوده دکمه Start
    Rectangle startButton = {
        START_BUTTON_X,
        START_BUTTON_Y,
        START_BUTTON_WIDTH,
        BUTTON_HEIGHT};

    // رسم دکمه Pause
    DrawControlButton(
        pauseButton,
        "Pause",
        running && !paused,
        IsMouseOver(pauseButton));

    // رسم دکمه Resume
    DrawControlButton(
        resumeButton,
        "Resume",
        running && paused,
        IsMouseOver(resumeButton));

    // رسم دکمه Start
    DrawControlButton(
        startButton,
        "Start",
        !running,
        IsMouseOver(startButton));

    // رسم Slider سرعت
    DrawSpeedSlider(
        bfs,
        dfs,
        dijkstra,
        astar,
        selectedAlgorithm);

    // رسم منوی انتخاب الگوریتم
    DrawAlgorithmDropdown(selectedAlgorithm);
}