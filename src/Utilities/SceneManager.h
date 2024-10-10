#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Scene.h"
#include <memory>
#include <stack>

class SceneManager
{
public:
    void ChangeScene(std::unique_ptr<Scene> newScene)
    {
        if (!scenes.empty())
        {
            scenes.top()->Unload();
            scenes.top()->ui.Unload();
        }
        scenes.push(std::move(newScene));
        scenes.top()->Start();
    }

    void Update()
    {
        if (!scenes.empty())
        {
            scenes.top()->ui.Update();
            scenes.top()->Update();
        }
    }

    void Draw()
    {
        if (!scenes.empty())
        {
            scenes.top()->Draw();
            scenes.top()->ui.Draw();
        }
    }

private:
    std::stack<std::unique_ptr<Scene>> scenes;
};

// Global instance of SceneManager
SceneManager globalSceneManager;

#endif