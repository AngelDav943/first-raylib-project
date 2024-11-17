#ifndef MAPSMENUSCENEH
#define MAPSMENUSCENEH

#include <string>
#include <list>
#include "../Scene.h"

#include "../Utilities/ElementsUI.h"

class MapsMenuScene : public Scene
{
private:
    Texture2D tile_texture;

public:
    void Start() override {
        tile_texture = LoadTexture("textures/white_hexagon_tiles.png");
        // FilePathList test = LoadDirectoryFiles("maps");

        ui.AddElement(
            "hasFolderTest",
            new UIButton(
                UIPosition{
                    .position = Vector2{}, 
                    .anchor = Anchor{ // Anchoring of element
                        Vector2{0, 0}, // local anchoring
                        Vector2{0, 0} // parent anchoring
                    }, // initial offset
                    .size = Size{ 
                        Scaling{0, 50}, // offset
                        Scaling{1, 0} // scale
                    } // Sizing of element
                },
                DirectoryExists("maps") ? "'maps' directory found" : "'maps' Directory doesn't exist"
            )
        );

#ifdef PLATFORM_DESKTOP
        ui.AddElement(
            "backMenu",
            new UIButton(
                UIPosition{
                    .position = Vector2{}, 
                    .anchor = Anchor{ // Anchoring of element
                        Vector2{0, 1}, // local anchoring
                        Vector2{0, 1} // parent anchoring
                    }, // initial offset
                    .size = Size{ 
                        Scaling{0, 50}, // offset
                        Scaling{1, 0} // scale
                    } // Sizing of element
                },
                "Back to menu"
            )
        );
#endif

        // SaveFileText("test.txt","Hello world");
    }

    int tileSpeed = 10;
    Vector2 tileOffset = {0};
    void Draw() override {
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

        FilePathList files = LoadDirectoryFiles("");
        list<string> texts = {
			(string)"Working directory: " + GetWorkingDirectory(),
            (string)"App directory:" + GetApplicationDirectory(),
            "Keypressed:" + to_string(GetKeyPressed())
		};

        for (int i = 0; i < files.count; i++)
        {
            texts.push_front(files.paths[i]);
        }

		int i = 0;
		for (string text : texts)
		{
			i++;
			DrawText(text.c_str(), 20, 55 + (i * 25), 20, ORANGE);
		}
    }
    void LateUpdate() override {
#ifdef PLATFORM_DESKTOP
        if (ui.GetElementById<UIButton>("backMenu")->hasClicked())
        {
            globalSceneManager.LoadScene("Menu");
            return;
        }
#else
        if (IsKeyPressed(KEY_BACK)) {
            globalSceneManager.LoadScene("Menu");
            return;
        }
#endif
    }
    
    void Unload() override {}
};
#endif