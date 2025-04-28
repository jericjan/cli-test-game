#include "entities.h"
#include "extras.h"
#include "items.h"
#include <random>
#include <list>
#include <functional>

/// Base class for all user interfaces in the game
class UserInterface
{
public:
    bool quitGame = false;
    virtual UserInterface *render() = 0;
    virtual ~UserInterface();
};

/// Basically just the base class for all user interfaces that have a player
class UIWithPlayer : public UserInterface
{
public:
    Player player;
    UIWithPlayer();
    UIWithPlayer(Player player);
};

/// The first menu the user sees when they start the game
class StartMenu : public UserInterface
{
public:
    UserInterface *render() override;
};

/// The game over screen. The user is sent back to the start menu after this.
class GameOver : public UserInterface
{
public:
    UserInterface *render() override;
};

/// The main menu after all the cutscenes and stuff
class MainMenu : public UIWithPlayer
{
public:
    MainMenu(Player player);
    UserInterface *render() override;
};

/// The menu for gambling. The user can win a legendary sword or nothing.
class GambleMenu : public UIWithPlayer
{
public:
    GambleMenu(Player player);
    UserInterface *render() override;
};

/// The battle screen. The user can attack, use an item, or run away.
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

/// The cutscene after the player wins the Jovial fight.
class JovialAftermath : public UIWithPlayer
{
public:
    string battleStatus;
    JovialAftermath(Player player, string bs);
    UserInterface *render() override;
};

/// The cutscene before the player fights Jovial.
class JovialCutscene : public UIWithPlayer
{
public:
    JovialCutscene(Player player);
    UserInterface *render() override;
};

/// The part after the player meets the mysterious stranger.
/// The player can stay for funny dialogue.
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

/// The intro dialogue. The player can choose their name and get a cool stick.
class IntroDialogue : public UIWithPlayer
{
public:
    UserInterface *render() override;
};
