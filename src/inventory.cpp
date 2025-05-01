#include <iostream>
#include <string>
#include "inventory.h"
#include "extras.h"
#include "items.h"

IItem *Inventory::addItem(IItem *item, bool animate)
{
    for (IItem *existingItem : items)
    {
        if (existingItem->matches(item))
        {
            if (!existingItem->isInfinite())
            {
                existingItem->increment(item->getCount());
            } else {  // Infinite item alr exists
                return existingItem;
            }
            string msg = "You now have " + to_string(existingItem->getCount()) + "x " + existingItem->getName() + "!";
            string border = "\n" + string(msg.length(), '=') + "\n";
            printAnimate(border, 1, animate);
            printAnimate(msg, 10, animate);
            printAnimate(border, 1, animate);
            return nullptr;
        }
    }

    string msg = item->getName() + " added to inventory!";
    string border = "\n" + string(msg.length(), '=') + "\n";
    printAnimate(border, 1, animate);
    printAnimate(msg, 10, animate);
    printAnimate(border, 1, animate);
    items.push_back(item);
    return nullptr;
}

void Inventory::listItems()
{
    int idx = 1;

    cout << padTo("[Name]", 20) << "|" << padTo("[Type]", 20) << "|" << padTo("[Description]", 50) << "|" << padTo("[Count]", 20) << endl;
    for (IItem *item : items)
    {
        string count = item->isInfinite() ? "Infinite" : to_string(item->getCount()) + "x";
        string numberedName = string("[") + to_string(idx) + "] " + item->getName();
        cout << padTo(numberedName, 20) << "|" << padTo(item->getType(), 20) << "|" << padTo(item->getDesc(), 50) << "|" << padTo(count, 20) << endl;
        idx++;
    }
    cout << "Type the # of the item to use, 0 to exit > ";
}

// idx is 1-indexed
bool Inventory::useItem(int idx, Player &player, Entity *enemy)
{
    IItem *item;
    try
    {
        item = items.at(idx - 1);
    }
    catch (...)
    {
        cout << "Invalid item number!" << endl;
        return false;
    }

    bool useResult;
    if (IPlayerItem *playerItem = dynamic_cast<IPlayerItem *>(item))
    {
        useResult = playerItem->use(player);
    }
    else if (IEnemyItem *enemyItem = dynamic_cast<IEnemyItem *>(item))
    {
        if (enemy == nullptr)
        {
            cout << "You can't use that item here!" << endl;
            return false;
        }
        useResult = enemyItem->use(player, *enemy);
    }

    if (!useResult)
    {
        return false;
    }

    if (!item->isInfinite())
    {
        item->decrement(1);
        if (item->isDepleted())
        {
            items.erase(items.begin() + idx - 1);
            delete item;
        }
    }
    return true;
}

void Inventory::dropItem(int idx)
{
}
