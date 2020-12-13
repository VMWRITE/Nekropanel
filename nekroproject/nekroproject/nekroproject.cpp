// nekroproject.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//



#define _CRT_SECURE_NO_WARNINGS
#include <Lmcons.h>

#include <iostream>

#include <windows.h>

#include <string>

#include "sha512.h"

#include <iostream>

#include <filesystem>

#include <tlhelp32.h>

#include <shlwapi.h>

#include <conio.h>

#include <stdio.h>

#include <io.h>

#include <fstream>

#include <vector>

#include <sstream>

#include <utility>

using namespace std;
std::vector < std::string > explode(std::string
    const& s, char delim) {
    std::vector < std::string > result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim);) {
        result.push_back(std::move(token));
    }

    return result;
}
bool FileExists(const char* fname) {
    return _access(fname, 0) != -1;
}
bool nekroexists(string file) {
    string path = getenv("TEMP");
    string fullpath = path + "/" + file;
    return FileExists(fullpath.c_str());
}
DWORD getHWID() {
    wchar_t NameBuffer[MAX_PATH];
    wchar_t SysNameBuffer[MAX_PATH];
    DWORD SerialNumber;
    DWORD Length;
    DWORD File;
    GetVolumeInformation(L"C:\\", NameBuffer, sizeof(NameBuffer), &SerialNumber, &Length, &File, SysNameBuffer,
        sizeof(SysNameBuffer));
    return SerialNumber;
}

string GetMyUserName()
{
    char buffer[UNLEN + 1];		// буфер
    DWORD size;			// размер
    size = sizeof(buffer);		// размер буфера
    if (GetUserNameA(buffer, &size) == 0)
        throw "Error GetUserName";	// при ошибке функция вернет 0
    return buffer;
}

void nekrolog(int color, string log) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    printf("[NEKRO] ");
    SetConsoleTextAttribute(hConsole, 15);
    printf(log.c_str());
    printf("\n");
}

void nekrologsucc( string log, string secmsg ) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 11);
    printf("[+] ");
    SetConsoleTextAttribute(hConsole, 8);
    printf(log.c_str());
    SetConsoleTextAttribute(hConsole, 4);
    printf(secmsg.c_str());
    SetConsoleTextAttribute(hConsole, 15);
    printf("\n");
}

void nekrocin(string msg)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 2);
    string rootmsg = "root@" + GetMyUserName();
    printf(rootmsg.c_str());
    SetConsoleTextAttribute(hConsole, 15);
    printf(":");
    SetConsoleTextAttribute(hConsole, 11);
    printf("~");
    SetConsoleTextAttribute(hConsole, 15);
    printf("$");
    SetConsoleTextAttribute(hConsole, 8);
    printf(msg.c_str());
    SetConsoleTextAttribute(hConsole, 15);
}


void nekrologsec(string log) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 11);
    printf("[~] ");
    SetConsoleTextAttribute(hConsole, 8);
    printf(log.c_str());
    printf("\n");
}


void nekrocreate(string file, string content) {
    string path = getenv("TEMP");
    FILE* filek;
    string fullpath;
    fullpath = path + "/" + file;
    filek = fopen(fullpath.c_str(), "w");

    fputs(content.c_str(), filek);

    fclose(filek);
}

void nekrodel(string file) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12);
    printf("[NEKRO] ");
    SetConsoleTextAttribute(hConsole, 15);
    printf("Deleted file - ");
    printf(file.c_str());
    remove(file.c_str());
    printf("\n");
}

wchar_t* cs2wchar(const char* charArray) {
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}

