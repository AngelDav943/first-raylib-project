#include "../Scene.h"
#include <bits/stdc++.h>
#include <list>
#include <memory>

#include <raylib.h>
#include <raymath.h>

#include "../Utilities/SceneManager.h"
#include "../Utilities/CameraController.h"
#include "../Utilities/ElementsUI.h"

#include "TestScene.h"

class MenuScene : public Scene
{
private:
    Texture2D tile_texture;
    UIButton *startButton = new UIButton(
        (Vector2){100, 100},
        (Vector2){200, 50},
        "Change scene");

public:
    // Initialize game assets
    void Start() override
    {
        tile_texture = LoadTexture("assets/textures/blueprint_tiles.png");

        // Sets the callback function for the onClick event
        startButton->onClick([]()
                             { globalSceneManager.ChangeScene(std::make_unique<TestScene>()); });

        ui.AddElement(startButton);
    }

    // Game logic update
    void Update() override
    {
        /*if (IsKeyPressed(KEY_ENTER)) // Change scene when Enter is pressed
        {
            globalSceneManager.ChangeScene(std::make_unique<TestScene>());
        }*/
    }

    void Draw() override
    {
        int tileWidthRepeats = (int)ceilf(GetScreenWidth() / tile_texture.width);
        int tileHeightRepeats = (int)ceilf(GetScreenHeight() / tile_texture.height);

        DrawTexture(tile_texture, 0, 0, WHITE);
        for (int x = 0; x <= tileWidthRepeats; x++)
        {
            for (int y = 0; y <= tileHeightRepeats; y++)
            {
                DrawTexture(tile_texture, x * tile_texture.width, y * tile_texture.height, WHITE);
            }
        }

        list<string> texts = {
            ("Hello world! FPS:" + to_string(GetFPS())),
            "Click the button to change scene.",
            ("x repeats:" + to_string(tileWidthRepeats)),
            ("y repeats:" + to_string(tileHeightRepeats))};

        int i = 0;
        for (string text : texts)
        {
            i++;
            DrawText(text.c_str(), 20, 20 + (i * 25), 20, ORANGE);
        }

        startButton->position = Vector2{
            (GetScreenWidth()/2) - (startButton->size.x/2),
            (GetScreenHeight()/2) - (startButton->size.y/2)
            };
    }

    void Unload() override
    {
        // Unload game assets
    }
};