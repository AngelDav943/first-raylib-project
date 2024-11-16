#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include <iostream>

#include <math.h>
#include <string>
#include <cstring>
#include <memory>
#include <raylib.h>
#include <vector>
#include <map>

unsigned currentHovering = 0;

float clamp(const float &value, const float &min, const float &max)
{
    return (value < min) ? min : (value > max) ? max
                                               : value;
}

struct Scaling
{
    float width;
    float height;

    Scaling(float w = 0, float h = 0) : width(w), height(h) {}
};

/// @brief The sizing for the UI Element ( offset in pixels and scale in percentage relative to parent size )
/// @param offset Offset sizing based in pixels
/// @param scale Scale sizing based on the percentage (0 to 1) of the size covered by the parent element
struct Size
{
    Scaling offset;
    Scaling scale;
};

/// @brief The anchor position for the element ( screen anchor and local anchor )
/// @param local Anchors relatively to the UI Element
/// @param screen Anchors relatively the UI Element to the parent Element
struct Anchor
{
    Vector2 local;
    Vector2 screen;
};

/// @brief Data needed for positioning a UI Element into the screen
/// @param position The offset position in pixels
/// @param anchor The anchor position for the element ( screen anchor and local anchor )
/// @param size The sizing for the element (offset in pixels and scale in percentage relative to parent size)
/// @param maxSize The maximum size an element can take
/// @param minSize The minimum size an element can take
struct UIPosition
{
    Vector2 position;
    Anchor anchor;
    Size size;

    Scaling maxSize = {1, 1};
    Scaling minSize = {0, 0};
};

class UIElement
{
public:
    string identificatorName;
    UIPosition location;

    UIElement(UIPosition startLocation, bool isVisible = true)
        : location(startLocation), visible(isVisible), isLoaded(true) {}
    virtual ~UIElement() {}

    virtual void Draw() = 0;

    virtual void BaseDraw(unsigned int zIndex)
    {
        drawnIndex = zIndex;
        Draw();
    }

    virtual void Update() {}
    virtual void Unload() {}

    void BaseUnload()
    {
        isLoaded = false;
    }

    void setParentBounds(UIElement *parent)
    {
        if (isLoaded == false)
            return;
        parentBounds = parent;
    }

    Rectangle getBounds()
    {
        if (isLoaded == false)
            return {};

        Rectangle parentRectangle = {0, 0, static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};
        if (parentBounds != nullptr)
        {
            parentRectangle = parentBounds->getBounds();
        }

        Anchor uiAnchor = location.anchor;
        Size uiSize = location.size;

        float screenXscale = parentRectangle.width * clamp(uiSize.scale.width, 0, 1);
        float screenYscale = parentRectangle.height * clamp(uiSize.scale.height, 0, 1);

        Scaling resultSize = {
            screenXscale + uiSize.offset.width,
            screenYscale + uiSize.offset.height};

        if (location.maxSize.width != -1 && location.minSize.width != -1)
        {
            resultSize = {
                clamp(screenXscale + uiSize.offset.width, parentRectangle.width * location.minSize.width, parentRectangle.width * location.maxSize.width),
                clamp(screenYscale + uiSize.offset.height, parentRectangle.height * location.minSize.height, parentRectangle.height * location.maxSize.width)};
        }

        float screenXAnchor = parentRectangle.width * clamp(uiAnchor.screen.x, 0, 1);
        float screenYAnchor = parentRectangle.height * clamp(uiAnchor.screen.y, 0, 1);

        float localXAnchor = resultSize.width * clamp(uiAnchor.local.x, 0, 1);
        float localYAnchor = resultSize.height * clamp(uiAnchor.local.y, 0, 1);

        return {
            screenXAnchor - localXAnchor + location.position.x + parentRectangle.x,
            screenYAnchor - localYAnchor + location.position.y + parentRectangle.y,
            resultSize.width,
            resultSize.height};
    }

