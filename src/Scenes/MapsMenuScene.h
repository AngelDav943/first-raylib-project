#ifndef MAPSMENUSCENEH
#define MAPSMENUSCENEH

#include "../Scene.h"

#include "../Utilities/ElementsUI.h"

class MapsMenuScene : public Scene
{
private:
    Texture2D tile_texture;

public:
    void Start() override {
        tile_texture = LoadTexture("assets/textures/white_hexagon_tiles.png");
        // FilePathList test = LoadDirectoryFiles("assets/maps");

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
                DirectoryExists("assets/maps") ? "'assets/maps' directory found" : "'assets/maps' Directory doesn't exist"
            )
        );

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
    }
    void LateUpdate() override {
        if (ui.GetElementById<UIButton>("backMenu")->hasClicked())
        {
            globalSceneManager.LoadScene("Menu");
            return;
        }
    }
    
    void Unload() override {}
};
#endif