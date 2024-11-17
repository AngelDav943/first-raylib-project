#ifndef PAUSEMENUCONTROLLER_H
#define PAUSEMENUCONTROLLER_H

#include "UIManager.h"
#include "ElementsUI.h"
#include "SceneManager.h"

class PauseMenuController
{
public:
    PauseMenuController() {}

    /// @brief Initializes the Pause menu
    /// @param ui The UI Manager used on the scene
    void init(UIManager &ui)
    {
        ui.AddElement(
              "pauseMenu",
              new UIContainer(
                  UIPosition{
                    .position = Vector2{0, 0}, // Offset position
                    .anchor = Anchor{
                          Vector2{0.5f, 0.5f}, // Local anchor
                          Vector2{0.5f, 0.5f}  // Screen anchor
                      },
                    .size = Size{
                          Scaling{500, 0}, // Offset
                          Scaling{0, 1}    // Scale
                      },
                    .maxSize = Scaling{1, 1}, // Max size
                    .minSize = Scaling{0, 0}  // Min size
                  },
                  // Children
                  {
                      {"resumeButton", // id
                       new UIButton(
                           UIPosition{
                               .position = Vector2{0, -5}, // Offset position
                               .anchor = Anchor{
                                   Vector2{0.0f, 1},    // Local anchor
                                   Vector2{0.0f, 0.5f}, // Screen anchor
                               },
                               .size = Size{
                                   Scaling{0, 50}, // Offset
                                   Scaling{1, 0}   // Scale
                               }},
                           "Resume")},
                      {"menuButton", // id
                       new UIButton(
                           UIPosition{
                               .position = Vector2{0, 5}, // Offset position
                               .anchor = Anchor{
                                   Vector2{0.5f, 0},    // Local anchor
                                   Vector2{0.5f, 0.5f}, // Screen anchor
                               },
                               .size = Size{
                                   Scaling{0, 50}, // Offset
                                   Scaling{1, 0},  // Scale
                               }},
                           "Back to menu")},
                      {"quitButton", // id
                       new UIButton(
                           UIPosition{
                               .position = Vector2{0, 0}, // Offset position
                               .anchor = Anchor{
                                   Vector2{0.5f, 1}, // Local anchor
                                   Vector2{0.5f, 1}, // Screen anchor
                               },
                               .size = Size{
                                   Scaling{0, 50}, // Offset
                                   Scaling{1, 0},  // Scale
                               }},
                           "Quit game")},
                  }))
            ->setVisible(false);
    }

    /// @brief Updates the menu, recommended to be used on `LateUpdate()`
    /// @param ui The UI Manager used on the scene
    /// @param sceneManager The scene manager used by the scene, usually is `globalSceneManager`
    void UpdateMenu(UIManager &ui, SceneManager &sceneManager)
    {
        UIContainer *pauseContainer = ui.GetElementById<UIContainer>("pauseMenu");
#ifdef PLATFORM_DESKTOP
        if (IsKeyPressed(KEY_ESCAPE)) {
#else
        if (IsKeyPressed(KEY_BACK)) {
#endif
            if (pauseContainer->isVisible() == false)
            {
                EnableCursor();
                pauseContainer->setVisible(true);
            }
            else
            {
                DisableCursor();
                pauseContainer->setVisible(false);
            }
        }

        // Pause menu logic
        UIButton *menuButton = pauseContainer->getElementById<UIButton>("menuButton");
        UIButton *resumeButton = pauseContainer->getElementById<UIButton>("resumeButton");
        UIButton *quitButton = pauseContainer->getElementById<UIButton>("quitButton");

        if (resumeButton->hasClicked())
        {
            DisableCursor();
            pauseContainer->setVisible(false);
            return;
        }

        if (menuButton->hasClicked())
        {
            sceneManager.LoadScene("Menu");
            return;
        }

        if (quitButton->hasClicked())
        {
            CloseWindow();
            return;
        }
    }

private:
};
#endif