#include "mMap.h"
#include <conio.h>


mMap::mMap(char* name) :
    hProcess(0),
    szNameProcess(name),
    dwProcessId(0)
{

}

bool       mMap::Attach(DWORD dwFlagAccess/* = DEFAULT_ACCESS*/)
{
    Detach();
    dwProcessId = GetProcessIdByName(szNameProcess);
    if (!dwProcessId)
        return false;

    hProcess = OpenProcess(dwFlagAccess, FALSE, dwProcessId);

    if (!hProcess)
        return false;
    return true;
}
void       mMap::Detach()
{
    if (hProcess)
        CloseHandle(hProcess);

    hProcess = NULL;
    dwProcessId = 0;
}
DWORD      mMap::GetProcessIdByName(char* szNameExe)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (hSnapshot == INVALID_HANDLE_VALUE)
        return NULL;

    PROCESSENTRY32 Entry = { NULL };
    Entry.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &Entry))
    {
        CloseHandle(hSnapshot);
        return NULL;
    }

    do {
        if (strcmp(Entry.szExeFile, szNameExe) == 0)
            break;
    } while (Process32Next(hSnapshot, &Entry));

    CloseHandle(hSnapshot);
    return Entry.th32ProcessID;
}


DWORD WINAPI LoadDll(PVOID p)
{
    P_INJECT m_pInject = (P_INJECT)p;


    DWORD i = 0,
        Function = 0,
        count = 0;


    PDWORD ptr = nullptr;
    PWORD  list = nullptr;



    PIMAGE_IMPORT_BY_NAME    pIBN = { 0 };
    PIMAGE_THUNK_DATA        FirstThunk = { 0 };
    PIMAGE_THUNK_DATA        OrigFirstThunk = { 0 };
    DllMainFn                EntryPoint = nullptr;



    PIMAGE_BASE_RELOCATION pIBR = m_pInject->BaseRelocation;
    DWORD delta = (DWORD)((LPBYTE)m_pInject->ImageBase - m_pInject->NtHeaders->OptionalHeader.ImageBase);

    while (pIBR->VirtualAddress)
    {
        if (pIBR->SizeOfBlock >= sizeof(IMAGE_BASE_RELOCATION))
        {
            count = (pIBR->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
            list = (PWORD)(pIBR + 1);

            for (i = 0; i < count; i++)
            {
                if (list[i])
                {
                    ptr = (PDWORD)((LPBYTE)m_pInject->ImageBase + (pIBR->VirtualAddress + (list[i] & 0xFFF)));
                    *ptr += delta;
                }
            }
        }
        pIBR = (PIMAGE_BASE_RELOCATION)((LPBYTE)pIBR + pIBR->SizeOfBlock);
    }
    PIMAGE_IMPORT_DESCRIPTOR pIID = m_pInject->ImportDirectory;
    HMODULE  hModule = NULL;
    while (pIID->Characteristics)
    {
        OrigFirstThunk = (PIMAGE_THUNK_DATA)((LPBYTE)m_pInject->ImageBase + pIID->OriginalFirstThunk);
        FirstThunk = (PIMAGE_THUNK_DATA)((LPBYTE)m_pInject->ImageBase + pIID->FirstThunk);
        hModule = m_pInject->pLoadLibraryA((LPCSTR)m_pInject->ImageBase + pIID->Name);

        if (!hModule)
            return FALSE;

        while (OrigFirstThunk->u1.AddressOfData)
        {
            if (OrigFirstThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)
            {
                Function = (DWORD)m_pInject->pGetProcAddress(hModule, (LPCSTR)(OrigFirstThunk->u1.Ordinal & 0xFFFF));

                if (!Function)
                    return FALSE;
                FirstThunk->u1.Function = Function;
            }
            else
            {
                pIBN = (PIMAGE_IMPORT_BY_NAME)((LPBYTE)m_pInject->ImageBase + OrigFirstThunk->u1.AddressOfData);
                Function = (DWORD)m_pInject->pGetProcAddress(hModule, (LPCSTR)pIBN->Name);

                if (!Function)
                    return FALSE;

                FirstThunk->u1.Function = Function;
            }
            OrigFirstThunk++;
            FirstThunk++;
        }
        pIID++;
    }

    if (m_pInject->NtHeaders->OptionalHeader.AddressOfEntryPoint)
    {
        EntryPoint = (DllMainFn)((LPBYTE)m_pInject->ImageBase + m_pInject->NtHeaders->OptionalHeader.AddressOfEntryPoint);
        return EntryPoint((HMODULE)m_pInject->ImageBase, DLL_PROCESS_ATTACH, nullptr);
    }
    return TRUE;
}
DWORD WINAPI LoadDllEnd()
{
    return 0;
}

BOOL  mMap::InjectM(PVOID  pBuffer)
{
    if (!dwProcessId)
        return FALSE;
    if (!hProcess)
        return FALSE;


    PIMAGE_DOS_HEADER     pIDH = { 0 };
    PIMAGE_NT_HEADERS     pINH = { 0 };
    PIMAGE_SECTION_HEADER pISH = { 0 };

    pIDH = (PIMAGE_DOS_HEADER)pBuffer;
    if (pIDH->e_magic != IMAGE_DOS_SIGNATURE)
    {
        std::cout << "Error:: Invalid executable image" << std::endl;
        VirtualFree(pBuffer, 0, MEM_RELEASE);
        return FALSE;
    }

    pINH = (PIMAGE_NT_HEADERS)((LPBYTE)pBuffer + pIDH->e_lfanew);
    if (pINH->Signature != IMAGE_NT_SIGNATURE)
    {
        std::cout << "Error:: Invalid PE header" << std::endl;
        VirtualFree(pBuffer, 0, MEM_RELEASE);
        return FALSE;
    }

    if (!(pINH->FileHeader.Characteristics & IMAGE_FILE_DLL))
    {
        std::cout << "Error:: The image is not DLL" << std::endl;
        VirtualFree(pBuffer, 0, MEM_RELEASE);
        return FALSE;
    }

    PVOID  image = VirtualAllocEx(hProcess, NULL, pINH->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!image)
    {
        std::cout << "Error::  Allocate memory for the DLL::" << GetLastError() << std::endl;
        VirtualFree(pBuffer, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    if (!WriteProcessMemory(hProcess, image, pBuffer, pINH->OptionalHeader.SizeOfHeaders, NULL))
    {
        std::cout << "Error::  Copy headers to process::" << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, image, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        VirtualFree(pBuffer, 0, MEM_RELEASE);
        return FALSE;
    }

    pISH = (PIMAGE_SECTION_HEADER)(pINH + 1);
    for (DWORD i = 0; i < pINH->FileHeader.NumberOfSections; i++)
    {
        WriteProcessMemory(hProcess, (PVOID)((LPBYTE)image + pISH[i].VirtualAddress), (PVOID)((LPBYTE)pBuffer + pISH[i].PointerToRawData), pISH[i].SizeOfRawData, NULL);
    }

    PVOID mem = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem)
    {
        std::cout << "Error::  Allocate memory for the loader code::" << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, image, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        VirtualFree(pBuffer, 0, MEM_RELEASE);
        return FALSE;
    }
    INJECT m_pInject = { 0 };
    memset(&m_pInject, 0, sizeof(INJECT));
    m_pInject.ImageBase = image;
    m_pInject.NtHeaders = (PIMAGE_NT_HEADERS)((LPBYTE)image + pIDH->e_lfanew);
    m_pInject.BaseRelocation = (PIMAGE_BASE_RELOCATION)((LPBYTE)image + pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
    m_pInject.ImportDirectory = (PIMAGE_IMPORT_DESCRIPTOR)((LPBYTE)image + pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    m_pInject.pLoadLibraryA = LoadLibraryA;
    m_pInject.pGetProcAddress = GetProcAddress;

    WriteProcessMemory(hProcess, mem, &m_pInject, sizeof(INJECT), NULL);
    WriteProcessMemory(hProcess, (PVOID)((P_INJECT)mem + 1), LoadDll, (DWORD)LoadDllEnd - (DWORD)LoadDll, NULL);

    HANDLE    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)((P_INJECT)mem + 1), mem, 0, NULL);
    if (!hThread)
    {
        std::cout << "Error::  Unable to execute loader code::" << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, mem, 0, MEM_RELEASE);
        VirtualFreeEx(hProcess, image, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        VirtualFree(pBuffer, 0, MEM_RELEASE);
        return FALSE;
    }
    DWORD ExitCode = 0;
    WaitForSingleObject(hThread, INFINITE);
    GetExitCodeThread(hThread, &ExitCode);

    if (!ExitCode)
    {
        VirtualFreeEx(hProcess, mem, 0, MEM_RELEASE);
        VirtualFreeEx(hProcess, image, 0, MEM_RELEASE);

        CloseHandle(hThread);
        CloseHandle(hProcess);

        VirtualFree(pBuffer, 0, MEM_RELEASE);
        return FALSE;
    }

    CloseHandle(hThread);
    VirtualFreeEx(hProcess, mem, 0, MEM_RELEASE);
    CloseHandle(hProcess);


    if (pINH->OptionalHeader.AddressOfEntryPoint)
    {
        printf_s("DLL entry point: %#x\n", (PVOID)((LPBYTE)image + pINH->OptionalHeader.AddressOfEntryPoint));
    }
    VirtualFree(pBuffer, 0, MEM_RELEASE);
    std::cout << "DLL injected!" << std::endl;
    return TRUE;
}
BOOL  mMap::InjectP(char* pDll)
{
    if (!dwProcessId)
        return FALSE;
    if (!hProcess)
        return FALSE;

    HANDLE  hFile = CreateFileA(pDll, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cout << "Error:: Open the DLL::" << GetLastError() << std::endl;
        return FALSE;
    }

    DWORD   dwFileSize = GetFileSize(hFile, NULL);
    PVOID   pBuffer = VirtualAlloc(NULL, dwFileSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!pBuffer)
    {
        std::cout << "Error:: Allocate memory for DLL data::" << GetLastError() << std::endl;
        CloseHandle(hFile);
        return FALSE;
    }
    DWORD dwRead = 0;
    if (!ReadFile(hFile, pBuffer, dwFileSize, &dwRead, NULL))
    {
        std::cout << "Error:: Read the DLL::" << GetLastError() << std::endl;
        VirtualFree(pBuffer, 0, MEM_RELEASE);
        CloseHandle(hFile);
        return FALSE;
    }

    CloseHandle(hFile);
    if (!InjectM(pBuffer))
        return FALSE;

    return TRUE;
}


mMap::~mMap()
{
    Detach();
}