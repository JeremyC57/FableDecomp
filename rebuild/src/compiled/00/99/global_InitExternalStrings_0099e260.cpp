// InitExternalStrings() -- load strings.dll (a message box and ExitProcess(1) if it is missing), read the language id from HKLM\Software\Microsoft\Microsoft Games\Fable\1.0 "LangID" (default 0x409, SUBLANG_DEFAULT added when the value has no sub-language), fetch the exception-dialog, EULA and error strings with built-in English fallbacks and record the start tick count
// Leaf: @InitExternalStrings@0
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099E260, 502 bytes (incl. int3 padding). FableWin.pdb `@InitExternalStrings@0`
//   (extern "C" __fastcall, no arguments). The catalog's "InitExternalStrings" agrees.
// Callees: LoadLibraryA, GetCurrentDirectoryA, MessageBoxA, ExitProcess, RegOpenKeyExA, RegQueryValueExA, RegCloseKey,
//   GetTickCount (IAT), RetrieveString 0x0099D470 (FableWin.pdb `@RetrieveString@16`: module, string id, buffer, size;
//   0 on failure), __security_check_cookie 0x00BFE9F9 (/GS: the 260-byte message buffer).
// Globals: strings module 0x013BD6EC, language id (WORD) 0x0138E060, exception title [0x100] 0x013BD1C8, exception
//   text [0x100] 0x013BD0C8, EULA file name [0x20] 0x013BD2C8, missing-strings error [0x400] 0x013BD2E8, start tick
//   count 0x013BCE38.
#include <string.h>
#pragma intrinsic(strcpy, strcat)

struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
struct HKEY__;
typedef HKEY__* HKEY;
#define HKEY_LOCAL_MACHINE ((HKEY)(unsigned long)0x80000002)
#define KEY_READ 0x20019
#define REG_DWORD 4
#define ERROR_SUCCESS 0
#define MAX_PATH 260

extern "C" __declspec(dllimport) HINSTANCE __stdcall LoadLibraryA(const char* fileName);
extern "C" __declspec(dllimport) unsigned long __stdcall GetCurrentDirectoryA(unsigned long length, char* buffer);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(HWND__* owner, const char* text, const char* caption, unsigned int type);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int exitCode);
extern "C" __declspec(dllimport) long __stdcall RegOpenKeyExA(HKEY key, const char* subKey, unsigned long options,
                                                              unsigned long desired, HKEY* result);
extern "C" __declspec(dllimport) long __stdcall RegQueryValueExA(HKEY key, const char* valueName, unsigned long* reserved,
                                                                 unsigned long* type, unsigned char* data, unsigned long* size);
extern "C" __declspec(dllimport) long __stdcall RegCloseKey(HKEY key);
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount();
extern "C" int __fastcall RetrieveString(HINSTANCE module, unsigned int id, char* buffer, int bufferSize);   // 0x0099D470

HINSTANCE g_hStringsDll;                       // 0x013BD6EC
unsigned short g_LanguageID;                   // 0x0138E060
char g_ExceptionTitle[0x100];                  // 0x013BD1C8
char g_ExceptionText[0x100];                   // 0x013BD0C8
char g_EulaFileName[0x20];                     // 0x013BD2C8
char g_InvalidStringsDllText[0x400];           // 0x013BD2E8
unsigned long g_StartTickCount;                // 0x013BCE38

extern "C" void __fastcall InitExternalStrings()
{
    g_hStringsDll = LoadLibraryA("strings.dll");
    if (g_hStringsDll == 0) {
        char message[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, message);
        strcat(message, "\\strings.dll is missing.");
        MessageBoxA(0, message, "Error!", 0);
        ExitProcess(1);
    }
    HKEY key;
    unsigned long type;
    unsigned long langID;
    unsigned long size;
    g_LanguageID = 0x409;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Microsoft Games\\Fable\\1.0", 0, KEY_READ, &key) == ERROR_SUCCESS) {
        type = REG_DWORD;
        size = sizeof(langID);
        if (RegQueryValueExA(key, "LangID", 0, &type, (unsigned char*)&langID, &size) == ERROR_SUCCESS)
            g_LanguageID = (unsigned short)(((langID & 0xFC00) == 0) ? (langID | 0x400) : langID);   // SUBLANG_DEFAULT
        RegCloseKey(key);
    }
    if (!RetrieveString(g_hStringsDll, 0xDB, g_ExceptionTitle, sizeof(g_ExceptionTitle)))
        strcpy(g_ExceptionTitle, "Exception!");
    if (!RetrieveString(g_hStringsDll, 0xDC, g_ExceptionText, sizeof(g_ExceptionText)))
        strcpy(g_ExceptionText, "Gathering Exception Data...");
    if (!RetrieveString(g_hStringsDll, 0xE8, g_EulaFileName, sizeof(g_EulaFileName)))
        strcpy(g_EulaFileName, "eula.rtf");
    if (!RetrieveString(g_hStringsDll, 0xEC, g_InvalidStringsDllText, sizeof(g_InvalidStringsDllText)))
        strcpy(g_InvalidStringsDllText, "Invalid / missing strings.dll");
    g_StartTickCount = GetTickCount();
}