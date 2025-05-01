#include "shop.h"

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
void Shop::listItems() {    
    int idx = 1;
    for (ShopItem &shopItem : items) {
        cout << "[" << idx << "] " << shopItem.item->name << " (" << shopItem.price << ")" << endl;
        idx += 1;
      }
    cout << "[" << idx << "] Leave" << endl;
}

bool Shop::buyItem(Player &player, int idx) {
    idx -= 1;
    if (idx < 0 || idx >= items.size()) {
        cout << "Invalid item index." << endl;
        return false;
    }
    ShopItem &shopItem = items[idx];
    
    if (player.money < shopItem.price) {
        cout << "You don't have enough money!" << endl;
        return false;
    }
    
    player.addMoney(-shopItem.price);
    player.inventory.addItem(shopItem.item);
    cout << "You bought a " << shopItem.item->name << "!" << endl;
    return true;
}