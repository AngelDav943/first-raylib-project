#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include <bits/stdc++.h>
#include <math.h>
#include <vector>
#include <memory>
#include <raylib.h>

class UIElement
{
public:
    Vector2 position;
    Vector2 size;

    virtual ~UIElement() {}
    virtual void Draw() = 0;
    virtual void Update() = 0;

    bool IsMouseOver()
    {
        Vector2 mousePos = GetMousePosition();
        return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
                mousePos.y >= position.y && mousePos.y <= position.y + size.y);
    }
};

class UIManager
{
public:
    void AddElement(UIElement* element)
    {
        elements.push_back(element);
    }

    void Update()
    {
        for (auto element : elements)
        {
            element->Update();
        }
    }

    void Draw()
    {
        for (auto element : elements)
        {
            element->Draw();
        }
    }

    void Unload()
    {
        for (auto element : elements)
        {
            delete element;
        }
        elements.clear();
    }

private:
    std::vector<UIElement*> elements;
};

class UIButton : public UIElement
{
public:
    UIButton(Vector2 pos, Vector2 scale, const char *text)
    {
        position = pos;
        size = scale;
        this->text = text;
    }

    void Draw() override
    {
        Color backgroundColor = IsMouseOver() ? DARKGRAY : LIGHTGRAY;
        int textLength = (int)(sizeof(text) / sizeof(text[0]));
        int buttonWidth = size.x;

        if (size.x < (size.y - 20) * textLength) {
            buttonWidth = (size.y - 20) * textLength;
            size.x = buttonWidth;
        }

        DrawRectangle(
            position.x, position.y,
            buttonWidth, size.y,
            backgroundColor);
        DrawText(text, position.x + 10, position.y + 10, size.y - 20, BLACK);
    }

    void onClick(std::function<void()> callback)
    {
        OnClick = callback;
    }

    void Update() override
    {
        // If mouse inside button area and clicked call the callback function
        if (IsMouseOver() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && OnClick)
        {
            OnClick();
        }
    }

private:
    const char* text;
    std::function<void()> OnClick; // Callback function
};

#endif