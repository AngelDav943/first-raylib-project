#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include <bits/stdc++.h>
#include <math.h>
#include <vector>
#include <memory>
#include <raylib.h>

template <typename T>
T clamp(const T &value, const T &min, const T &max)
{
    return (value < min) ? min : (value > max) ? max
                                               : value;
}

struct Scaling
{
    float width;
    float height;
};

struct Size
{
    Scaling offset;
    Scaling scale;

    Size(Scaling off, Scaling scl)
        : offset(off), scale(scl) {}
};

struct Anchor
{
    Vector2 local;
    Vector2 screen;
    Anchor(Vector2 lc, Vector2 sc)
        : local(lc), screen(sc) {}
};

struct UIPosition
{
    Vector2 position;
    Anchor anchor;
    Size size;

    UIPosition(Vector2 pos, Anchor anch, Size sz)
        : position(pos), anchor(anch), size(sz) {}
};

class UIElement
{
public:
    UIPosition location;

    UIElement(UIPosition startLocation)
        : location(startLocation), visible(true) {}

    virtual void Draw() = 0;
    virtual void Update() = 0;
    virtual void Unload() = 0;

    Rectangle getBounds()
    {
        int screenWidth = GetRenderWidth();
        int screenHeight = GetRenderHeight();

        Anchor uiAnchor = location.anchor;
        Size uiSize = location.size;

        float screenXscale = screenWidth * clamp<float>(uiSize.scale.width, 0, 1);
        float screenYscale = screenHeight * clamp<float>(uiSize.scale.height, 0, 1);

        Scaling resultSize = {
            screenXscale + uiSize.offset.width,
            screenYscale + uiSize.offset.height
        };

        float screenXAnchor = screenWidth * clamp<float>(uiAnchor.screen.x, 0, 1);
        float screenYAnchor = screenHeight * clamp<float>(uiAnchor.screen.y, 0, 1);

        float localXAnchor = resultSize.width * clamp<float>(uiAnchor.local.x, 0, 1);
        float localYAnchor = resultSize.height * clamp<float>(uiAnchor.local.y, 0, 1);

        return (Rectangle){
            screenXAnchor - localXAnchor + location.position.x,
            screenYAnchor - localYAnchor + location.position.y,
            resultSize.width,
            resultSize.height};
    }

    bool IsMouseOver()
    {
        if (visible == false)
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
        return IsMouseOver() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    bool isVisible()
    {
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
};

class UIButton : public UIElement
{
public:
    UIButton(UIPosition startLocation, const char *text)
        : UIElement(startLocation), text(text), clickCallback(nullptr)
    {
    }

    void Draw() override
    {
        Scaling buttonSizeOffset = location.size.offset;
        Color backgroundColor = IsMouseOver() ? DARKGRAY : LIGHTGRAY;
        int textLength = (int)(sizeof(text) / sizeof(text[0]));
        int buttonWidth = buttonSizeOffset.width;

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
        DrawText(text, buttonBounds.x + 10, buttonBounds.y + 10, buttonBounds.height - 20, BLACK);
    }

    void onClick(std::function<void()> callbackFunction)
    {
        clickCallback = callbackFunction;
    }

    void Update() override
    {
        // If mouse inside button area and clicked call the callback function
        if (hasClicked() && clickCallback)
        {
            clickCallback();
        }
    }

    void Unload() override
    {
    }

private:
    const char *text;
    std::function<void()> clickCallback; // Callback function
};

class UIContainer : public UIElement
{
public:
    // std::vector<UIElement *> children;

    UIContainer(UIPosition startLocation, const map<std::string, UIElement *> elements)
        : UIElement(startLocation), children(elements)
    {
    }

    UIElement *getElementById(std::string identificator)
    {
        if (children.empty())
        {
            return nullptr;
        }

        if (children.count(identificator) > 0)
        {
            return children.at(identificator);
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