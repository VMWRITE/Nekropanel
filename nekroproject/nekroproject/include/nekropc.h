#pragma once

#define UNLEN 256
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <string>


namespace nekropc {

    DWORD getHWIDEX() {
        wchar_t NameBuffer[MAX_PATH];
        wchar_t SysNameBuffer[MAX_PATH];
        DWORD SerialNumber;
        DWORD Length;
        DWORD File;
        GetVolumeInformation(L"C:\\", NameBuffer, sizeof(NameBuffer), &SerialNumber, &Length, &File, SysNameBuffer,
            sizeof(SysNameBuffer));
        return SerialNumber;
    }


    std::string GetMyUserNameEx()
    {
        char buffered[UNLEN + 1];
        DWORD sizerer;
        sizerer = sizeof(buffered);
        if (GetUserNameA(buffered, &sizerer) == 0)
            throw "Error GetUserName";
        return buffered;
    }
}
