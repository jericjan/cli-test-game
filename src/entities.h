#pragma once

#include <iostream>
#include "inventory.h"


using namespace std;

class ShopItem;

/// Stats for all entities
struct Stats {
    int atk;
    int def;
    Stats(int atk, int def);
};

struct Health {
    int current;
    int max;
    Health(int current, int max);
};

/// Base class for all entities in the game. This includes the player and enemies.
class Entity
{
protected:
    Health health;        
    string name;    
    Stats stats;
public:
    
    Entity(int health, string name, int atk, int def);
    virtual ~Entity();
    void attack(Entity &target, int extraDmg = 0);
    int getHealth();
    int getMaxHealth();
    bool isFullHealth();
    void fullyHeal();
    bool isAlive();
    void heal(int amount);
    void kill();
    bool isDead();
    string getName();
    int getAtk();
    int getDef();
    void boostAtk(int amount);
    void boostDef(int amount);
};

/// The Player class is an Entity that has an inventory and money.
class Player : public Entity
{
private:
    int money;    
public:
    Inventory inventory;
    
    Player();
    Player(int health, string name, int atk, int def, int money);

    void addMoney(int amount);
    int getMoney();
    void setMoney(int amount);
    bool canAfford(int amount);
    bool canAfford(ShopItem &item);
};