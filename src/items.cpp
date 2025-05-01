
#include <iostream>
#include <string>
#include "entities.h"
#include "items.h"
using namespace std;

IItem::IItem(string n, string t, string d, int c) : name(n), type(t), desc(d), count(c) {}
IItem::~IItem() = default; // required for polymorphic base class for dynamic_cast

string IItem::getDesc() {
    return desc;
}

IEnemyItem::IEnemyItem(string n, string t, string d, int c) : IItem(n, t, d, c) {}

IPlayerItem::IPlayerItem(string n, string t, string d, int c) : IItem(n, t, d, c) {}

IPotion::IPotion(string n, string d, int c, int strength) : IPlayerItem(n, "Potion", d, c), strength(strength) {}

HealthPotion::HealthPotion(int c) :
IPotion("Health Potion", "", c, 100) {}
string HealthPotion::getDesc()
{
    return "Heals the user " + to_string(strength) + "HP";
}

bool HealthPotion::use(Player &player)
{
    if (player.currHealth == player.maxHealth)
    {
        cout << "You're already at full health!" << endl;
        return false;
    }
    player.currHealth = min(player.maxHealth, player.currHealth + strength);
    return true;
}

StrengthPotion::StrengthPotion(int c) : IPotion("Strength Potion", "", c, 100) {}
string StrengthPotion::getDesc()
{
    return "Increases the user's ATK by " + to_string(strength) + "!";
}

bool StrengthPotion::use(Player &player)
{
    player.stats.atk += strength;    
    cout << "Your ATK increased by " + to_string(strength) + "!" << endl;
    cout << "Your new ATK is " + to_string(player.stats.atk) + "!" << endl;
    return true;
}



Weapon::Weapon(string name, string desc, int count, int dmg) : IEnemyItem(name, "Weapon", desc, count), dmg(dmg) {}
bool Weapon::use(Player &player, Entity &enemy)
{
    player.attack(enemy, dmg);
    return true;
}

CoolStick::CoolStick() : Weapon("Cool Stick", "", -1, 60) {}
string CoolStick::getDesc()
{
    return "A cool stick that does " + to_string(dmg) + " damage!";
}


Yamato::Yamato() : Weapon("Yamato", "", -1, 1000) {}
string Yamato::getDesc()
{
    return "A legendary sword that does " + to_string(dmg) + " damage!";
}