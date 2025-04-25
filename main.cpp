#include <iostream>
using namespace std;

bool quitGame = false;

class UserInterface {
    public:
        virtual UserInterface* render() = 0;
};

class Stats {
    public:
        int atk;
        int def;
        Stats(): atk(0), def(0) {}
        Stats(int x, int y): atk(x), def(y) {}
};

class Entity {
    protected:
        int health;        
        Stats stats;
    public:
        string name;
        Entity(int _health, string _name, int atk, int def) {
            health = _health;
            name = _name;
            stats = Stats(atk, def);
        }

};

class Inventory {

};

class Player: public Entity {
    public:
        Inventory inventory;
        int money;
        Player(): Entity(0, "", 0, 0), money(0) {}
        Player(int _health, string _name, int _atk, int _def, int _money): 
        Entity(_health, _name, _atk, _def), money(_money) {
            inventory = Inventory();
        }        
};

class UIWithPlayer: public UserInterface {
    public:
        Player player;
};

class IntroDialogue: public UIWithPlayer {
    public:
        UserInterface* render() {
            cout << "Woah! Didn't see you there. Who might you be?" << endl;
            string userInput;
            cin >> userInput;
            player = Player(100, userInput, 200, 50, 0);
            cout << "Hello there " << player.name << "!\n";
            return this;

        }
};

class StartMenu: public UserInterface {
public:    
    UserInterface* render() {
        cout << "Welcome to The Game!" << endl << "[1] Start" << endl << "[2] Quit" << endl;
        int userInput;
        cin >> userInput;
        if (userInput == 1) {
            cout << "AAAAAAAAAAAAAAA";
            return new IntroDialogue();
        } else {
            cout << "See you next time!" << endl;
            quitGame = true;
            return this;
        }
        
    }
};


class UIHandler {
public:
    UserInterface* currUI;

    UIHandler(UserInterface* x): currUI(x) {}

    void start(){
        while (!quitGame){
            currUI = currUI->render();
        }
  
    }
};

int main()
{
    StartMenu* startMenu = new StartMenu();
    UIHandler uiHandler = UIHandler(startMenu);
    uiHandler.start();

    return 0;
}