#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "ElementsUI.h"
#include <map>
#include <vector>

class UIManager
{
private:
    vector<UIElement *> elements;
    bool initialized = false;
    unsigned int elementIndex = 0;

public:
    /// @brief Inserts an element to the UI Manager
    /// @param identificator Name used for identification (can't be two elements with the same id)
    /// @param element Element devired from the class UIElement
    /// @return Returns the element added
    UIElement *AddElement(std::string identificator, UIElement *element)
    {
        // elements.insert({identificator, element});
        element->identificatorName = identificator;
        elements.push_back(element);
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

        /*if (elements.count(identificator) > 0)
        {
            UIElement* elementFound = elements.at(identificator);

            if (T* element = dynamic_cast<T*>(elementFound))
            {
                return element;
            }
        }*/

        for (long long unsigned int i = 0; i < elements.size(); i++)
        {
            UIElement *currentElement = elements[i];
            if (currentElement->identificatorName == identificator)
            {
                if (T *element = dynamic_cast<T *>(currentElement))
                {
                    return element;
                }
            }
        }

        return nullptr;
    }

    void Update()
    {
        bool isHovering = false;
        for (long long unsigned int i = 0; i < elements.size(); i++)
        {
            UIElement *element = elements[i];
            if (element && element->isVisible() == true)
            {
                if (element->IsMouseOver())
                {
                    isHovering = true;
                }
                element->Update();
            }
        }

        if (isHovering == false)
        {
            currentHovering = 0;
        }
    }

    void Draw()
    {
        for (int i = 0; i < elements.size(); i++)
        {
            UIElement *element = elements[i];
            if (element != nullptr && element->isVisible() == true)
            {
                element->BaseDraw(i);
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
        for (int i = 0; i < elements.size(); i++)
        {
            UIElement *element = elements[i];
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
};

#endif