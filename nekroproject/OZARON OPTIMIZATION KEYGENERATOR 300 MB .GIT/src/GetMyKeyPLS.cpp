#include <Lmcons.h>

#include <iostream>

#include <windows.h>

#include <string>

#include <filesystem>

#include <tlhelp32.h>

#include <shlwapi.h>

#include <conio.h>

#include <stdio.h>

#include <io.h>

#include "..\include\crypt.h"


#define UNLEN 256

using namespace std;

DWORD getHWID()
{
    wchar_t NameBuffer[MAX_PATH];
    wchar_t SysNameBuffer[MAX_PATH];
    DWORD SerialNumber;
    DWORD Length;
    DWORD File;
    GetVolumeInformation(L"C:\\", NameBuffer, sizeof(NameBuffer), &SerialNumber, &Length, &File, SysNameBuffer,
        sizeof(SysNameBuffer));
    return SerialNumber;
}

int main()
{
    char key[1024];
    string hwid;
    printf("[NEKRO] KEY EDITOR\nENTER YOUR HWID PLS\n");

    string keycrypt = "DLq7lMCcuMIIVD13MRAuLv7H2fdW9f6g";
    string keys = "QQ5CjZu3kEbHDjeBBFnR0";

    getline(cin, hwid);

    string fucker = decrypt_vigenere(hwid, keys);

    string crypting = keys + fucker;

    string zalupa = encrypt_vigenere(crypting, keycrypt);

    cout << "KEY: " + zalupa << endl;
   
    cin >> hwid;
}

