#ifndef SCENE_H
#define SCENE_H

#include "Utilities/UIManager.h"

class Scene
{
public:
    UIManager ui;
    virtual ~Scene() {}
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void LateUpdate() = 0;
    virtual void Draw() = 0;
    virtual void Unload() = 0;
};

#endif // SCENE_H