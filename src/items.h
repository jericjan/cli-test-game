#pragma once

#include <iostream>
using namespace std;

/// Base class for all items in the game
class IItem
{
public:
    string name;
    string type;
    string desc;
    int count; // A count of -1 means the item can be used infinitely
    IItem(string n, string t, string d, int c);
    virtual ~IItem();

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
    IPotion(string n, string d, int c);
};

/// Health potions heal the player of course
class HealthPotion : public IPotion
{
public:
    static const int healAmount = 100; // declared static to constructor can use it
    HealthPotion(int c);
    bool use(Player &player) override;
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
    static const int dmg = 60;
    CoolStick();
};

/// The OP weapon of the game, the Yamato!!!
class Yamato : public Weapon
{
public:
    static const int dmg = 500;
    Yamato();
};
