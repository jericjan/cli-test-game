#pragma once

#include <vector>
#include <iostream>
using namespace std;

class Entity;

class IItem;

class Player;

class Inventory
{
public:
    vector<IItem *> items;

    void addItem(IItem *item);

    void listItems();

    // returns true if the item was used successfully, false if it was not
    bool useItem(int idx, Player &player, Entity *enemy);

    void dropItem(int idx);
};
