    #include <iostream>
#include <vector>
#include "extras.h"
#include "ui.h"
#include "inventory.h"
#include "items.h"

using namespace std;

/// Handles UI in the game.
/// Basically runs the UI in a loop, and the UIs can return other UIs to switch to.
/// Ends when the current UI is a GameOver screen or when UserInterface#quitGame is set to true.
class UIHandler
{
private:    
    UserInterface *currUI;
public:    
    UIHandler(UserInterface *x) : currUI(x) {}

    void start()
    {
        while (true)
        {
            UserInterface *oldUI = currUI;            // Store the current pointer
            UserInterface *nextUI = currUI->render(); // Get the result from render
            if (nextUI != oldUI)
            {
                delete oldUI;
            }
            currUI = nextUI;

            if (currUI->quitGame)
            {
                delete currUI;
                break;
            }
        }
    }
};

int main()
{
    StartMenu *startMenu = new StartMenu();
    UIHandler uiHandler = UIHandler(startMenu);
    uiHandler.start();

    return 0;
}