#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "ElementsUI.h"
#include <map>

class UIManager
{
public:
    /// @brief Inserts an element to the UI Manager
    /// @param identificator Name used for identification (can't be two elements with the same id)
    /// @param element Element devired from the class UIElement
    /// @return Returns the element added
    UIElement *AddElement(std::string identificator, UIElement *element)
    {
        elements.insert({identificator, element});
        return element;
    }

    /// @brief Gets an element by the requested id
    /// @tparam T A derived class from UIElement
    /// @param identificator Name identificator
    /// @return The element requested that matches their id with the requested one
    template <typename T>
    T *GetElementById(std::string identificator)
    {
        if (elements.empty() || initialized == false)
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

    /// @brief Sets the initialized mode to true, enabling the `GetElementById`
    void Load()
    {
        initialized = true;
    }

    /// @brief Unloads the ui by deleting all the elements loaded and setting the initialized mode to false, disabling the `GetElementById`
    void Unload()
    {
        for (auto elementPair : elements)
        {
            UIElement *element = elementPair.second;
            if (element)
            {
                element->BaseUnload();
                element->Unload();
                delete element;
            }
        }
        elements.clear();
        initialized = false;
    }

private:
    map<std::string, UIElement *> elements;
    bool initialized = false;
};

#endif