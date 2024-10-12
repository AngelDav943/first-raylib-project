#include "../Scene.h"

#include <list>
#include <raylib.h>
#include <raymath.h>

#include "../Utilities/CameraController.h"
#include "../Utilities/ElementsUI.h"
#include "../Utilities/SceneObject.h"

class TestScene : public Scene
{
private:
    const Color background = {20, 160, 133, 255};

    CameraController CamController;

    Texture2D book_texture;
    Texture2D tile_texture;

    // Model book;
    // Vector3 bookPosition;
    // BoundingBox bookBBox;

    Model plane;
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

        // Loads the book's models
        // book = LoadModel("assets/models/book.obj");
        // bookPosition = {0.0f, 0.0f, 0.0f};
        // book.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = book_texture;

        // space.AddObject("book", new SceneObject(LoadModel("assets/models/book.obj")) );
        // space.AddObject("book", new SceneObject( LoadModelFromMesh(GenMeshCone(10, 10, 5)) ) );

        space.AddObject(
            "book",
            new SceneObject( LoadModel("assets/models/book.obj") )
        );
        space.GetElementById<SceneObject>("book")->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = book_texture;

        plane = LoadModelFromMesh(GenMeshPlane(50, 50, 1, 1));
        plane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tile_texture;

        ui.AddElement(
              "pauseMenu",
              new UIContainer(
                  UIPosition{
                      Vector2{0, 0}, // Offset position
                      Anchor{
                          Vector2{0.5f, 0.5f}, // Local anchor
                          Vector2{0.5f, 0.5f}  // Screen anchor
                      },
                      Size{
                          Scaling{500, 0}, // Offset
                          Scaling{0, 1}    // Scale
                      },
                      Scaling{1, 1}, // Max size
                      Scaling{0, 0}  // Min size
                  },
                  // Children
                  {{"resumeButton",
                    new UIButton(
                        UIPosition{
                            Vector2{0, -5}, // Offset position
                            Anchor{
                                Vector2{0.0f, 1},    // Local anchor
                                Vector2{0.0f, 0.5f}, // Screen anchor
                            },
                            Size{
                                Scaling{0, 50}, // Offset
                                Scaling{1, 0}   // Scale
                            }},
                        "Resume")},
                   {"menuButton",
                    new UIButton(
                        UIPosition{
                            Vector2{0, 5}, // Offset position
                            Anchor{
                                Vector2{0.5f, 0},    // Local anchor
                                Vector2{0.5f, 0.5f}, // Screen anchor
                            },
                            Size{
                                Scaling{0, 50}, // Offset
                                Scaling{1, 0}   // Scale
                            }},
                        "Back to menu")}}))
            ->setVisible(false);
    }

    void Update() override {
        if (IsCursorHidden())
        {
            CamController.Update();
            SetMousePosition(center.x, center.y); // Set the mouse position to the center
        }

        // Toggles camera controls
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if (IsCursorHidden())
                EnableCursor();
            else
                DisableCursor();
        }
    }

    // Game logic update after Drawing
    void LateUpdate() override
    {

        UIContainer *pauseContainer = ui.GetElementById<UIContainer>("pauseMenu");
        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (pauseContainer->isVisible() == false)
            {
                EnableCursor();
                pauseContainer->setVisible(true);
            }
            else
            {
                DisableCursor();
                pauseContainer->setVisible(false);
            }
        }

        // Pause menu logic
        if (pauseContainer->getElementById("resumeButton")->hasClicked())
        {
            DisableCursor();
            pauseContainer->setVisible(false);
        }

        if (pauseContainer->getElementById("menuButton")->hasClicked())
        {
            globalSceneManager.LoadScene("Menu");
        }
    }

    void DrawEarly() override {
        DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), background);
    }

    void Draw() override {

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

    void Draw3D() override
    {
        DrawModel(plane, (Vector3){0.0f, -2.0f, 0.0f}, 1.0f, WHITE);

        // DrawCube(camera.target, 0.2f, 0.2f, 0.2f, BLUE);

        DrawCube((Vector3){5.0f, -1.75f, 5.0f}, 0.5f, 0.5f, 0.5f, ORANGE);
        DrawCube((Vector3){-5.0f, -1.5f, -5.0f}, 1.0f, 1.0f, 1.0f, RED);
        // DrawModel(book, bookPosition, 1.0f, WHITE); // Draws the book model

        // bookObject->Draw();
        // bookObject->position.y = bookObject->position.y - (1 * GetFrameTime());
        // BoundingBox bookBBox = bookObject->getBoundingBox();
        // DrawBoundingBox(bookBBox, RED);
    }

    // Unload game assets
    void Unload() override
    {
        // delete bookObject;
        // UnloadModel(book);
        UnloadModel(plane);
        UnloadTexture(book_texture);
        UnloadTexture(tile_texture);
    }
};