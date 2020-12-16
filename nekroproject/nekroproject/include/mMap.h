#pragma once
#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>
#define DEFAULT_ACCESS  PROCESS_VM_READ |PROCESS_VM_WRITE | \
                        PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION


using LoadLibraryFn = HMODULE(WINAPI*)(LPCSTR);
using GetProcAddressFn = FARPROC(WINAPI*)(HMODULE, LPCSTR);
using DllMainFn = BOOL(WINAPI*)(HMODULE, DWORD, PVOID);


typedef struct _INJECT
{
    PVOID                    ImageBase;
    PIMAGE_NT_HEADERS        NtHeaders;
    PIMAGE_BASE_RELOCATION   BaseRelocation;
    PIMAGE_IMPORT_DESCRIPTOR ImportDirectory;
    LoadLibraryFn            pLoadLibraryA;
    GetProcAddressFn         pGetProcAddress;
}INJECT, * P_INJECT;


class mMap
{
public:
    mMap(char* name);
    virtual ~mMap();
    bool                        Attach(DWORD dwFlagAccess = DEFAULT_ACCESS);
    void       Detach();
    BOOL   InjectM(PVOID  pBuffer);
    BOOL   InjectP(char* name);
protected:
    DWORD       GetProcessIdByName(char* name);
private:
    HANDLE hProcess;
    DWORD  dwProcessId;
    char* szNameProcess;
};