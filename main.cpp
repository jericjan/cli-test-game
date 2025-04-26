#include <iostream>
#include <list>

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
    public:
        int health;
        Stats stats;    
        string name;
        Entity(int _health, string _name, int atk, int def) {
            health = _health;
            name = _name;
            stats = Stats(atk, def);
        }

};

class IItem {
    protected:        
        string type;
        string desc;
        int count; // A count of -1 means the item can be used infinitely
    public:
        string name;
        IItem(string n, string t, string d, int c): name(n), type(t), desc(d), count(c) {}
};

class Inventory {
    public:
        list<IItem*> items;

        void addItem(IItem* item) {
            string msg = item->name + " added to inventory!";
            string border = string(msg.length(), '=');
            cout << endl << border << endl 
            << msg <<
            endl << border << endl;
            items.push_back(item);
        }

        void listItems() {

        }

        void useItem(int idx) {

        }

        void dropItem(int idx) {

        }
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

class IEnemyItem: public IItem {
    public:
        IEnemyItem(string n, string t, string d, int c): IItem(n, t, d, c) {}
        virtual void use(Player player, Entity enemy) = 0;
};





class IPlayerItem: public IItem {
    public:
        IPlayerItem(string n, string t, string d, int c): IItem(n, t, d, c) {}
        virtual void use(Player player) = 0;
};

class IPotion: public IPlayerItem {
    public:
        IPotion(string n, string d, int c): IPlayerItem(n, "Potion", d, c) {}
};

class HealthPotion: public IPotion {
    public:
        HealthPotion(int c, Player p): 
        IPotion("Health Potion", "Heals the user 5HP", c) {}

        void use(Player player) override {
            player.health += 5;
        }
};

class IWeapon: public IEnemyItem {
    public:
        int dmg;
        IWeapon(string name, string desc, int count, int dmg): IEnemyItem(name, "Weapon", desc, count), dmg(dmg) {}
        void use(Player player, Entity enemy) override {
            enemy.health = max(1, (player.stats.atk + dmg) - enemy.stats.def);
        }        
};

class CoolStick: public IWeapon {
    public:
    CoolStick(): IWeapon("Cool Stick", "A cool stick some stranger gave you", -1, 10) {}
};

class UIWithPlayer: public UserInterface {
    public:
        Player player;
        UIWithPlayer() {}
        UIWithPlayer(Player player): player(player) {}
};

class TestUI: public UIWithPlayer {
    public:
        UserInterface* render() override {
            return this;
        }
};

// forward declaration of StartMenu because c++ sucks
class StartMenu: public UserInterface {
    public:    
        UserInterface* render() override;
};

class FirstChoice: public UIWithPlayer {
    public:
        list<string> stayResps = {
            "Hello? I said get out.",
            "Come on man. I have stuff to do. Please leave.",
            "I'm gonna run out of dialogue options here. The devs didn't give me that many lines.",
            "Alright, I'm giving you the silent treatment.",
            "..."
        };
        FirstChoice(Player player): UIWithPlayer(player) {
        

        }
        UserInterface* render() override {
            cout << "[1] Stay" << endl << "[2] Leave" << endl;
            int userInput;
            cin >> userInput;
            cout << endl;
            if (userInput == 1) {
                cout << stayResps.front() << endl;
                if (stayResps.size() > 1) {                    
                    stayResps.pop_front();
                }
                return this;
            } else {
                return new StartMenu();
            }
        }
};

class IntroDialogue: public UIWithPlayer {
    public:
        UserInterface* render() override {
            cout << "Woah! Didn't see you there. Who might you be?\n> ";
            string userInput;
            getline(cin >> ws, userInput);
            player = Player(100, userInput, 200, 50, 0);
            cout << "Hello there " << player.name << "!\n" << 
            "You are about to embark on a heroic journey of... something. I don't know. Go out there man. Go nuts.\n"
            << "Me? You don't need to know who I am. Ooh, I'll give you this cool stick I found!" << endl;

            CoolStick* stick = new CoolStick();
            player.inventory.addItem(stick);

            cout << "This will give you some extra damage in your battles. I should probably explain how that works." << endl <<
            "In a battle, you can either: punch, use an item from your inventory, or run. I'm sure you're familiar with this formula." << endl <<
            "Alright, that's all from me. Get outta here." << endl;
            return new FirstChoice(player);

        }
};


UserInterface* StartMenu::render() {
        cout << "Welcome to The Game!" << endl << "[1] Start" << endl << "[2] Quit" << endl;
        int userInput;
        
        cin >> userInput;
        if (userInput == 1) {
            return new IntroDialogue();
        } else {
            cout << "See you next time!" << endl;
            quitGame = true;
            return this;
        }
        
}



class UIHandler {
public:
    UserInterface* currUI;

    UIHandler(UserInterface* x): currUI(x) {}

    void start(){
        while (!quitGame){
            UserInterface* oldUI = currUI; // Store the current pointer
            UserInterface* nextUI = currUI->render(); // Get the result from render
            if (nextUI != oldUI) {
                delete oldUI;
            }            
            currUI = nextUI;
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