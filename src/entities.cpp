#include <iostream>
#include <string>
#include "entities.h"
#include "extras.h"
using namespace std;

Entity::~Entity() = default;
Player::Player() : Entity(0, "", 0, 0), money(0) {}
Player::Player(int health, string name, int atk, int def, int money) : Entity(health, name, atk, def), money(money)
{
    inventory = Inventory();
}

void Player::addMoney(int amount)
{
    if (amount < 0)
    {
        cout << colorizeText("You lost " + to_string(-amount) + " money!", RED) << endl;
    }
    else if (amount == 0)
    {
        cout << "You didn't gain or lose any money." << endl;
    }
    else if (amount > 0)
    {
        cout << colorizeText("You gained " + to_string(amount) + " money!", YELLOW) << endl;
    }
    money += amount;
}

Stats::Stats() : atk(0), def(0) {}
Stats::Stats(int x, int y) : atk(x), def(y) {}

Entity::Entity(int health, string name, int atk, int def):
currHealth(health), maxHealth(health), stats(Stats(atk, def)), name(name) {}

void Entity::attack(Entity &target, int extraDmg)
{
    int calcDmg = max(1, (stats.atk + extraDmg) - target.stats.def);
    target.currHealth = max(0, target.currHealth - calcDmg);
    cout << target.name << " took " << calcDmg << " DMG!" << endl;
}


