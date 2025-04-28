
#include <iostream>
#include "entities.h"
#include "items.h"
using namespace std;

IItem::IItem(string n, string t, string d, int c) : name(n), type(t), desc(d), count(c) {}
IItem::~IItem() = default; // required for polymorphic base class for dynamic_cast

IEnemyItem::IEnemyItem(string n, string t, string d, int c) : IItem(n, t, d, c) {}

IPlayerItem::IPlayerItem(string n, string t, string d, int c) : IItem(n, t, d, c) {}

IPotion::IPotion(string n, string d, int c) : IPlayerItem(n, "Potion", d, c) {}

HealthPotion::HealthPotion(int c) : IPotion("Health Potion", "Heals the user " + to_string(healAmount) + "HP", c) {}

bool HealthPotion::use(Player &player)
{
    if (player.currHealth == player.maxHealth)
    {
        cout << "You're already at full health!" << endl;
        return false;
    }
    player.currHealth = min(player.maxHealth, player.currHealth + healAmount);
    return true;
}

Weapon::Weapon(string name, string desc, int count, int dmg) : IEnemyItem(name, "Weapon", desc, count), dmg(dmg) {}
bool Weapon::use(Player &player, Entity &enemy)
{
    player.attack(enemy, dmg);
    return true;
}

CoolStick::CoolStick() : Weapon("Cool Stick", "A cool stick some stranger gave you. " + to_string(dmg) + "DMG", -1, dmg) {}

Yamato::Yamato() : Weapon("Yamato", "A legendary sword that does " + to_string(dmg) + " damage!", -1, dmg) {}