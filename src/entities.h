#pragma once

#include <iostream>
#include "inventory.h"
using namespace std;

/// Stats for all entities
class Stats
{
public:
    int atk;
    int def;
    Stats();
    Stats(int x, int y);
};

/// Base class for all entities in the game. This includes the player and enemies.
class Entity
{
public:
    int maxHealth;
    int currHealth;
    Stats stats;
    string name;
    Entity(int health, string name, int atk, int def);
    virtual ~Entity();
    void attack(Entity &target, int extraDmg = 0);
};

/// The Player class is an Entity that has an inventory and money.
class Player : public Entity
{
public:
    Inventory inventory;
    int money;
    Player();
    Player(int health, string name, int atk, int def, int money);

    void addMoney(int amount);
};