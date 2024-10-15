#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Scene.h"
#include "ObjectManager.h"
#include <map>
#include <memory>
#include <stack>

class SceneManager
{
public:
    Scene *getScene(string SceneName)
    {
        if (savedScenes.count(SceneName) > 0)
        {
            return savedScenes.at(currentScene).get();
        }
        return nullptr;
    }

    void LoadScene(string SceneName)
    {
        if (loaded)
        {
            Scene *loadedScene = savedScenes.at(currentScene).get();
            loadedScene->space.Unload();
            loadedScene->ui.Unload();
            loadedScene->Unload();
        }

        currentScene = SceneName;
        Scene *newScene = getScene(SceneName);
        if (newScene)
        {
            started = false;

            newScene->ui.Load();
            newScene->Start();
            loaded = true;
        }
    }

    void InsertScene(string sceneName, std::unique_ptr<Scene> newScene)
    {
        savedScenes.insert({sceneName, std::move(newScene)});
    }

    void Update()
    {
        if (!savedScenes.empty() && started == true)
        {
            Scene *loadedScene = savedScenes.at(currentScene).get();
            loadedScene->ui.Update();
            loadedScene->space.Update();
            loadedScene->Update();
        }
    }

    void LateUpdate()
    {
        if (!savedScenes.empty() && started == true)
        {
            Scene *loadedScene = savedScenes.at(currentScene).get();
            loadedScene->LateUpdate();
        }
    }

    void Draw()
    {
        if (!savedScenes.empty())
        {
            started = true;
            Scene *loadedScene = savedScenes.at(currentScene).get();
            loadedScene->DrawEarly();
            if (loadedScene->hasCamera() == true && loadedScene->camera3D != nullptr)
            {
                BeginMode3D(*(loadedScene->camera3D));
                loadedScene->Draw3D();
                loadedScene->space.Draw();
                EndMode3D();
            }
            loadedScene->Draw();
            loadedScene->ui.Draw();

            if (loadedScene->camera3D != nullptr && false) // for debugging camera and stuff
            {
                Camera3D current = *loadedScene->camera3D;
                list<string> texts = {
                    ("TargetX: " + to_string(current.target.x)),
                    ("TargetZ: " + to_string(current.target.z)),
                    ("X: " + to_string(current.position.x)),
                    ("Y: " + to_string(current.position.y)),
                    ("Z: " + to_string(current.position.z))};

                int i = 0;
                for (string text : texts)
                {
                    i++;
                    DrawText(text.c_str(), 20, 20 + (i * 25), 20, RED);
                }
            }
        }
    }

private:
    map<string, std::unique_ptr<Scene>> savedScenes;
    string currentScene;
    bool loaded = false;
    bool started = false;
};

// Global instance of SceneManager
SceneManager globalSceneManager;

#endif