bool checkLicense(string argv) {
    string key = argv.c_str();
    string hwid = to_string(getHWID());
    string keycrypt = "DLq7lMCcuMIIVD13MRAuLv7H2fdW9f6g";
    string keys = "QQ5CjZu3kEbHDjeBBFnR0";

    string crypting = keys + hwid;
    if (strcmp(key.c_str(), encrypt_vigenere(crypting, keycrypt).c_str()) == 0) {
        MessageBoxA(0, "Your license has activated!", "[NEKRO]", MB_ICONINFORMATION);
        HKEY hKey;
        if (RegCreateKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Nekro\\License", 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL)) {
            MessageBoxA(0, "Can't create RegKey", "RegKey!", MB_OK);
            return false;
        }
        RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Nekro\\License", 0,
            KEY_ALL_ACCESS, &hKey);
        LPCTSTR stuff = cs2wchar(encrypt_vigenere(crypting, keycrypt).c_str());

        size_t size;
        size = sizeof(stuff);

        if (RegSetValueEx(hKey, L"lKey", 0, REG_SZ, (LPBYTE)stuff, 1024) != ERROR_SUCCESS) {
            MessageBoxA(0, "Can't set Reg value", "RegKey!", MB_OK);
            return false;
        }
        return true;
    }
    else {
        MessageBoxA(0, "Your code have errors!", "[NEKRO]", MB_ICONHAND);
        return false;
    }
}

void cmdlist()
{
    const char* str[] = {
  "8 - Kill processes",
  "7 - Change cooldown",
  "6 - Change params",
  "5 - Change game",
  "4 - Open account list",
  "3 - Start Bots",
  "2 - Injector",
  "1 - Clear Logs"
    };

    for (int i = 8 - 1; i >= 0; i--)
        nekrolog(9, str[i]);
}

