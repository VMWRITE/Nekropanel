#define _CRT_SECURE_NO_WARNINGS

#include <Lmcons.h>

#include <iostream>

#include <windows.h>

#include <string>

#include <fstream>

#include <sstream>

#include <io.h>

#include <vector>

#include "ConsoleColor.h"

#include "sha512.h"

using namespace std;

string path = getenv("TEMP");

std::vector < std::string > explode(std::string
    const& s, char delim) {
    std::vector < std::string > result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim);) {
        result.push_back(std::move(token));
    }

    return result;
}

void nekrologsucc(std::string log, std::string secmsg) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout << cyan << "[+] " << grey << log << darkred << secmsg << white << std::endl;
}

void nekrocin(std::string msg) {
    char buffer[UNLEN + 1];
    DWORD size;
    size = sizeof(buffer);
    if (GetUserNameA(buffer, &size) == 0)
        throw "Error GetUserName";
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout << darkgreen << "root@" << buffer << white << ":" << cyan << "~" << white << "$" << grey << msg << white;
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

string GetMyUserNameEx() {
    char buffered[UNLEN + 1];
    DWORD sizerer;
    sizerer = sizeof(buffered);
    if (GetUserNameA(buffered, &sizerer) == 0)
        throw "Error GetUserName";
    return buffered;
}

std::string readfile(std::string file) {
    std::string line;

    string fullpath = path + "/" + file;
    std::ifstream nekrofile(fullpath);
    if (nekrofile.is_open()) {
        while (getline(nekrofile, line)) {
            line = line;
        }
    }
    nekrofile.close();

    return line;
}

std::string readstring(std::string text) {
    std::string cinned;
    nekrocin(text);
    std::getline(std::cin, cinned);
    return cinned;
}

void nekrocreateex(std::string file, std::string content) {
    std::string fullpathers = path + "/" + file;;
    FILE* fileks = fopen(fullpathers.c_str(), "w");
    fputs(content.c_str(), fileks);
    fclose(fileks);
}

void nekrodelex(std::string file) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    remove(file.c_str());
    std::cout << red << "[NEKRO] " << white << "Deleted file - " << file << white << std::endl;
}

bool FileExistsEx(const char* fname) {
    return _access(fname, 0) != -1;
}
bool nekroexistsex(std::string file) {
    std::string fullpather2 = path + "/" + file;
    return FileExistsEx(fullpather2.c_str());
}

string GetRegValue(HKEY where, LPCSTR reg, LPCSTR value) {
    HKEY rKey;
    char Reget[1024];
    DWORD RegetPath = 1024;
    RegOpenKeyExA(HKEY_CURRENT_USER, reg, 0, KEY_QUERY_VALUE, &rKey);
    RegQueryValueExA(rKey, value, NULL, NULL, (LPBYTE)Reget, &RegetPath);
    RegCloseKey(rKey);
    return Reget;
}

void SetRegValue(HKEY where, LPCSTR reg, LPCWSTR key, LPCTSTR regval) {
    HKEY hKey;
    if (RegCreateKeyExA(where, reg, 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL)) {
        MessageBoxA(0, "Can't create RegKey", "RegKey!", MB_OK);
    }

    RegOpenKeyExA(where, reg, 0,
        KEY_ALL_ACCESS, &hKey);
    cout << regval << endl;
    if (RegSetValueEx(hKey, key, 0, REG_SZ, (LPBYTE)regval, 1024) != ERROR_SUCCESS) {
        MessageBoxA(0, "Can't set Reg value", "RegKey!", MB_OK);
    }
}

wchar_t* cs2wchar(const char* charArray) {
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}

string hwid = to_string(getHWID());
string keycrypt = "DLq7lMCcuMIIVD13MRAuLv7H2fdW9f6g";
string keys = "QQ5CjZu3kEbHDjeBBFnR0";

