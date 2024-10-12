#ifndef SCENE_H
#define SCENE_H

#include "Utilities/UIManager.h"
#include "Utilities/ObjectManager.h"

/// @brief 
class Scene
{
public:
    UIManager ui;
    ObjectManager space;
    Camera3D* camera3D = nullptr;

    void SetMainCamera(Camera3D* cam)
    {
        camera3D = cam;
        using3DCamera = true;
    }

    bool hasCamera()
    {
        return using3DCamera;
    }

    virtual ~Scene() {}

    /// @brief Runs when the scene is loaded
    virtual void Start() = 0; // Initialize scene

    /// @brief Runs before the drawing functions
    virtual void Update() = 0;

    /// @brief Drawing logic inside a BeginDrawing before Draw3D
    virtual void DrawEarly() = 0; // Drawing logic

    /// @brief Drawing logic inside a BeginMode3D
    virtual void Draw3D() = 0;

    /// @brief Drawing logic inside a BeginDrawing after Draw3D
    virtual void Draw() = 0; // Drawing logic

    /// @brief Runs after the Drawing functions
    virtual void LateUpdate() = 0; // Logic after Draw

    /// @brief Runs when unloading the scene
    virtual void Unload() = 0; // Logic when scene unloaded

private:
    bool using3DCamera = false;
};

#endif // SCENE_H
