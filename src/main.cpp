#include <raylib.h>
#include <raymath.h>
using namespace std;

#include "Utilities/SceneManager.h"

#include "Scenes/MenuScene.h"
#include "Scenes/MapsMenuScene.h"
#include "Scenes/TestScene.h"

int main()
{
#ifdef PLATFORM_DESKTOP
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "An amazing project");
    SetWindowMinSize(screenWidth / 2, screenHeight / 2);
    ChangeDirectory("assets");
#else
    InitWindow(0, 0, "An amazing project");
#endif

    globalSceneManager.InsertScene("Menu", make_unique<MenuScene>());
    globalSceneManager.InsertScene("MapsMenu", make_unique<MapsMenuScene>());
    globalSceneManager.InsertScene("Test", make_unique<TestScene>());

    globalSceneManager.LoadScene("Menu");

    SetTargetFPS(60);

    Vector2 lastWindowSize = {0};
    const Color background = {0, 0, 0, 0};
    while (!WindowShouldClose())
    {
        SetExitKey(KEY_NULL);

#ifdef PLATFORM_DESKTOP
        if (IsKeyPressed(KEY_F11))
        {
            // Checks if the window is already fullscreen
            if (!IsWindowFullscreen())
            {
                // If it's not then resize the window to cover the entire monitor size and then toggle the fullscreen
                lastWindowSize = {(float)GetScreenWidth(), (float)GetScreenHeight()};
                int currentMonitor = GetCurrentMonitor();
                SetWindowSize(GetMonitorWidth(currentMonitor), GetMonitorHeight(currentMonitor));
                ToggleFullscreen();
            }
            else
            {
                // If it is then toggle the fullscreen and resize it back to it's original scale
                ToggleFullscreen();
                SetWindowSize(static_cast<int>(lastWindowSize.x), static_cast<int>(lastWindowSize.y));
            }
        }
#endif

        globalSceneManager.Update();

        BeginDrawing();

        ClearBackground(background);
        globalSceneManager.Draw();

        EndDrawing();

        globalSceneManager.LateUpdate();
    }

    CloseWindow();
    return 0;
}