bool checkLicense(string argv) {
    string key = argv.c_str();

    string crypting = keys + hwid;
    if (strcmp(key.c_str(), encrypt_vigenere(crypting, keycrypt).c_str()) == 0) {
        MessageBoxA(0, "Your license has activated!", "[NEKRO]", MB_ICONINFORMATION);
        HKEY hKey;

        LPCTSTR newkey = cs2wchar(encrypt_vigenere(crypting, keycrypt).c_str());
        SetRegValue(HKEY_CURRENT_USER, "SOFTWARE\\Nekro\\License", L"lKey", newkey);
        return true;
    }
    else {
        MessageBoxA(0, "Your code have errors!", "[NEKRO]", MB_ICONHAND);
        return false;
    }
}

void cmdlist() {
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
        cout << blue << "[NEKRO] " << white << str[i] << endl;
}

int nekroclearprefetch() {
    WIN32_FIND_DATAA data;
    HANDLE hFind = FindFirstFileA("C:/Windows/Prefetch/*.pf", &data); // DIRECTORY

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string file = data.cFileName;

            if (file.find("LOAD") != std::string::npos or file.find("DLL") != std::string::npos or file.find("INJECT") != std::string::npos or file.find("CHEAT") != std::string::npos or file.find("HAX") != std::string::npos or file.find("HACK") != std::string::npos) {

                nekrodelex("C:/Windows/Prefetch/" + string(file.c_str()));
                cout << red << "[DELETE] " << white << file << endl;
            }

        } while (FindNextFileA(hFind, &data));
        FindClose(hFind);
    }
}

int getAccounts() {
    if (nekroexistsex("nekro.dat")) {
        cout << "--------------------------ACCOUNT LIST--------------------------" << endl;
        cout << "UID | LOGIN | PASS" << endl;
        string line;
        std::ifstream in(path + "/nekro.dat");
        int accid = 1;
        if (in.is_open()) {
            while (getline(in, line)) {

                auto logpass = explode(line, ':');
                cout << accid << grey << " | " << logpass[0] << " | " << logpass[1] << white << endl;
                accid++;
            }

        } in.close();
        cout << "TOTAL: " <<
            darkred << accid - 1 << "\n" <<
            white << "----------------------------------------------------------------" << endl;
    }
    else {
        nekrocreateex("nekro.dat", "");
        cout << red << "temp/nekro.dat created (Account List)" << white << endl;
    }
}

void createifnotexists(string file, string standarttext) {
    if (!nekroexistsex(file)) {
        nekrocreateex(file, standarttext);
        cout << cyan << "Created temp/" << file << ".dat" << white << endl;
    }
}

string appid = readfile("nekroapp.dat");
string cd = readfile("nekrotime.dat");
string lparams = readfile("nekroprelaunch.dat");

