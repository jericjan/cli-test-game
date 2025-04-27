#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <thread>
#include <random>


using namespace std;

bool quitGame = false;

void sleepMS(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void printAnimate(string msg, int delay = 10) {
    cout << msg ;
    // for (auto &ch : msg) { 
    //     cout << ch;
    //     sleepMS(delay);
    // } 
}

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
        int maxHealth;
        int currHealth;
        Stats stats;    
        string name;
        Entity(int _health, string _name, int atk, int def) {
            currHealth = _health;
            maxHealth = _health;
            name = _name;
            stats = Stats(atk, def);
        }

        void attack(Entity& target, int extraDmg = 0) {
            int calcDmg = max(1, (stats.atk + extraDmg) - target.stats.def);
            target.currHealth = max(0, target.currHealth - calcDmg);
            cout << target.name << " took " << calcDmg << " DMG!" << endl;
        }

};

class IItem {
    public:
        string name;
        string type;
        string desc;
        int count; // A count of -1 means the item can be used infinitely        
        IItem(string n, string t, string d, int c): name(n), type(t), desc(d), count(c) {}
        virtual ~IItem() = default;  // required for polymorphic base class for dynamic_cast

};

class Player;

class Inventory {
    public:
        vector<IItem*> items;

        void addItem(IItem* item);

        void listItems();

        // returns true if the item was used successfully, false if it was not
        bool useItem(int idx, Player& player, Entity* enemy);

        void dropItem(int idx);
};

class Player: public Entity {
    public:
        Inventory inventory;
        int money;
        Player();
        Player(int _health, string _name, int _atk, int _def, int _money);        

        void addMoney(int amount) {
            cout << "You received " << amount << " money!" << endl;
            money += amount;
        }
};

class IEnemyItem: public IItem {
    public:
        IEnemyItem(string n, string t, string d, int c): IItem(n, t, d, c) {}
        virtual bool use(Player& player, Entity& enemy) = 0;
};

class IPlayerItem: public IItem {
    public:
        IPlayerItem(string n, string t, string d, int c): IItem(n, t, d, c) {}
        virtual bool use(Player& player) = 0;
};


        void Inventory::addItem(IItem* item) {
            string msg = item->name + " added to inventory!";
            string border = "\n" + string(msg.length(), '=') + "\n";
            printAnimate(border, 1);
            printAnimate(msg);
            printAnimate(border, 1);
            items.push_back(item);
        }

        string padTo(string str, const size_t num, const char paddingChar = ' ')
        {
            if(num > str.size()) {
                div_t divided = div(num - str.size(), 2);
                size_t leftSpace = divided.quot;
                size_t rightSpace = divided.quot + divided.rem;
                str.insert(str.begin(), leftSpace, paddingChar);
                str.insert(str.end(), rightSpace, paddingChar);
            }
            return str;
        }

        void Inventory::listItems() {
            int idx = 1;

            cout << padTo("[Name]", 20) << "|" << padTo("[Type]", 20) << "|" << padTo("[Description]", 50) << "|" << padTo("[Count]", 20) << endl;
            for (IItem* item: items) {
                string count = (item->count == -1) ? "Infinite" : to_string(item->count) + "x";
                string numberedName = string("[") + to_string(idx) + "] " + item->name;
                cout << padTo(numberedName, 20) << "|" << padTo(item->type, 20) << "|" << padTo(item->desc, 50) << "|" << padTo(count, 20) << endl;
                idx++;
            }
            cout << "Type the # of the item to use, 0 to exit > ";
        }

        // idx is 1-indexed
        bool Inventory::useItem(int idx, Player& player, Entity* enemy) {
            IItem* item = items.at(idx - 1);
            bool useResult;
            if (IPlayerItem* playerItem = dynamic_cast<IPlayerItem*>(item)) {
                useResult = playerItem->use(player);
            } else if (IEnemyItem* enemyItem = dynamic_cast<IEnemyItem*>(item)) {
                if (enemy == nullptr) {
                    cout << "You can't use that item here!" << endl;
                    return false;
                }
                useResult = enemyItem->use(player, *enemy);
            }

            if (!useResult) {
                return false;
            }

            if (item->count != -1) {
                item->count--;
                if (item->count == 0) {
                    items.erase(items.begin() + idx - 1);
                }
            }     
            return true;
        }   

        void Inventory::dropItem(int idx) {

        }



Player::Player(): Entity(0, "", 0, 0), money(0) {}
Player::Player(int _health, string _name, int _atk, int _def, int _money): 
Entity(_health, _name, _atk, _def), money(_money) {
    inventory = Inventory();
}




class IPotion: public IPlayerItem {
    public:
        IPotion(string n, string d, int c): IPlayerItem(n, "Potion", d, c) {}
};

class HealthPotion: public IPotion {    
    public:
        HealthPotion(int c): 
        IPotion("Health Potion", "Heals the user 5HP", c) {}

