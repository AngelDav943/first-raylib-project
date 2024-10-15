#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <map>
#include "SceneObject.h"

class ObjectManager
{
public:
    ObjectManager(map<string, SceneObject *> children = {})
        : objects(children) {}

    template <typename T>
    T *GetElementById(std::string identificator)
    {
        if (objects.empty())
        {
            return nullptr;
        }

        if (objects.count(identificator) > 0)
        {
            SceneObject* elementFound = objects.at(identificator);

            if (T* element = dynamic_cast<T*>(elementFound))
            {
                return element;
            }
        }
        return nullptr;
    }

    SceneObject *AddObject(string identificator, SceneObject *object)
    {
        objects.insert({identificator, object});
        return object;
    }

    void Update()
    {
        for (auto elementPair : objects)
        {
            SceneObject *element = elementPair.second;
            if (element)
            {
                element->Update();
            }
        }
    }

    void Draw()
    {
        for (auto elementPair : objects)
        {
            SceneObject *element = elementPair.second;
            if (element != nullptr)
            {
                element->Draw();
            }
        }
    }

    void Unload()
    {
        for (auto elementPair : objects)
        {
            SceneObject *element = elementPair.second;
            delete element;
        }
        objects.clear();
    }

    map<string, SceneObject *> getObjects()
    {
        return objects;
    }

private:
    map<string, SceneObject *> objects;
};

#endif