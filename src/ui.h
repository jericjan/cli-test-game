#include "entities.h"
#include "extras.h"
#include "items.h"
#include <random>
#include <list>
#include <functional>

class UserInterface
{
public:
    bool quitGame = false;
    virtual UserInterface *render() = 0;
    virtual ~UserInterface();
};

class UIWithPlayer : public UserInterface
{
public:
    Player player;
    UIWithPlayer();
    UIWithPlayer(Player player);
};

// forward declaration of StartMenu because c++ sucks
class StartMenu : public UserInterface
{
public:
    UserInterface *render() override;
};

class GameOver : public UserInterface
{
public:
    UserInterface *render() override;
};

class MainMenu : public UIWithPlayer
{
public:
    MainMenu(Player player);
    UserInterface *render() override;
};

class GambleMenu : public UIWithPlayer
{
public:
    GambleMenu(Player player);
    UserInterface *render() override;
};

class Battle : public UIWithPlayer
{
public:
    Entity enemy;
    bool didIntro;
    int rewardMoney;
    function<UserInterface *(Player)> onWin;
    function<UserInterface *(Player)> onLose;
    Battle(Player player, Entity enemy, int reward,
           function<UserInterface *(Player)> winCallback,
           function<UserInterface *(Player)> loseCallback);
    UserInterface *render() override;
};

class JovialAftermath : public UIWithPlayer
{
public:
    string battleStatus;
    JovialAftermath(Player player, string bs);
    UserInterface *render() override;
};

class JovialCutscene : public UIWithPlayer
{
public:
    JovialCutscene(Player player);
    UserInterface *render() override;
};

class FirstChoice : public UIWithPlayer
{
public:
    list<string> stayResps = {
        "Hello? I said get out.",
        "Come on man. I have stuff to do. Please leave.",
        "I'm gonna run out of dialogue options here. The devs didn't give me that many lines.",
        "Alright, I'm giving you the silent treatment.",
        "..."};
    FirstChoice(Player player);

    UserInterface *render() override;
};

class IntroDialogue : public UIWithPlayer
{
public:
    UserInterface *render() override;
};