        bool use(Player& player) override {
            if (player.currHealth == player.maxHealth) {
                cout << "You're already at full health!" << endl;
                return false;
            }
            player.currHealth = min(player.maxHealth, player.currHealth + 5);            
            return true;
        }
};

class IWeapon: public IEnemyItem {
    public:
        int dmg;
        IWeapon(string name, string desc, int count, int dmg): IEnemyItem(name, "Weapon", desc, count), dmg(dmg) {}
        bool use(Player& player, Entity& enemy) override {
            player.attack(enemy, dmg);
            return true;
        }
};

class CoolStick: public IWeapon {
    public:
    CoolStick(): IWeapon("Cool Stick", "A cool stick some stranger gave you", -1, 60) {}
};

class Yamato: public IWeapon {
    public:
        Yamato(): IWeapon("Yamato", "A legendary sword that does 500 damage!", -1, 500) {}
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

string yellowText(string msg) {
    return  "\033[33m" + msg + "\033[0m";
}

class GameOver: public UserInterface {
    public:
        UserInterface* render() override {
            printAnimate(string("Game Over! You lost the game. No respawns here. You're gone. Sorry.\n") + 
            "Press Enter to try again.\n");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            return new StartMenu();
        }
};

class MainMenu: public UIWithPlayer {
    public:
        MainMenu(Player player);
        UserInterface* render() override;
};

class GambleMenu: public UIWithPlayer {
    public:
        GambleMenu(Player player): UIWithPlayer(player) {}
        UserInterface* render() override {
            cout << "It costs 100 to gamble. You either win the Legendary Sword Yamato or nothing.\nContinue? (y/n) > ";
            string userInput;
            cin >> userInput;
            if (userInput == "y") {
                if (player.money < 100) {
                    cout << "You don't have enough money!" << endl;
                    return this;
                } else {
                    player.addMoney(-100);
                    random_device rd;
                    mt19937 mt(rd());
                    uniform_real_distribution<double> dist(0.0, 1.0);        
                    bool win = dist(mt) < 0.25;                    
                    if (!win) {
                        cout << "You got nothing! Better luck next time." << endl;
                        return this;
                    } else {
                        cout << "You got the Legendary Sword Yamato! It's a legendary sword that does 500 damage!\n";
                        Yamato* yamato = new Yamato();
                        player.inventory.addItem(yamato);
                        return new MainMenu(player);
                    }
                }
            } else if (userInput == "n") {
                return new MainMenu(player);
            } else {
                cout << "That's not one of the options!" << endl;
            }
            return this;
        }
};

class Battle: public UIWithPlayer {
    public:
    Entity enemy;
    bool didIntro;
    int rewardMoney;
    Battle(Player player, Entity enemy, int reward);
    UserInterface* render() override;
};

MainMenu::MainMenu(Player player): UIWithPlayer(player) {}

UserInterface* MainMenu::render() {
    cout << "[Main Menu]" << endl;
    cout << "HP: " << player.currHealth << "/" << player.maxHealth << " ATK: " << player.stats.atk << " DEF: " << player.stats.def << endl;
    cout << "Money: " << player.money << endl;            
    cout << "[1] Inventory" << endl;
    cout << "[2] Grind" << endl;
    cout << "[3] Gamble" << endl;
    cout << "[4] Hospital" << endl;
    cout << "[5] Quit" << endl;
    string userInput;
    cin >> userInput;
    if (userInput == "1") {
        player.inventory.listItems();
        int bagInput;
        while (true) {
            cin >> bagInput;
            if (bagInput == 0) {
                break;
            } else {
                if (player.inventory.useItem(bagInput, player, nullptr)) {
                    break;
                }
            }
        }                
        return this;
    }  else if (userInput == "2") {
        Entity enemy = Entity(500, "Random Guy", 50, 50);
        return new Battle(player, enemy, 50);
    }  else if (userInput == "3") {
        return new GambleMenu(player);
    }  else if (userInput == "4") {
        printAnimate("You were healed to full health!\n");
        player.currHealth = player.maxHealth;
        return this;
    }  else if (userInput == "5") {
        return new StartMenu();
    } else {
        cout << "That's not one of the options!" << endl;
        return this;
    }
}



class JovialAftermath: public UIWithPlayer {
    public:
        string battleStatus;
        JovialAftermath(Player player, string bs): UIWithPlayer(player), battleStatus(bs) {}
        UserInterface* render() override {
            if (battleStatus == "win") {
                printAnimate(yellowText("Jovial: I- I lost? I can't believe it...\n") + 
                player.name + ": Yeah, tough luck man. Now get outta here.\n" + 
                yellowText("Jovial: Okay, geez.\n") + 
                "Villager 2: You saved us! Take these health potions and cash as a form of gratitude from us.\n");
                HealthPotion* hp = new HealthPotion(20);
                player.inventory.addItem(hp);
                player.addMoney(100);
                return new MainMenu(player);

            } else if (battleStatus == "lose") {
                printAnimate("Jovial: Finally. This will make up for the race I lost!\n");
                return new GameOver();
            } else {
                cout << "Error: Invalid battle status." << endl;
            }
            return this;
        }
};


Battle::Battle(Player player, Entity enemy, int reward): 
UIWithPlayer(player), enemy(enemy), didIntro(false), rewardMoney(reward) {}

UserInterface* Battle::render() {
    if (!didIntro) {
        cout << "\n\n===BATTLE START!!===\n\n";
        didIntro = true;
    }
    cout << enemy.name << ": " << enemy.currHealth << "/" << enemy.maxHealth << endl <<
    player.name << ": " << player.currHealth << "/" << player.maxHealth << "\n\n" <<
    "[1] Punch\n[2] Bag\n[3] Run\n";
    string userInput;
    cin >> userInput;
    cout << endl;
    if (userInput == "1") {
        player.attack(enemy);
        enemy.attack(player);
    } else if (userInput == "2") {
        player.inventory.listItems();
        int bagInput;
        cin >> bagInput;
        if (bagInput == 0) {}
        else {
            player.inventory.useItem(bagInput, player, &enemy);
            enemy.attack(player);
        }
    } else if (userInput == "3") {
        cout << "You can't run from this battle!" << endl;
    } else if (userInput == "4") { // for testing purposes
        enemy.currHealth = 0;
        cout << "You punched the enemy so hard, they died instantly!" << endl;
    } else {
        cout << "That's not one of the options!" << endl;            
        return this;
    }
    
    if (player.currHealth == 0) {
        cout << player.name << " lost the battle!" << endl;
        return new JovialAftermath(player, "lose");
    } else if (enemy.currHealth == 0) {
        cout << enemy.name << " lost the battle!" << endl;
        return new JovialAftermath(player, "win");
        if (rewardMoney > 0) {
            player.addMoney(rewardMoney);
        }
    }
    return this;
}



class JovialCutscene: public UIWithPlayer {
    public:
        JovialCutscene(Player player): UIWithPlayer(player) {}
        UserInterface* render() override {
            printAnimate(string("Villager 1: Hey, did you know? Jovial lost the final race today...\n") + 
            "Villager 2: What?? That can't be! She's the best horse there is. I put all my money on her and she LOST???\n" +
            "Villager 1: Okay, calm your horses (hehe). It's gonna be fine.\n" + 
            "Villager 2: NO IT WON'T!! When I see that horse again, I'll... I'll...\n" + 
            yellowText("Jovial: You'll what?\n") + 
            "*Jovial approaches Villager 2 very menacingly*\n\n" + 
            player.name + ": STOP!! If anybody's fighting, it'll be you and me, Jovial.\n" +
            "Jovial: Alright then, kid, let's fight!\n");

            Entity jovial = Entity(1000, "Jovial Merryment", 100, 50);
            return new Battle(player, jovial, 0);
        }
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
        FirstChoice(Player player): UIWithPlayer(player) {}

