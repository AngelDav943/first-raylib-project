#include <bits/stdc++.h>
#include <raylib.h>
#include <raymath.h>
using namespace std;

Camera camera = {0};
float camera_speed = 6.0f;
float camera_sensitivity = 0.25f;

float angleX = 0;
float angleY = 0;

void UpdateCameraControls()
{
    float deltaTime = GetFrameTime();
    Vector3 direction = Vector3Normalize({
        (float)(IsKeyDown(KEY_A) - IsKeyDown(KEY_D)),
        (float)(IsKeyDown(KEY_E) - IsKeyDown(KEY_Q)),
        (float)(IsKeyDown(KEY_W) - IsKeyDown(KEY_S))
    });

    Vector2 mouseDelta = GetMouseDelta();
    angleX -= camera_sensitivity * mouseDelta.x * deltaTime;

    float newAngleY = angleY - (camera_sensitivity * mouseDelta.y * deltaTime);
    if (abs(newAngleY) < 3.0f) {
        angleY = newAngleY;
    }

    float targetX = sin(angleX) + (cos(angleY) * sin(angleX) );
    float targetZ = cos(angleX) + (cos(angleY) * cos(angleX) );
    float targetY = sin(angleY);

    // float upZ = sin(angleX);

    Vector3 right = Vector3Normalize(Vector3CrossProduct(camera.up, {targetX, targetY, targetZ}));

    Vector3 frontMov = Vector3Scale({targetX, targetY, targetZ}, direction.z);
    Vector3 rightMov = Vector3Scale(right, direction.x);
    Vector3 upMov    = Vector3Scale(camera.up, direction.y);

    camera.position = Vector3Add(
        camera.position,
        Vector3Scale(
            Vector3Add(upMov, Vector3Add(frontMov, rightMov)),
            camera_speed * deltaTime
        )
    );

    // camera.position.x += direction.x * camera_speed * deltaTime;
    // camera.position.y += direction.y * camera_speed * deltaTime;
    // camera.position.z += direction.z * camera_speed * deltaTime;

    camera.target = Vector3Add(camera.position, {targetX, targetY, targetZ});

    // camera.target = camera.position;
    // camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    // camera.
}

int main()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "An amazing project");
    SetWindowMinSize(screenWidth/2, screenHeight/2);

    // Calculate the center position
    Vector2 center = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

    // Set the mouse position to the center
    SetMousePosition(center.x, center.y);

    // camera.position = (Vector3){10.0f, 10.0f, 8.0f};
    camera.position = (Vector3){0.0f, 2.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Loads textures
    Texture2D book_texture = LoadTexture("assets/textures/book_texture.png");
    Texture2D tiles_texture = LoadTexture("assets/textures/blueprint_tiles.png");

    // Loads the book's models
    Model book = LoadModel("assets/models/book.obj");
    Vector3 bookPosition = {0.0f, 0.0f, 0.0f};
    book.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = book_texture;

    Model plane = LoadModelFromMesh(GenMeshPlane(50, 50, 1, 1));
    plane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tiles_texture;

    SetTargetFPS(60);

    const Color darkGreen = {20, 160, 133, 255};

    Vector2 mouseDelta = GetMouseDelta();
    while (!WindowShouldClose())
    {
        SetExitKey(KEY_NULL);

        if (IsCursorHidden())
        {
            mouseDelta = GetMouseDelta();
            UpdateCameraControls(); 
            SetMousePosition(center.x, center.y); // Set the mouse position to the center
        }

        // Toggles camera controls
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_ESCAPE))
        {
            if (IsCursorHidden())
                EnableCursor();
            else
                DisableCursor();
        }

        ClearBackground(darkGreen);
        BeginMode3D(camera); // Started 3D Drawing

        DrawModel(plane, (Vector3){0.0f,-2.0f,0.0f}, 1.0f, WHITE);
        
        // DrawCube(camera.target, 0.2f, 0.2f, 0.2f, BLUE);

        DrawCube((Vector3){5.0f, -1.75f, 5.0f}, 0.5f, 0.5f, 0.5f, ORANGE);
        DrawCube((Vector3){-5.0f, -1.5f, -5.0f}, 1.0f, 1.0f, 1.0f, RED);
        DrawModel(book, bookPosition, 1.0f, WHITE); // Draws the book model

        EndMode3D(); // Stopped 3D Drawing

        BeginDrawing();
        DrawRectangle(50, GetScreenHeight() - 150, 100, 100, ORANGE);


        int textsCount = 7;
        string texts[textsCount] = {
            ("Hello world! FPS:" + to_string(GetFPS())),
            ("TargetX: " + to_string(camera.target.x)),
            ("TargetZ: " + to_string(camera.target.z)),
            ("AngleY: " + to_string(angleY)),
            ("X: " + to_string(camera.position.x)),
            ("Y: " + to_string(camera.position.y)),
            ("Z: " + to_string(camera.position.z))
        };

        for (int i = 0; i < textsCount; i++)
        {
            DrawText(texts[i].c_str(), 20, 20 + (i * 25), 20, ORANGE);
        }

        EndDrawing();
    }

    CloseWindow();
}