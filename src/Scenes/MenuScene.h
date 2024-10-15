#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "../Scene.h"
#include <string>
#include <list>
#include <memory>

#include <raylib.h>
#include <raymath.h>

#include "../Utilities/SceneManager.h"
#include "../Utilities/CameraController.h"
#include "../Utilities/ElementsUI.h"

class MenuScene : public Scene
{
private:
    Texture2D tile_texture;

public:
    // Initialize game assets
    void Start() override
    {
        tile_texture = LoadTexture("assets/textures/blueprint_tiles.png");

        ui.AddElement(
            "test",
            new UIButton(
                UIPosition{
                    Vector2{0, -10}, // Local position
                    Anchor{
                        Vector2{0.5f, 1},   // Local anchor
                        Vector2{0.5f, 0.5f} // Screen anchor
                    },
                    Size{
                        Scaling{500, 50}, // Offset
                        Scaling{0}        // Scale
                    },
                    Scaling{1, 1}, // Max size
                    Scaling{0, 0}  // Min size
                },
                "Toggle start button visibility"));

        ui.AddElement(
            "start",
            new UIButton(
                UIPosition{
                    Vector2{0, 0}, // Offset position
                    Anchor{
                        Vector2{0.5f, 0},   // Local anchor
                        Vector2{0.5f, 0.5f} // Screen anchor
                    },
                    Size{
                        Scaling{500, 50}, // Offset
                        Scaling{0},       // Scale
                    },
                    Scaling{1, 1}, // Max size
                    Scaling{0, 0}  // Min size
                },
                "Play"));

        ui.AddElement(
            "maps",
            new UIButton(
                UIPosition{
                    Vector2{0, 60}, // Offset position
                    Anchor{
                        Vector2{0.5f, 0},   // Local anchor
                        Vector2{0.5f, 0.5f} // Screen anchor
                    },
                    Size{
                        Scaling{500, 50}, // Offset
                        Scaling{0},       // Scale
                    },
                    Scaling{1, 1}, // Max size
                    Scaling{0, 0}  // Min size
                },
                "Maps test"));

        ui.AddElement(
            "quit",
            new UIButton(
                UIPosition{
                    Vector2{0, 0}, // Offset position
                    Anchor{
                        Vector2{0, 1}, // Local anchor
                        Vector2{0, 1}  // Screen anchor
                    },
                    Size{
                        Scaling{0, 50}, // Offset
                        Scaling{1, 0},  // Scale
                    }},
                "Quit"));
    }

    // Game logic update after drawing
    void LateUpdate() override
    {
        UIButton *startButton = ui.GetElementById<UIButton>("start");
        UIButton *testButton = ui.GetElementById<UIButton>("test");
        UIButton *mapsButton = ui.GetElementById<UIButton>("maps");
        UIButton *quitButton = ui.GetElementById<UIButton>("quit");

        if (testButton->hasClicked())
        {
            startButton->setVisible(startButton->isVisible() == false);
            return;
        }

        if (quitButton->hasClicked())
        {
            CloseWindow();
            return;
        }

        if (startButton->hasClicked())
        {
            globalSceneManager.LoadScene("Test");
            return;
        }

        if (mapsButton->hasClicked())
        {
            globalSceneManager.LoadScene("MapsMenu");
            return;
        }
    }

    int tileSpeed = 20;
    Vector2 tileOffset = {0};
    void Draw() override
    {

        // While the offset is less than the width of the texture it'll be multiplied by 1
        // If its offset is more than the width it'll be multiplied by 0 thus resetting the position
        tileOffset = {
            (tileOffset.x <= tile_texture.width) * (tileOffset.x + (tileSpeed * GetFrameTime())),
            (tileOffset.y <= tile_texture.height) * (tileOffset.y + (tileSpeed * GetFrameTime()))};

        int tileWidthRepeats = (int)ceilf(GetScreenWidth() / tile_texture.width);
        int tileHeightRepeats = (int)ceilf(GetScreenHeight() / tile_texture.height);
        for (int x = 0; x <= tileWidthRepeats + 1; x++)
        {
            for (int y = 0; y <= tileHeightRepeats + 1; y++)
            {
                DrawTexture(
                    tile_texture,
                    tileOffset.x + ((x - 1) * tile_texture.width),
                    tileOffset.y + ((y - 1) * tile_texture.height),
                    WHITE);
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
    }

    void Unload() override
    {
        UnloadTexture(tile_texture);
    }
};
#endif