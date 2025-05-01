#pragma once

#include <iostream>
using namespace std;

class Player;
class Entity;

/// Base class for all items in the game
class IItem
{
private:
    string desc;
public:
    string name;
    string type;    
    int count; ///< A count of -1 means the item can be used infinitely
    IItem(string n, string t, string d, int c);
    virtual ~IItem();
    virtual string getDesc();
};

/// Enemy items. The use function takes in a player and an enemy.
class IEnemyItem : public IItem
{
public:
    IEnemyItem(string n, string t, string d, int c);
    virtual bool use(Player &player, Entity &enemy) = 0;
};

/// Player items. The use function takes in a player only
class IPlayerItem : public IItem
{
public:
    IPlayerItem(string n, string t, string d, int c);
    virtual bool use(Player &player) = 0;
};

/// Potions set the type to "Potion"
class IPotion : public IPlayerItem
{
public:
    int strength;
    IPotion(string n, string d, int c, int strength);
};

/// Health potions heal the player of course
class HealthPotion : public IPotion
{
public:    
    HealthPotion(int c);
    bool use(Player &player) override;
    string getDesc() override;
};

/// Increases player ATK
class StrengthPotion : public IPotion
{
public:    
    StrengthPotion(int c);
    bool use(Player &player) override;
    string getDesc() override;
};

/// Weapons set the type to "Weapon" and implement the use function
/// cuz all weapons attack lmao.
/// Derived classes only need to set the `name`, `desc`, and `dmg` in the constructor
class Weapon : public IEnemyItem
{
public:
    int dmg;
    Weapon(string name, string desc, int count, int dmg);
    bool use(Player &player, Entity &enemy) override;
};

/// The Cool Stick is the first weapon the player gets
class CoolStick : public Weapon
{
public:
    CoolStick();
    string getDesc() override;
};

/// The OP weapon of the game, the Yamato!!!
class Yamato : public Weapon
{
public:
    Yamato();
    string getDesc() override;
};
