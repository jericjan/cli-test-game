#pragma once

#include <iostream>
using namespace std;

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

class IEnemyItem : public IItem
{
public:
    IEnemyItem(string n, string t, string d, int c);
    virtual bool use(Player &player, Entity &enemy) = 0;
};

class IPlayerItem : public IItem
{
public:
    IPlayerItem(string n, string t, string d, int c);
    virtual bool use(Player &player) = 0;
};

class IPotion : public IPlayerItem
{
public:
    IPotion(string n, string d, int c);
};

class HealthPotion : public IPotion
{
public:
    static const int healAmount = 100; // declared static to constructor can use it
    HealthPotion(int c);
    bool use(Player &player) override;
};

class IWeapon : public IEnemyItem
{
public:
    int dmg;
    IWeapon(string name, string desc, int count, int dmg);
    bool use(Player &player, Entity &enemy) override;
};

class CoolStick : public IWeapon
{
public:
    static const int dmg = 60;
    CoolStick();
};

class Yamato : public IWeapon
{
public:
    Yamato();
};