int start() {
    createifnotexists("nekroapp.dat", "730");
    createifnotexists("nekroprelaunch.dat", " -language english -sw -w 640 -h 480");
    createifnotexists("nekrotime.dat", "5000");
    cout << cyan << "AppID=" <<
        white << appid << "\n" <<
        cyan << "Cooldown=" << white << cd << "\n" <<
        cyan << "Launch Parameters=" << white << "\n" <<
        "[ " << lparams << " ]" << "\n" <<
        yellow << "Welcome, " << GetMyUserNameEx() << "\n" <<
        darkred << "Build: " << __DATE__ << "\n" <<
        yellow << "Loading accounts: " << white << endl;
    getAccounts();

    while (1) {

        cmdlist();
        string type = readstring(" ");

        if (type == "1") {
            nekroclearprefetch();
        }
        else if (type == "2") {
            cout << red << "Coming soon" << white << endl;
        }
        else if (type == "3") {

            cout << cyan << "Starting bots" << white << endl;
            string line;
            std::ifstream in(path + "/nekro.dat");

            string params = readfile("nekroprelaunch.dat");
            int numberone = 1;
            if (in.is_open()) {
                cout << "UID | LOGIN | PASSWORD | APPID | COOLDOWN\n" <<
                    "\n---------------------------PARAMETERS---------------------------\n" <<
                    cyan <<
                    params <<
                    white <<
                    "----------------------------------------------------------------\n" <<
                    endl;

                while (getline(in, line)) {
                    auto logpass = explode(line, ':');

                    string steamPath = GetRegValue(HKEY_CURRENT_USER, "SOFTWARE\\Valve\\Steam", "SteamPath") + "\\steam.exe";

                    cout << cyan << "[~] " << grey << "Starting...\n\tUID: " <<
                        numberone <<
                        " | " << logpass[0] <<
                        " | " << logpass[1] <<
                        " | " << appid <<
                        " | " << cd << "ms" << white <<
                        endl;

                    Sleep(stoi(cd));

                    cout << "UID: " <<
                        numberone <<
                        " | " << logpass[0] <<
                        " | " << logpass[1] << " launched!" <<
                        endl;

                    LPCWSTR App = cs2wchar(&steamPath[0]);
                    char autorun[1024] = " -login ";

                    strcat(autorun, logpass[0].c_str());
                    strcat(autorun, " ");
                    strcat(autorun, logpass[1].c_str());
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

                    CreateProcessA(steamPath.c_str(),
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
            if (nekroexistsex("nekro.dat")) {

                string fullpath = path + "/nekro.dat";
                system(fullpath.c_str());
            }
            else {
                nekrocreateex("nekro.dat", "");
                string fullpath = path + "/nekro.dat";
                system(fullpath.c_str());
            }
        }
        else if (type == "5") {
            string appeid = readstring("appid=");

            cout << "New AppID - " <<
                darkred <<
                appeid <<
                white <<
                endl;
            nekrocreateex("nekroapp.dat", appeid);
        }
        else if (type == "6") {
            cout << cyan << "Enter Launch Parameters" << white << endl;
            string launchparams = readstring("launch=");
            nekrocreateex("nekroprelaunch.dat", launchparams);
            cout << "New Launch Parameters - " <<
                darkred <<
                launchparams <<
                white <<
                endl;
        }
        else if (type == "7") {
            cout << cyan << "Enter Cooldown" << white << endl;
            string cooldown = readstring("cooldown=");;

            nekrocreateex("nekrotime.dat", cooldown);

            cout << "New Cooldown - " <<
                darkred <<
                cooldown <<
                white <<
                endl;
        }
        else if (type == "8") {
            cout << red << "Killing processes" << white << endl;
            system("taskkill /f /IM csgo.exe");
            system("taskkill /f /IM hl2.exe");
            system("taskkill /f /IM gmod.exe");
            system("taskkill /f /IM steam.exe");
            system("taskkill /f /IM steamwebhelper.exe");
        }
    }
}

void drawLogo() {
    cout << white << "     __       _                \n  /\\ \\ \\ ___ | | __ _ __  ___  \n" <<
        darkblue << " /  \\/ // _ \\| |/ /| '__|/ _ \\ \n/ /\\  /|  __/|   < | |  | (_) |" <<
        white << "\t -=(Created by 0x040rn && x64cat for BOOST)=-\n" <<
        red << "\\_\\ \\/  \\___||_|\\_\\|_|   \\___/ \n                               " << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");

    drawLogo();
    string hwid = to_string(getHWID());

    string myhwid = encrypt_vigenere(hwid, keys);

    cout << cyan << "[HWID] " << white << "Your HardwareID - " << myhwid << white << endl;

    string crypting2 = keys + hwid;
    string lKey = GetRegValue(HKEY_CURRENT_USER, "SOFTWARE\\Nekro\\License", "lKey");
    if (strcmp(lKey.c_str(), encrypt_vigenere(crypting2, keycrypt).c_str()) == 0) {
        nekrologsucc("Authorized with registry", "");
        start();
    }
    else {
        cout << red << "Enter your key" << white << endl;
        std::ofstream out;
        string freakkey = readstring("lKey=");
        if (checkLicense(freakkey) == true) {
            cout << green << "Authorized" << white << endl;
            start();
        }
    }
    getchar();

}