    bool IsMouseOver()
    {
#ifdef PLATFORM_DESKTOP
        if (IsCursorOnScreen() == false) {
            return false;
        }
#else
        if (GetTouchPointCount() <= 0) {
            return false;
        }
#endif

        if (isVisible() == false || isLoaded == false || currentHovering > drawnIndex)
        {
            return false;
        }

        Vector2 mousePos = GetTouchPosition(0);
        Rectangle bounds = getBounds();

        bool isOver = (mousePos.x >= bounds.x && mousePos.x <= bounds.x + bounds.width &&
                       mousePos.y >= bounds.y && mousePos.y <= bounds.y + bounds.height);

        if (isOver)
        {
            currentHovering = drawnIndex;
        }

        return isOver;
    }

    bool hasClicked()
    {
        if (isLoaded == false)
            return false;
        // return IsMouseOver() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        return IsMouseOver() && IsGestureDetected(GESTURE_TAP);
    }

    bool isVisible()
    {
        if (isLoaded == false)
            return false;

        if (parentBounds != nullptr)
        {
            return parentBounds->isVisible();
        }
        return visible;
    }

    bool setVisible(bool status)
    {
        // std::cout << "HELLO CONSOLE" << status << std::endl;
        visible = status;
        return status;
    }

private:
    bool visible = true;
    UIElement *parentBounds = nullptr;

protected:
    unsigned int drawnIndex = 0;
    bool isLoaded = false;
};

struct ButtonColoring
{
    Color background;
    Color hoverBackground;
    Color text;
};

class UIButton : public UIElement
{
public:
    UIButton(UIPosition startLocation, const char *text, ButtonColoring coloring = {LIGHTGRAY, DARKGRAY, BLACK})
        : UIElement(startLocation), text(text), btnColoring(coloring)
    {
    }

    void Draw() override
    {

        Scaling buttonSizeOffset = location.size.offset;
        Color backgroundColor = IsMouseOver() ? btnColoring.hoverBackground : btnColoring.background;
        int textLength = static_cast<int>(strlen(text));
        int buttonWidth = static_cast<int>(buttonSizeOffset.width);

        if (buttonSizeOffset.width < buttonSizeOffset.height * textLength)
        {
            buttonWidth = buttonSizeOffset.height * textLength;
            buttonSizeOffset.width = buttonWidth;
        }

        Rectangle buttonBounds = getBounds();

        DrawRectangle(
            buttonBounds.x, buttonBounds.y,
            buttonBounds.width, buttonBounds.height,
            backgroundColor);
        DrawText(text, buttonBounds.x + 10, buttonBounds.y + 10, buttonBounds.height - 20, btnColoring.text);
    }

private:
    const char *text;
    ButtonColoring btnColoring;
};

class UIContainer : public UIElement
{
private:
    vector<UIElement *> children;

public:
    UIContainer(UIPosition startLocation, const map<std::string, UIElement *> elements)
        : UIElement(startLocation)
    {
        for (auto elementPair : elements)
        {
            if (elementPair.second)
            {
                elementPair.second->identificatorName = elementPair.first;
                children.push_back(elementPair.second);
                elementPair.second->setParentBounds(this);
            }
        }
    }

    template <typename T>
    T *getElementById(std::string identificator)
    {
        if (children.empty())
        {
            return nullptr;
        }

        for (long long unsigned int i = 0; i < children.size(); i++)
        {
            UIElement *currentElement = children[i];
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

    void Draw() override
    {
        Color backgroundColor = Color{0, 0, 0, 100};
        Rectangle buttonBounds = getBounds();

        DrawRectangle(
            buttonBounds.x, buttonBounds.y,
            buttonBounds.width, buttonBounds.height,
            backgroundColor);

        for (long long unsigned int i = 0; i < children.size(); i++)
        {
            UIElement *element = children[i];
            if (element && element->isVisible() == true)
            {
                element->BaseDraw(drawnIndex);
            }
        }
    }

    void Update() override
    {
        for (long long unsigned int i = 0; i < children.size(); i++)
        {
            UIElement *element = children[i];
            if (element && element->isVisible() == true)
            {
                element->Update();
            }
        }
    }

    void Unload() override
    {
        for (long long unsigned int i = 0; i < children.size(); i++)
        {
            UIElement *element = children[i];
            element->Unload();
            delete element;
        }
        children.clear();
    }
};

#endif