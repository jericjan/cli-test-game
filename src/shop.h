#pragma once

#include "items.h"
#include <vector>
#include "entities.h"

class ShopItem
{
public:
    IItem *item;
    int price;
    ShopItem(IItem *item, int price);
};

class Shop {
public:
    vector<ShopItem> items;
    void addItem(IItem *item, int price);

    /// Remove an item from the shop.
    /// @param idx 1-indexed item number
    void removeItem(int idx); 
    void listItems();

    /// @brief Buy an item from the shop.
    /// @param player The player buying the item
    /// @param idx 1-indexed item number
    bool buyItem(Player &player, int idx);
};