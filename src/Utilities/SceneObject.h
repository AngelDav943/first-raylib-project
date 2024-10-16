#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <raylib.h>
#include <raymath.h>
#include <optional>

class BaseSceneObject
{
private:
    BoundingBox boundBox;
    Vector3 rotation;

    float degreesToRadians(float radians)
    {
        return (radians * PI) / 180.0f;
    }

public:
    Model model;
    Vector3 position;

    BaseSceneObject(Model mdl) : model(mdl)
    {
        boundBox = GetModelBoundingBox(mdl);
    }
    virtual ~BaseSceneObject() {}

    /// @brief Returns the Bounding box of the Object's model taking into account the position
    BoundingBox getBoundingBox(std::optional<Vector3> overridePosition = std::nullopt)
    {
        Vector3 currentPosition =  position;
        if (overridePosition.has_value())
        {
            currentPosition = overridePosition.value();
        }
    
        return BoundingBox{
            Vector3Add(currentPosition, boundBox.min), // Minimum vertex box-corner
            Vector3Add(currentPosition, boundBox.max)  // Maximum vertex box-corner
        };
    }

    Vector3 getRotation()
    {
        return rotation;
    }

    Vector3 setRotation(Vector3 newRotation)
    {
        model.transform = MatrixRotateXYZ(Vector3{
            degreesToRadians(newRotation.x),
            degreesToRadians(newRotation.y),
            degreesToRadians(newRotation.z)
        });
        return newRotation;
    }

    virtual void Update() {}; // Logic before Draw
    virtual void Draw() = 0;   // Draw logic

};

class SceneObject : public BaseSceneObject
{
public:

    SceneObject(Model mdl, Vector3 initialPos = {0})
        : BaseSceneObject(mdl)
    {
        colorBase = WHITE;
        position = initialPos;
    }

    SceneObject(Model mdl, Vector3 initialPos, Color baseColor)
        : BaseSceneObject(mdl)
    {
        position = initialPos;
        colorBase = baseColor;
    }

    SceneObject(Model mdl, Vector3 initialPos, Texture2D MaterialTexture)
        : BaseSceneObject(mdl)
    {
        SetMaterialTexture(mdl.materials, MATERIAL_MAP_ALBEDO, MaterialTexture);
        position = initialPos;
        colorBase = WHITE;
    }

    void Update() override {}

    void Draw() override
    {
        DrawModel(model, position, 1, colorBase);
        DrawBoundingBox(getBoundingBox(), ORANGE);
    }

private:
    Color colorBase;
};

#endif