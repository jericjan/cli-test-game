#include <iostream>
#include <string>
#include "entities.h"
#include "extras.h"
#include "shop.h"
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

int Player::getMoney()
{
    return money;
}

void Player::setMoney(int amount)
{
    money = amount;
}

bool Player::canAfford(int amount)
{
    return money >= amount;
}

bool Player::canAfford(ShopItem &item){
    return canAfford(item.price);
}

Stats::Stats(int atk, int def) : atk(atk), def(def) {}

Health::Health(int current, int max) : current(current), max(max) {}

Entity::Entity(int health, string name, int atk, int def):
health(health, health), stats(Stats(atk, def)), name(name) {}

void Entity::attack(Entity &target, int extraDmg)
{
    int calcDmg = max(1, (stats.atk + extraDmg) - target.stats.def);
    target.health.current = max(0, target.health.current - calcDmg);
    cout << target.getName() << " took " << calcDmg << " DMG!" << endl;
}

int Entity::getHealth() {
    return health.current;
}
int Entity::getMaxHealth(){
    return health.max;
}
bool Entity::isFullHealth(){
    return health.current == health.max;
}
void Entity::fullyHeal(){
    health.current = health.max;
    printAnimate(name + " was fully healed!\n");
}

bool Entity::isAlive(){
    return health.current > 0;
}
void Entity::heal(int amount){
    health.current = min(health.max, health.current + amount);
}

void Entity::kill(){
    health.current = 0;
}

string Entity::getName() {
    return name;
}

int Entity::getAtk() {
    return stats.atk;
}

int Entity::getDef() {
    return stats.def;
}

void Entity::boostAtk(int amount) {
    stats.atk += amount;    
}

void Entity::boostDef(int amount) {
    stats.def += amount;    
}

void Entity::printInfo(bool showName) {
    if (showName)
    {
        cout << "[" << getName() << "] ";
    }
    cout  << "HP: " << getHealth() << "/" << getMaxHealth() << " ATK: " << getAtk() << " DEF: " << getDef() << endl;
}