#include "entities.h"
#include "extras.h"
#include "items.h"
#include <random>
#include <list>
#include <functional>
#include <string>
#include "ui.h"
#include "shop.h"

using namespace std;

UserInterface::~UserInterface() = default;

UIWithPlayer::UIWithPlayer() {}
UIWithPlayer::UIWithPlayer(Player player) : player(player) {}

UserInterface *GameOver::render()
{
    printAnimate(string("Game Over! You lost the game. No respawns here. You're gone. Sorry.\n") +
                 "Press Enter to try again.\n");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return new StartMenu();
}

GambleMenu::GambleMenu(Player player) : UIWithPlayer(player) {}
UserInterface *GambleMenu::render()
{
    cout << "It costs 100 to gamble. You either win the Legendary Sword Yamato or nothing.\nContinue? (y/n) > ";
    string userInput;
    cin >> userInput;
    if (userInput == "y")
    {
        if (!player.canAfford(100))
        {
            cout << colorizeText("You don't have enough money!", RED) << endl;
            return this;
        }
        else
        {
            player.addMoney(-100);
            random_device rd;
            mt19937 mt(rd());
            uniform_real_distribution<double> dist(0.0, 1.0);
            bool win = dist(mt) < 0.25;
            if (!win)
            {
                cout << colorizeText("You got nothing! Better luck next time.", RED) << endl;
                return this;
            }
            else
            {
                Yamato *yamato = new Yamato();
                cout << colorizeText("You got the Legendary Sword Yamato! It's a legendary sword that does " + to_string(yamato->dmg) + " damage!\n", GREEN) << endl;

                IItem* possibleYamato = player.inventory.addItem(yamato);                
                if (Yamato* newYamato = dynamic_cast<Yamato *>(possibleYamato))
                {
                    cout << "Interesting... It seems you already have this item. Tell you what, I'll double its damage for you.\n";
                    newYamato->dmg *= 2;
                }
                return new MainMenu(player);
            }
        }
    }
    else if (userInput == "n")
    {
        return new MainMenu(player);
    }
    else
    {
        cout << "That's not one of the options!" << endl;
    }
    return this;
}

MainMenu::MainMenu(Player player) : UIWithPlayer(player) {}

UserInterface *MainMenu::render()
{
    cout << "[Main Menu]" << endl;
    player.printInfo(false);
    cout << "Money: " << player.getMoney() << endl;
    cout << "[1] Inventory" << endl;
    cout << "[2] Grind" << endl;
    cout << "[3] Shop" << endl;
    cout << "[4] Gamble" << endl;
    cout << "[5] Hospital" << endl;
    cout << "[6] Quit" << endl;
    int userInput = inputNum();
    if (userInput == 1)
    {
        player.inventory.listItems();
        int bagInput;
        while (true)
        {
            bagInput = inputNum();
            if (bagInput == 0)
            {
                break;
            }
            else
            {
                if (player.inventory.useItem(bagInput, player, nullptr))
                {
                    break;
                }
            }
        }
        return this;
    }
    else if (userInput == 2)
    {
        Entity enemy = Entity(500, "Random Guy", 70, 50);
        auto win = [=](Player player) -> UserInterface *
        {
            return new MainMenu(player);
        };
        auto lose = [=](Player player) -> UserInterface *
        {
            printAnimate("You lost? In a random battle? How embarrassing.\n");
            return new GameOver();
        };        
        return new Battle(player, enemy, 50, win, lose);
    }
    else if (userInput == 3)
    {
        return new ShopMenu(player);
    }    
    else if (userInput == 4)
    {
        return new GambleMenu(player);
    }
    else if (userInput == 5)
    {
        printAnimate("You were healed to full health!\n");
        player.fullyHeal();        
        return this;
    }
    else if (userInput == 6)
    {
        return new StartMenu();
    }
    else if (userInput == 7)
    {
        player.setMoney(999999);
        return this;
    }    
    else
    {
        cout << "That's not one of the options!" << endl;
        return this;
    }
}

ShopMenu::ShopMenu(Player player) : UIWithPlayer(player) {}

UserInterface *ShopMenu::render()
{
    cout << "Welcome to the shop! What would you like to buy?" << endl;
    Shop shop;
    shop.addItem(new HealthPotion(1), 50);
    shop.addItem(new StrengthPotion(1), 70);
    shop.listItems(player);
    int userInput = inputNum();
    if (userInput == 0)
    {
        return new MainMenu(player);
    }
    else
    {
        shop.buyItem(player, userInput);
    }
    return this;
}

JovialAftermath::JovialAftermath(Player player, string bs) : UIWithPlayer(player), battleStatus(bs) {}
UserInterface *JovialAftermath::render()
{
    if (battleStatus == "win")
    {
        printAnimate(colorizeText("Jovial: I- I lost? I can't believe it...\n", YELLOW) +
                     player.getName() + ": Yeah, tough luck man. Now get outta here.\n" +
                     colorizeText("Jovial: Okay, geez.\n", YELLOW) +
                     "Villager 2: You saved us! Take these health potions and cash as a form of gratitude from us.\n");
        HealthPotion *hp = new HealthPotion(20);
        player.inventory.addItem(hp);
        player.addMoney(100);
        return new MainMenu(player);
    }
    else if (battleStatus == "lose")
    {
        printAnimate("Jovial: Finally. This will make up for the race I lost!\n");
        return new GameOver();
    }
    else
    {
        cout << "Error: Invalid battle status." << endl;
    }
    return this;
}