void start()
{
    string path = getenv("TEMP");
    nekrolog(14, "Welcome, " + GetMyUserName());
    nekrolog(4, "Build: v1.0 | 17:33 | 12.12.2020");
    nekrolog(14, "Loading accounts");
    if (nekroexists("nekro.dat")) {
        printf("\n");
        printf("--------------------------ACCOUNT LIST--------------------------\n");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 15);
        printf("UID | LOGIN | PASS\n");
        string line;

        std::ifstream in(path + "/nekro.dat"); // окрываем файл для чтения
        int ponos = 1;
        if (in.is_open()) {
            while (getline(in, line)) {

                auto dura = explode(line, ':');
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 3);
                cout << ponos;
                SetConsoleTextAttribute(hConsole, 8);
                printf(" | %s | %s\n", dura[0].c_str(), dura[1].c_str());
                SetConsoleTextAttribute(hConsole, 15);
                ponos++;
            }

        } in.close(); // закрываем файл
        cout << "TOTAL: ";
        SetConsoleTextAttribute(hConsole, 4);
        cout << ponos - 1 << endl;
        SetConsoleTextAttribute(hConsole, 15);
        printf("----------------------------------------------------------------\n");
    }
    else {
        nekrocreate("nekro.dat", "");

        nekrolog(12, "temp/nekro.dat created (Account List)");
    }



    string type;
    while (true) {
        cmdlist();
        nekrocin(" ");
        getline(cin, type);
        if (type == "1") {
            nekrolog(3, "Selected - Clear Logs");
            WIN32_FIND_DATAA data;
            HANDLE hFind = FindFirstFileA("C:/Windows/Prefetch/*.pf", &data); // DIRECTORY

            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    string file = data.cFileName;

                    if (file.find("LOAD") != std::string::npos or file.find("DLL") != std::string::npos or file.find("INJECT") != std::string::npos or file.find("CHEAT") != std::string::npos or file.find("HAX") != std::string::npos or file.find("HACK") != std::string::npos) {

                        nekrodel("C:/Windows/Prefetch/" + string(file.c_str()));
                    }

                } while (FindNextFileA(hFind, &data));
                FindClose(hFind);
            }
        }
        else if (type == "2") {
            nekrolog(10, "Selected - Injector");

        }
        else if (type == "3") {
            nekrolog(13, "Selected - Start Bots");

            nekrolog(12, "Starting bots...");
            string line;

            string path = getenv("TEMP");
            std::ifstream in(path + "/nekro.dat"); // окрываем файл для чтения
            string params;
            string fullpath2 = path + "/nekroprelaunch.dat";

            std::ifstream ieeen(fullpath2); // окрываем файл для чтения
            if (ieeen.is_open()) {
                while (getline(ieeen, params)) {
                    params = params;
                }

            } ieeen.close(); // закрываем файл
            int numberone = 1;
            if (in.is_open()) {
                printf("UID | LOGIN | PASSWORD | APPID | COOLDOWN\n");
                printf("\n---------------------------PARAMETERS---------------------------\n");
                nekrolog(4, params);
                printf("----------------------------------------------------------------\n\n");
                while (getline(in, line)) {

                    auto dura = explode(line, ':');

                    HKEY rKey;
                    char steamPath[1024];
                    DWORD RegetPath = 1024;
                    RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Valve\\Steam", 0, KEY_QUERY_VALUE, &rKey);
                    DWORD dwType;
                    dwType = REG_SZ;
                    RegQueryValueExA(rKey, "SteamPath", NULL, &dwType, (LPBYTE)&steamPath, &RegetPath);
                    RegCloseKey(rKey);

                    string appid;
                    string path = getenv("TEMP");
                    string fullpath = path + "/nekroapp.dat";

                    std::ifstream ina(fullpath); // окрываем файл для чтения
                    if (ina.is_open()) {
                        while (getline(ina, appid)) {
                            appid = appid;
                        }

                    } ina.close(); // закрываем файл


                    string cd;
                    string fullpathcd = path + "/nekrotime.dat";

                    std::ifstream incd(fullpathcd); // окрываем файл для чтения
                    if (incd.is_open()) {
                        while (getline(incd, cd)) {
                            cd = cd;
                        }

                    } incd.close(); // закрываем файл


                    
                    string logstr2 = "Starting...\n\tUID: " + to_string(numberone) + " | " + dura[0] + " | " + dura[1] + " | " + appid + " | " + cd + "ms\n";

                    
                    nekrologsec(logstr2);
                    Sleep(stoi(cd));
                    string logstr = "UID: " + to_string(numberone) + " | " + dura[0] + " | " + dura[1] + " launched!" ;
                    nekrologsec(logstr);
 
                    string steamPaths = string(steamPath) + "\\steam.exe";

                    LPCWSTR App = cs2wchar(&steamPaths[0]);
                    char autorun[1024] = " -login ";

                    strcat(autorun, dura[0].c_str());
                    strcat(autorun, " ");
                    strcat(autorun, dura[1].c_str());
                    strcat(autorun, " -silent -applaunch ");
                    strcat(autorun, appid.c_str());
                    strcat(autorun, params.c_str());
       
                    LPWSTR sss = cs2wchar(autorun);

                    STARTUPINFOA strt;
                    PROCESS_INFORMATION proci;
                    for (int i = 0; i < sizeof(strt); i++)
                        ((char*)&strt)[i] = 0;
                    for (int i = 0; i < sizeof(proci); i++)
                        ((char*)&proci)[i] = 0;

                    CreateProcessA(steamPaths.c_str(),
                        autorun,
                        NULL,
                        NULL,
                        FALSE,
                        CREATE_NEW_CONSOLE,
                        NULL,
                        NULL, &
                        strt, &
                        proci);
                        numberone++;
                }

            } in.close(); // закрываем файл

        }
        else if (type == "4") {
            nekrolog(14, "Selected - Open account list");
            if (nekroexists("nekro.dat")) {

                string fullpath = path + "/nekro.dat";

                system(fullpath.c_str());
            }
            else {
                nekrocreate("nekro.dat", "");
                string fullpath = path + "/nekro.dat";
                system(fullpath.c_str());
            }
        }
        else if (type == "5") {
            nekrolog(11, "Enter AppID");
            char pizda[256];
            nekrocin("appid=");
            cin >> pizda;

            nekrocreate("nekroapp.dat", pizda);

            nekrolog(14, "New AppID");
            nekrolog(14, pizda);
        }
        else if (type == "6") {
        nekrolog(11, "Enter Launch Params");
        string pizda;

        nekrocin("launch=");
        getline(cin, pizda);
        nekrocreate("nekroprelaunch.dat", pizda);

        nekrolog(14, "New Launch Params");  
        nekrolog(14, pizda);
        }
        else if (type == "7") {
        nekrolog(11, "Enter Cooldown");
        string pizda;
        nekrocin("cooldown=");
        getline(cin, pizda);
        nekrocreate("nekrotime.dat", pizda);

        nekrolog(14, "New Cooldown");
        nekrolog(14, pizda);
        }
        else if (type == "8") {
            nekrolog(6, "Killing processes");

            system("taskkill /f /IM csgo.exe");
            system("taskkill /f /IM hl2.exe");
            system("taskkill /f /IM gmod.exe");
            system("taskkill /f /IM steam.exe");
            system("taskkill /f /IM steamwebhelper.exe");
        }
    }
}


