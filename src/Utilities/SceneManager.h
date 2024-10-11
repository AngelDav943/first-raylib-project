#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Scene.h"
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
            loadedScene->ui.Unload();
            loadedScene->Unload();
        }

        currentScene = SceneName;
        Scene *newScene = getScene(SceneName);
        if (newScene)
        {
            started = false;
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
            loadedScene->Draw();
            loadedScene->ui.Draw();
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