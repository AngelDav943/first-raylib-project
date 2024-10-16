#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include <iostream>

#include <math.h>
#include <list>
#include <map>
#include <string>
#include <cstring>
#include <memory>
#include <raylib.h>

float clamp(const float &value, const float &min, const float &max)
{
    return (value < min) ? min : (value > max) ? max
                                               : value;
}

struct Scaling
{
    float width;
    float height;
};

/// @brief The sizing for the UI Element ( offset in pixels and scale in percentage relative to parent size )
struct Size
{
    Scaling offset;
    Scaling scale;

    /// @brief Sizing for the UI Element
    /// @param offsetSize Offset sizing based in pixels
    /// @param scaleSize Scale sizing based on the percentage (0 to 1) of the size covered by the parent element
    Size(Scaling offsetSize, Scaling scaleSize)
        : offset(offsetSize), scale(scaleSize) {}
};

/// @brief The anchor position for the element ( screen anchor and local anchor )
struct Anchor
{
    Vector2 local;
    Vector2 screen;

    /// @brief Anchoring of the UI element
    /// @param localAnc Anchors relatively to the UI Element
    /// @param screenAnc Anchors relatively the UI Element to the parent Element
    Anchor(Vector2 localAnc, Vector2 screenAnc)
        : local(localAnc), screen(screenAnc) {}
};

/// @brief Data needed for positioning a UI Element into the screen
struct UIPosition
{
    Vector2 position;
    Anchor anchor;
    Size size;

    Scaling maxSize;
    Scaling minSize;

    /// @brief Positioning for a UI element
    /// @param offset The offset position in pixels
    /// @param anchoring The anchor position for the element ( screen anchor and local anchor )
    /// @param sizing The sizing for the element (offset in pixels and scale in percentage relative to parent size)
    /// @param maxSizing The maximum size an element can take
    /// @param minSizing The minimum size an element can take
    UIPosition(
        Vector2 offset,
        Anchor anchoring,
        Size sizing,
        Scaling maxSizing = {-1},
        Scaling minSizing = {-1})
        : position(offset),
          anchor(anchoring),
          size(sizing),
          maxSize(maxSizing),
          minSize(minSizing) {}
};

class UIElement
{
public:
    UIPosition location;

    UIElement(UIPosition startLocation, bool isVisible = true)
        : location(startLocation), visible(isVisible), isLoaded(true) {}
    virtual ~UIElement() {}

    virtual void Draw() = 0;
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
        if (IsCursorOnScreen() == false || isVisible() == false || isLoaded == false)
        {
            return false;
        }

        Vector2 mousePos = GetMousePosition();
        Rectangle bounds = getBounds();
        return (mousePos.x >= bounds.x && mousePos.x <= bounds.x + bounds.width &&
                mousePos.y >= bounds.y && mousePos.y <= bounds.y + bounds.height);
    }

    bool hasClicked()
    {
        if (isLoaded == false)
            return false;
        return IsMouseOver() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
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
    bool isLoaded = false;
};

struct ButtonColoring
{
    Color hoverBackground;
    Color background;
    Color text;

    ButtonColoring(Color backgroundColor = LIGHTGRAY, Color hoverBackgroundColor = DARKGRAY, Color textColor = BLACK)
        : background(backgroundColor), hoverBackground(hoverBackgroundColor), text(textColor) {}
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

    void Update() override
    {
    }

    void Unload() override
    {
    }

private:
    const char *text;
    ButtonColoring btnColoring;
};

class UIContainer : public UIElement
{
public:
    UIContainer(UIPosition startLocation, const map<std::string, UIElement *> elements)
        : UIElement(startLocation), children(elements)
    {
        for (auto elementPair : elements)
        {
            if (elementPair.second)
            {
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

        if (children.count(identificator) > 0)
        {
            UIElement *elementFound = children.at(identificator);

            if (T *element = dynamic_cast<T *>(elementFound))
            {
                return element;
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

        for (auto elementPair : children)
        {
            UIElement *element = elementPair.second;
            if (element && element->isVisible() == true)
            {
                element->Draw();
            }
        }
    }

    void Update() override
    {
        for (auto elementPair : children)
        {
            UIElement *element = elementPair.second;
            if (element && element->isVisible() == true)
            {
                element->Update();
            }
        }
    }

    void Unload() override
    {
        for (auto elementPair : children)
        {
            UIElement *element = elementPair.second;
            element->Unload();
            delete element;
        }
        children.clear();
    }

private:
    map<std::string, UIElement *> children;
};

#endif