#pragma once

#include <iostream>
#include "inventory.h"
using namespace std;

class Stats
{
public:
    int atk;
    int def;
    Stats();
    Stats(int x, int y);
};

class Entity
{
public:
    int maxHealth;
    int currHealth;
    Stats stats;
    string name;
    Entity(int _health, string _name, int atk, int def);

    void attack(Entity &target, int extraDmg = 0);
};

class Player : public Entity
{
public:
    Inventory inventory;
    int money;
    Player();
    Player(int _health, string _name, int _atk, int _def, int _money);

    void addMoney(int amount);
};