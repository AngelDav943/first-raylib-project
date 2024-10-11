#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "ElementsUI.h"
#include <map>

class UIManager
{
public:
    UIElement *AddElement(std::string identificator, UIElement *element)
    {
        elements.insert({identificator, element});
        return element;
    }

    template <typename T>
    T *GetElementById(std::string identificator)
    {
        if (elements.empty())
        {
            return nullptr;
        }

        if (elements.count(identificator) > 0)
        {
            UIElement* elementFound = elements.at(identificator);

            if (T* element = dynamic_cast<T*>(elementFound))
            {
                return element;
            }
        }
        return nullptr;
    }

    void Update()
    {
        for (auto elementPair : elements)
        {
            UIElement *element = elementPair.second;
            if (element && element->isVisible() == true)
            {
                element->Update();
            }
        }
    }

    void Draw()
    {
        for (auto elementPair : elements)
        {
            UIElement *element = elementPair.second;
            if (element != nullptr && element->isVisible() == true)
            {
                element->Draw();
            }
        }
    }

    void Unload()
    {
        for (auto elementPair : elements)
        {
            UIElement *element = elementPair.second;
            if (element)
            {
                element->Unload();
                delete element;
            }
        }
        elements.clear();
    }

private:
    map<std::string, UIElement *> elements;
    // std::vector<UIElement *> elements;
};

#endif