int main() {
    setlocale(LC_ALL, "Russian");
    std::ofstream out;
    string freakkey;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 240);
    printf("     __       _                \n");
    printf("  /\\ \\ \\ ___ | | __ _ __  ___  \n");
    SetConsoleTextAttribute(hConsole, 159);
    printf(" /  \\/ // _ \\| |/ /| '__|/ _ \\ \n");
    printf("/ /\\  /|  __/|   < | |  | (_) |");
    SetConsoleTextAttribute(hConsole, 15);
    printf("\t -=(Created by 0x040rn && x64cat for BOOST)=-\n");
    SetConsoleTextAttribute(hConsole, 79);
    printf("\\_\\ \\/  \\___||_|\\_\\|_|   \\___/ \n");
    printf("                               \n");
    SetConsoleTextAttribute(hConsole, 15);

    DWORD SerialNumber = getHWID();
    char hwid = SerialNumber;

    string key = "QQ5CjZu3kEbHDjeBBFnR0"; //это короче дохуя твой ключ ебать, через нее будет шифроваться все, но через ассемблер он палится, придется юзать защиту от этого
    string keycrypt = "DLq7lMCcuMIIVD13MRAuLv7H2fdW9f6g";

    string str = to_string(SerialNumber); // ко манда

    string crypting = str;
    string myhwid = encrypt_vigenere(crypting, key); //вот он блять

    string keycrypter = "DLq7lMCcuMIIVD13MRAuLv7H2fdW9f6g";

    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Nekro\\License", 0, KEY_ALL_ACCESS, &hKey);

    string pcname = GetMyUserName();

    printf("[HWID] Your HardwareID - %s:%s\n", pcname.c_str(), myhwid.c_str());

    HKEY rKey;
    char Reget[1024];
    DWORD RegetPath = 1024;
    string crypting2 = key + str;
    RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Nekro\\License", 0, KEY_QUERY_VALUE, &rKey);
    RegQueryValueExA(rKey, "lKey", NULL, NULL, (LPBYTE)Reget, &RegetPath);
    RegCloseKey(rKey);
    if (strcmp(Reget, encrypt_vigenere(crypting2, keycrypt).c_str()) == 0) {
        nekrologsucc("Authorized with registry", "");

        if (!nekroexists("nekroapp.dat")) {

            nekrocreate("nekroapp.dat", "730");

            nekrolog(11, "Created temp/nekroapp.dat - file with AppID");
        }

        if (!nekroexists("nekroprelaunch.dat")) {

            nekrocreate("nekroprelaunch.dat", " -language english -sw -w 640 -h 480");

            nekrolog(11, "Created temp/nekroprelaunch.dat - file with Prelaunch Settings");
        }


        if (!nekroexists("nekrotime.dat")) {

            nekrocreate("nekrotime.dat", "5000");

            nekrolog(11, "Created temp/nekrotime.dat - file with Prelaunch Settings");
        }

        string appid;
        string path = getenv("TEMP");
        string fullpath = path + "/nekroapp.dat";

        std::ifstream in(fullpath); // окрываем файл для чтения
        if (in.is_open()) {
            while (getline(in, appid)) {
                appid = appid;
            }

        } in.close(); // закрываем файл

        string cd;
        string fullpathcd = path + "/nekrotime.dat";

        std::ifstream incd(fullpathcd); // окрываем файл для чтения
        if (incd.is_open()) {
            while (getline(incd, cd)) {
                cd = cd;
            }

        } incd.close(); // закрываем файл

        string lparams;

        string fullpathprelaunch = path + "/nekroprelaunch.dat";

        std::ifstream en(fullpathprelaunch); // окрываем файл для чтения
        if (en.is_open()) {
            while (getline(en, lparams)) {
                lparams = lparams;
            }

        } in.close(); // закрываем файл

        SetConsoleTextAttribute(hConsole, 15);
        printf("[#] Your AppID - ");
        SetConsoleTextAttribute(hConsole, 4);
        printf(appid.c_str());
        SetConsoleTextAttribute(hConsole, 15);
        printf("\n");
        printf("[#] Your CoolDown - ");
        SetConsoleTextAttribute(hConsole, 3);
        printf(cd.c_str());
        SetConsoleTextAttribute(hConsole, 15);
        printf("\n");
        printf("[#] Your Launch Parameters [ ");
        SetConsoleTextAttribute(hConsole, 11);
        printf(lparams.c_str());
        SetConsoleTextAttribute(hConsole, 15);
        printf(" ]\n");

        start();
    }
    else {
        SetConsoleTextAttribute(hConsole, 12);
        printf("[HWID] Enter your key\n");
        SetConsoleTextAttribute(hConsole, 15);
        nekrocin("lKey=");
        getline(cin, freakkey);
        if (checkLicense(freakkey) == true) {
            nekrolog(14, "Authorized");

            if (!nekroexists("nekroapp.dat")) {

                nekrocreate("nekroapp.dat", "730");

                nekrolog(11, "Created temp/nekroapp.dat - file with AppID");
            }

            if (!nekroexists("nekroprelaunch.dat")) {

                nekrocreate("nekroprelaunch.dat", " -language english -sw -w 640 -h 480");

                nekrolog(11, "Created temp/nekroprelaunch.dat - file with Prelaunch Settings");
            }

            string appid;
            string path = getenv("TEMP");
            string fullpath = path + "nekroapp.dat";

            std::ifstream in(fullpath); // окрываем файл для чтения
            if (in.is_open()) {
                while (getline(in, appid)) {
                    appid = appid;
                }

            } in.close(); // закрываем файл

            string lparams;

            string fullpathprelaunch = path + "/nekroprelaunch.dat";

            std::ifstream en(fullpathprelaunch); // окрываем файл для чтения
            if (en.is_open()) {
                while (getline(en, lparams)) {
                    lparams = lparams;
                }

            } in.close(); // закрываем файл

            string cd;
            string fullpathcd = path + "/nekrotime.dat";

            std::ifstream incd(fullpathcd); // окрываем файл для чтения
            if (incd.is_open()) {
                while (getline(incd, cd)) {
                    cd = cd;
                }

            } incd.close(); // закрываем файл

            SetConsoleTextAttribute(hConsole, 15);
            printf("[#] Your AppID - ");
            SetConsoleTextAttribute(hConsole, 4);
            printf(appid.c_str());
            SetConsoleTextAttribute(hConsole, 15);
            printf("\n");
            printf("[#] Your CoolDown - ");
            SetConsoleTextAttribute(hConsole, 3);
            printf(cd.c_str());
            SetConsoleTextAttribute(hConsole, 15);
            printf("\n");
            printf("[#] Your Launch Parameters [ ");
            SetConsoleTextAttribute(hConsole, 11);
            printf(lparams.c_str());
            SetConsoleTextAttribute(hConsole, 15);
            printf(" ]\n");
            start();
        }
    }
    getchar();

}