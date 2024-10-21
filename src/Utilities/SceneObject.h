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

    float degreesToRadians(float radians) const
    {
        return (radians * PI) / 180.0f;
    }

    void getOBBVertices(Vector3 *vertices)
    {
        Vector3 halfSize = {
            (boundBox.max.x - boundBox.min.x) * 0.5f,
            (boundBox.max.y - boundBox.min.y) * 0.5f,
            (boundBox.max.z - boundBox.min.z) * 0.5f,
        };

        Vector3 localVertices[8] = {
            {-halfSize.x, -halfSize.y, -halfSize.z},
            {-halfSize.x, -halfSize.y, -halfSize.z},
            {halfSize.x, halfSize.y, -halfSize.z},
            {halfSize.x, halfSize.y, -halfSize.z},
            {-halfSize.x, -halfSize.y, halfSize.z},
            {halfSize.x, -halfSize.y, halfSize.z},
            {halfSize.x, halfSize.y, halfSize.z},
            {-halfSize.x, halfSize.y, halfSize.z},
        };

        for (int i = 0; i < 8; i++)
        {
            Vector3 rotatedVertex = {localVertices[i].x, localVertices[i].y, localVertices[i].z};
            
            Matrix rotationMatrix = MatrixRotateXYZ({
                degreesToRadians(rotation.x),
                degreesToRadians(rotation.y),
                degreesToRadians(rotation.z),
            });
            rotatedVertex = Vector3Transform(rotatedVertex, rotationMatrix);

            // Translate the vertices to its final position
            vertices[i] = Vector3Add(rotatedVertex, position);
        }
    }

    void ProjectVertices(const Vector3 *vertices, const Vector3 &axis, float &min, float &max) const
    {
        min = max = Vector3DotProduct(vertices[0], axis);

        for (int i = 1; i < 8; i++)
        {
            float projection = Vector3DotProduct(vertices[i], axis);
            if (projection < min)
            {
                min = projection;
            }
            if (projection > max)
            {
                max = projection;
            }
        }
    }

    bool CheckOBBCollision(const Vector3 *verticesA, const Vector3 *verticesB) const
    {
        std::vector<Vector3> axes; // Axes to test

        // OBB A axes
        axes.push_back(Vector3Subtract(verticesA[1], verticesA[0])); // A.x
        axes.push_back(Vector3Subtract(verticesA[3], verticesA[0])); // A.y
        axes.push_back(Vector3Subtract(verticesA[4], verticesA[0])); // A.z

        // OBB B axes
        axes.push_back(Vector3Subtract(verticesB[1], verticesB[0])); // B.x
        axes.push_back(Vector3Subtract(verticesB[3], verticesB[0])); // B.y
        axes.push_back(Vector3Subtract(verticesB[4], verticesB[0])); // B.z

        // Cross products
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                axes.push_back(Vector3CrossProduct(axes[i], axes[3 + j]));
            }
        }

        for (const auto &axis : axes)
        {
            // Skipping if length is zero
            if (Vector3Length(axis) == 0)
                continue;

            float minA, maxA;
            ProjectVertices(verticesA, axis, minA, maxA);

            float minB, maxB;
            ProjectVertices(verticesB, axis, minB, maxB);

            // Check for overlap
            if (maxA < minB || maxB < minA)
            {
                return false; // No collision
            }
        }

        return true; // Collision detected
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
        Vector3 currentPosition = position;
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
            degreesToRadians(newRotation.z)});

        rotation = {
            newRotation.x,
            newRotation.y,
            newRotation.z,
        };

        return newRotation;
    }

    bool checkCollision(BaseSceneObject &other)
    {
        Vector3 verticesA[8], verticesB[8];
        getOBBVertices(verticesA);
        other.getOBBVertices(verticesB);

        return CheckOBBCollision(verticesA, verticesB);
    }

    virtual void Update() {}; // Logic before Draw

    void DrawOBBVerts() {
        Vector3 verticesA[8];
        getOBBVertices(verticesA);

        for (int i = 0; i < 8; i++)
        {
            DrawPoint3D(verticesA[i], BLACK);
        }
    }

    virtual void Draw() = 0;  // Draw logic
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
        // DrawBoundingBox(getBoundingBox(), ORANGE);
        DrawOBBVerts();
    }

private:
    Color colorBase;
};

#endif