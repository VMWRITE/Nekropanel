#pragma once
#define UNLEN 256

#include <string>
#include <windows.h>
#include <iostream>
#include "ConsoleColor.h"

void nekrologsucc(std::string log, std::string secmsg) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout << cyan << "[+] " << grey << log << darkred << secmsg << white << std::endl;
}

void nekrocin(std::string msg)
{
    char buffer[UNLEN + 1];
    DWORD size;
    size = sizeof(buffer);
    if (GetUserNameA(buffer, &size) == 0)
        throw "Error GetUserName";
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout << darkgreen << "root@" << buffer << white << ":" << cyan << "~" << white << "$" << grey << msg << white << std::endl;
}
