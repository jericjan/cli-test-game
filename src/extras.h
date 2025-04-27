#pragma once


#include <chrono>
#include <thread>
#include <iostream>
using namespace std;

string padTo(string str, const size_t num, const char paddingChar = ' ');

void sleepMS(int ms);

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

string colorizeText(string msg, Color color);

int inputNum();

void printAnimate(string msg, int delay = 10);
