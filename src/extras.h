#pragma once


#include <chrono>
#include <thread>
#include <iostream>
using namespace std;

/// Centers text in a given `num` of characters.
string padTo(string str, const size_t num, const char paddingChar = ' ');

void sleepMS(int ms);

/// ANSI color codes for colored text (ANSI)
enum Color
{
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37
};

/// Colors text.
/// Usually used like this: `\033[31m Your Text Here \033[0m`, where `31` is the color code for red.
string colorizeText(string msg, Color color);

/// Gets an integer from the user. Loops until a valid integer is entered.
int inputNum();

/// Outputs a text character by character with a given delay.
void printAnimate(string msg, int delay = 10, bool animate = true);
