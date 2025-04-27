#include <iostream>
#include "entities.h"
#include "extras.h"
using namespace std;

Player::Player() : Entity(0, "", 0, 0), money(0) {}
Player::Player(int _health, string _name, int _atk, int _def, int _money) : Entity(_health, _name, _atk, _def), money(_money)
{
    inventory = Inventory();
}

    Stats::Stats() : atk(0), def(0) {}
    Stats::Stats(int x, int y) : atk(x), def(y) {}

Entity::Entity(int _health, string _name, int atk, int def)
{
    currHealth = _health;
    maxHealth = _health;
    name = _name;
    stats = Stats(atk, def);
}

void Entity::attack(Entity &target, int extraDmg)
{
    int calcDmg = max(1, (stats.atk + extraDmg) - target.stats.def);
    target.currHealth = max(0, target.currHealth - calcDmg);
    cout << target.name << " took " << calcDmg << " DMG!" << endl;
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