Battle::Battle(Player player, Entity enemy, int reward,
               function<UserInterface *(Player)> winCallback,
               function<UserInterface *(Player)> loseCallback) : UIWithPlayer(player), enemy(enemy), didIntro(false), rewardMoney(reward),
                                                                 onWin(winCallback), onLose(loseCallback) {}

UserInterface *Battle::render()
{
    if (!didIntro)
    {
        cout << "\n\n===BATTLE START!!===\n\n";
        didIntro = true;
    }
    enemy.printInfo();
    player.printInfo();
    cout << "\n[1] Punch\n[2] Bag\n[3] Run\n";
    int userInput = inputNum();
    cout << endl;
    if (userInput == 1)
    {
        player.attack(enemy);
        if (enemy.isAlive())
        {
            enemy.attack(player);
        }
    }
    else if (userInput == 2)
    {
        player.inventory.listItems();
        while (true)
        {
            int bagInput = inputNum();
            if (bagInput == 0)
            {
                break;
            }
            else
            {
                if (!player.inventory.useItem(bagInput, player, &enemy))
                {
                    continue;
                }
                else
                {
                    if (enemy.isAlive())
                    {
                        enemy.attack(player);
                    }
                    break;
                };
                
            }
        }
    }
    else if (userInput == 3)
    {
        cout << "You can't run from this battle!" << endl;
    }
    else if (userInput == 4)
    { // for testing purposes
        enemy.kill();
        cout << "You punched the enemy so hard, they died instantly!" << endl;
    }
    else
    {
        cout << "That's not one of the options!" << endl;
        return this;
    }

    if (!player.isAlive())
    {
        cout << player.getName() << " lost the battle!" << endl;
        return onLose(player);
    }
    else if (!enemy.isAlive())
    {
        cout << enemy.getName() << " lost the battle!" << endl;
        if (rewardMoney > 0)
        {
            player.addMoney(rewardMoney);
        }
        return onWin(player);
    }
    return this;
}

JovialCutscene::JovialCutscene(Player player) : UIWithPlayer(player) {}
UserInterface *JovialCutscene::render()
{
    printAnimate(string("Villager 1: Hey, did you know? Jovial lost the final race today...\n") +
                 "Villager 2: What?? That can't be! She's the best horse there is. I put all my money on her and she LOST???\n" +
                 "Villager 1: Okay, calm your horses (hehe). It's gonna be fine.\n" +
                 "Villager 2: NO IT WON'T!! When I see that horse again, I'll... I'll...\n" +
                 colorizeText("Jovial: You'll what?\n", YELLOW) +
                 "*Jovial approaches Villager 2 very menacingly*\n\n" +
                 player.getName() + ": STOP!! If anybody's fighting, it'll be you and me, Jovial.\n" +
                 "Jovial: Alright then, kid, let's fight!\n");

    Entity jovial = Entity(1000, "Jovial Merryment", 100, 50);

    auto onWin = [=](Player player) -> UserInterface *
    {
        return new JovialAftermath(player, "win");
    };

    auto onLose = [=](Player player) -> UserInterface *
    {
        return new JovialAftermath(player, "lose");
    };

    return new Battle(player, jovial, 0, onWin, onLose);
}

FirstChoice::FirstChoice(Player player) : UIWithPlayer(player) {}

UserInterface *FirstChoice::render()
{
    cout << "[1] Stay" << endl
         << "[2] Leave" << endl;
    int userInput = inputNum();
    cout << endl;
    if (userInput == 1)
    {
        printAnimate(stayResps.front() + "\n");
        if (stayResps.size() > 1)
        {
            stayResps.pop_front();
        }
        return this;
    }
    else if (userInput == 2)
    {
        return new JovialCutscene(player);
    }
    else
    {
        cout << "That's not one of the options!" << endl;
        return this;
    }
}

UserInterface *IntroDialogue::render()
{
    printAnimate("Woah! Didn't see you there. Who might you be?\n> ");
    string userInput;
    getline(cin >> ws, userInput);
    player = Player(1000, userInput, 50, 50, 0);
    printAnimate("Hello there " + player.getName() + "!\n" +
                 "You are about to embark on a heroic journey of... something. I don't know. Go out there man. Go nuts.\n" + "Me? You don't need to know who I am. Ooh, I'll give you this cool stick I found!\n");

    CoolStick *stick = new CoolStick();
    player.inventory.addItem(stick);

    printAnimate(string("This will give you some extra damage in your battles. I should probably explain how that works.\n") +
                 "In a battle, you can either: punch, use an item from your inventory, or run. I'm sure you're familiar with this formula.\n" +
                 "Alright, that's all from me. Get outta here.\n");
    return new FirstChoice(player);
}

UserInterface *StartMenu::render()
{
    cout << "Welcome to The Game!" << endl
         << "[1] Start" << endl
         << "[2] Quit" << endl;
    int userInput = inputNum();
    if (userInput == 1)
    {
        return new IntroDialogue();
    }
    else if (userInput == 2)
    {
        cout << "See you next time!" << endl;
        // quitGame = true;
        this->quitGame = true;
        return this;
    }
    else
    {
        cout << "That's not one of the options!" << endl;
        return this;
    }
}