#pragma once

#include <vector>
#include <iostream>
using namespace std;

class Entity;

class IItem;

class Player;

/// Inventory class that holds all items for the player.
class Inventory
{
public:
    vector<IItem *> items;

    void addItem(IItem *item);

    /// Lists all items in the inventory.
    /// Should probably include the user input functionality too.
    void listItems();

    /// Use an item.
    /// Returns true if the item was used successfully, false if it was not
    /// @param idx 1-indexed item number
    /// @param player The player using the item
    /// @param enemy The enemy the item is being used on. If the item is not an enemy item, this can be nullptr.
    bool useItem(int idx, Player &player, Entity *enemy);

    /// drop an item. not implemented yet.
    /// @param idx 1-indexed item number
    void dropItem(int idx);
};
