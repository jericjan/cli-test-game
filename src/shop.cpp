#include "shop.h"
#include "extras.h"
ShopItem::ShopItem(IItem *item, int price) : item(item), price(price) {}

void Shop::addItem(IItem *item, int price) {
    items.push_back(ShopItem(item, price));
}

void Shop::removeItem(int idx) {
    idx -= 1;
    if (idx < 0 || idx >= items.size()) {
        cout << "Invalid item index." << endl;
        return;
    }
    items.erase(items.begin() + idx);
}
void Shop::listItems(Player &player) {    
    int idx = 1;
    cout << "Money: " << player.money << endl;
    cout << "[0] Leave" << endl;
    for (ShopItem &shopItem : items) {
        cout << "[" << idx << "] " << shopItem.item->name << " [Cost: " << shopItem.price << "]" << endl;
        idx += 1;
      }    
}

bool Shop::buyItem(Player &player, int idx) {
    idx -= 1;
    if (idx < 0 || idx >= items.size()) {
        cout << "Invalid item index." << endl;
        return false;
    }
    ShopItem &shopItem = items[idx];
    
    if (player.money < shopItem.price) {
        cout << colorizeText("You don't have enough money!", RED) << endl;
        return false;
    }

    player.addMoney(-shopItem.price);    
    cout << "You bought a " << shopItem.item->name << "!" << endl;
    player.inventory.addItem(shopItem.item);
    return true;
}