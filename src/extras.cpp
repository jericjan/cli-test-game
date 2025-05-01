#include "extras.h"
#include <string>

string padTo(string str, const size_t num, const char paddingChar)
{
    if (num > str.size())
    {
        div_t divided = div(num - str.size(), 2);
        size_t leftSpace = divided.quot;
        size_t rightSpace = divided.quot + divided.rem;
        str.insert(str.begin(), leftSpace, paddingChar);
        str.insert(str.end(), rightSpace, paddingChar);
    }
    return str;
}

void sleepMS(int ms)
{
    this_thread::sleep_for(chrono::milliseconds(ms));
}

string colorizeText(string msg, Color color)
{
    return string("\033[") + to_string(color) + "m" + msg + "\033[0m";
}

int inputNum()
{

    int result;

    while (!(cin >> result))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "That's not a number!" << std::endl;
    }

    return result;
}

void printAnimate(string msg, int delay)
{
    cout << msg;
    // for (auto &ch : msg)
    // {
    //     cout << ch;
    //     sleepMS(delay);
    // }
}