        UserInterface* render() override {
            cout << "[1] Stay" << endl << "[2] Leave" << endl;
            string userInput;
            cin >> userInput;
            cout << endl;
            if (userInput == "1") {
                printAnimate(stayResps.front() + "\n");
                if (stayResps.size() > 1) {                    
                    stayResps.pop_front();
                }
                return this;
            } else  if (userInput == "2"){
                return new JovialCutscene(player);
            } else {
                cout << "That's not one of the options!" << endl;
                return this;
            }
        }
};

class IntroDialogue: public UIWithPlayer {
    public:
        UserInterface* render() override {            
            printAnimate("Woah! Didn't see you there. Who might you be?\n> ");
            string userInput;
            getline(cin >> ws, userInput);
            player = Player(1000, userInput, 50, 50, 0);
            printAnimate("Hello there " + player.name + "!\n" + 
            "You are about to embark on a heroic journey of... something. I don't know. Go out there man. Go nuts.\n"
            + "Me? You don't need to know who I am. Ooh, I'll give you this cool stick I found!\n");

            CoolStick* stick = new CoolStick();
            player.inventory.addItem(stick);

            printAnimate(string("This will give you some extra damage in your battles. I should probably explain how that works.\n") + 
            "In a battle, you can either: punch, use an item from your inventory, or run. I'm sure you're familiar with this formula.\n" +
            "Alright, that's all from me. Get outta here.\n");
            return new FirstChoice(player);

        }
};


UserInterface* StartMenu::render() {
        cout << "Welcome to The Game!" << endl << "[1] Start" << endl << "[2] Quit" << endl;
        string userInput;
        
        cin >> userInput;
        if (userInput == "1") {
            return new IntroDialogue();
        } else if (userInput == "2") {
            cout << "See you next time!" << endl;
            quitGame = true;
            return this;
        } else  {
            cout << "That's not one of the options!" << endl;
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