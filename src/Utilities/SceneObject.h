#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <raylib.h>
#include <raymath.h>

class BaseSceneObject
{
public:
    Model model;
    Vector3 position;

    BaseSceneObject(Model mdl) : model(mdl)
    {
        boundBox = GetModelBoundingBox(mdl);
    }
    virtual ~BaseSceneObject() {}

    /// @brief Returns the Bounding box of the Object's model taking into account the position
    BoundingBox getBoundingBox()
    {
        return BoundingBox{
            Vector3Add(position, boundBox.min), // Minimum vertex box-corner
            Vector3Add(position, boundBox.max)  // Maximum vertex box-corner
        };
    }

    virtual void Update() = 0; // Logic before Draw
    virtual void Draw() = 0; // Draw logic

private:
    BoundingBox boundBox;
};

class SceneObject : public BaseSceneObject
{
public:
    SceneObject(Model mdl) : BaseSceneObject(mdl) {}

    void Update() override {}

    void Draw() override
    {
        DrawModel(model, position, 1, WHITE);
    }

private:
};

#endif