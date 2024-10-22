#ifndef SCENEPHYSICSOBJECT_H
#define SCENEPHYSICSOBJECT_H

#include <map>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include "../ObjectManager.h"
#include "../SceneObject.h"

class PhysicsObject: public SceneObject {
private:
    ObjectManager* spaceManager;
    Vector3 lastHit;

public:
    PhysicsObject(Model mdl, ObjectManager* space, Vector3 initialPos = {0})
        : SceneObject(mdl, initialPos), spaceManager(space)
    {
    }

    void OnCollision(BaseSceneObject &hit)
    {
        lastHit = hit.getClosestCorner(position);
    }

    void Update() override {
        Vector3 newPosition = Vector3Subtract(position, {0, 1 * GetFrameTime(), 0});

        bool canMove = true;
        map<string, SceneObject *> objects = spaceManager->getObjects();
        for (auto objectPair: objects)
        {
			if (objectPair.second == this) {
				continue;
			}

            // bool hasCollided = CheckCollisionBoxes(getBoundingBox(newPosition), objectPair.second->getBoundingBox());
            bool hasCollided = checkCollision(*(objectPair.second));
            if (hasCollided)
            {
                OnCollision(*(objectPair.second));
                canMove = false;
            }
        }

        if (canMove) {
            position = newPosition;
        }
    }

    void Draw() override {
        SceneObject::Draw();

        DrawSphere(lastHit, 0.1f, BLUE);
    }
};
#endif