#include "../Scene.h"

#include <list>
#include <raylib.h>
#include <raymath.h>

#include "../Utilities/CameraController.h"
#include "../Utilities/PauseMenuController.h"

#include "../Utilities/ElementsUI.h"
#include "../Utilities/SceneObject.h"

class TestScene : public Scene
{
private:
    const Color background = {20, 160, 133, 255};

    CameraController CamController;
    PauseMenuController PauseController;

    Texture2D book_texture;
    Texture2D tile_texture;

    Vector2 center;

public:
    // Initialize game assets
    void Start() override
    {
        // Calculate the center position
        center = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        // Set the mouse position to the center
        if (IsCursorOnScreen())
        {
            SetMousePosition(center.x, center.y);
            DisableCursor();
        }

        CamController = CameraController();
        SetMainCamera(&(CamController.currentCamera));

        // Loads textures
        book_texture = LoadTexture("assets/textures/book_texture.png");
        tile_texture = LoadTexture("assets/textures/blueprint_tiles.png");

        // Loads the pause menu
        PauseController.init(ui);

        space.AddObject(
            "cone_test",
            new SceneObject(
                LoadModelFromMesh(GenMeshCone(5, 5, 7)), // model
                Vector3{10, 0, 10}                       // initial position
                ));
        space.GetElementById<SceneObject>("cone_test")->model.materials->maps->texture = tile_texture;

        space.AddObject(
                 "baseplane",
                 new SceneObject(
                     LoadModelFromMesh(GenMeshPlane(50, 50, 1, 1)), // model
                     Vector3{0.0f, -2.0f, 0.0f}                     // initial position
                     ))
            ->model.materials[0]
            .maps[MATERIAL_MAP_DIFFUSE]
            .texture = tile_texture;

        space.AddObject(
            "cube1", // name
            new SceneObject(
                LoadModelFromMesh(GenMeshCube(0.5f, 0.5f, 0.5f)), // model
                Vector3{5.0f, -1.75f, 5.0f},                      // initial position
                ORANGE                                            // base color
                ));

        space.AddObject(
            "cube2", // name
            new SceneObject(
                LoadModelFromMesh(GenMeshCube(1, 1, 1)), // model
                Vector3{-5.0f, -1.5f, -5.0f},            // initial position
                RED                                      // base color
                ));

        space.AddObject(
                 "book",
                 new SceneObject(LoadModel("assets/models/book.obj")))
            ->model.materials[0]
            .maps[MATERIAL_MAP_DIFFUSE]
            .texture = book_texture;

        
    }

    void Update() override
    {
        if (IsCursorHidden())
        {
            CamController.Update();
            SetMousePosition(center.x, center.y); // Set the mouse position to the center
        }

        // Toggles camera controls
        /*if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if (IsCursorHidden())
                EnableCursor();
            else
                DisableCursor();
        }*/
    }

    // Game logic update after Drawing
    void LateUpdate() override
    {
        PauseController.UpdateMenu(ui, globalSceneManager);
    }

    void DrawEarly() override
    {
        DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), background);
    }

    void Draw() override
    {

        DrawRectangle(50, GetScreenHeight() - 150, 100, 100, ORANGE);

        list<string> texts = {
            ("Hello world! FPS:" + to_string(GetFPS())),
            ("TargetX: " + to_string(CamController.currentCamera.target.x)),
            ("TargetZ: " + to_string(CamController.currentCamera.target.z)),
            ("AngleY: " + to_string(CamController.angleY)),
            ("X: " + to_string(CamController.currentCamera.position.x)),
            ("Y: " + to_string(CamController.currentCamera.position.y)),
            ("Z: " + to_string(CamController.currentCamera.position.z))};

        int i = 0;
        for (string text : texts)
        {
            i++;
            DrawText(text.c_str(), 20, 20 + (i * 25), 20, ORANGE);
        }
    }

    // Unload game assets
    void Unload() override
    {
        UnloadTexture(book_texture);
        UnloadTexture(tile_texture);
    }
};