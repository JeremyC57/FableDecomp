// ClearDirtyExitFlag() -- write ExitFlag = "clean" under the Fable TLC registry key
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099DC50, 86 bytes. FableWin.pdb `@ClearDirtyExitFlag@0`: extern "C"
// `__fastcall`, no arguments, no result (the `push ecx` is the key-handle slot).
// Callees: RegCreateKeyExA [0x0143FBF0], RegSetValueExA [0x0143FBF4], RegCloseKey [0x0143FBFC] (ADVAPI32),
// lstrlenA [0x0143FD88] (KERNEL32).
// Strings: "Software\\Microsoft\\Microsoft Games\\Fable TLC" 0x0129A8E4, "ExitFlag" 0x0129A9C8, "clean" 0x0129A9D4.
// Shape: HKEY_CURRENT_USER key created/opened with KEY_WRITE (0x20006), no class, options or security, no
// disposition; the REG_SZ (1) value is written with lstrlenA("clean") + 1 bytes (the terminator included).
// Leaf: @ClearDirtyExitFlag@0
struct HKEY__ { int unused; };
typedef HKEY__* HKEY;
struct SECURITY_ATTRIBUTES;

extern "C" __declspec(dllimport) long __stdcall RegCreateKeyExA(HKEY key, const char* subKey, unsigned long reserved,
    char* keyClass, unsigned long options, unsigned long access, SECURITY_ATTRIBUTES* security, HKEY* result,
    unsigned long* disposition);                                                                         // [0x0143FBF0]
extern "C" __declspec(dllimport) long __stdcall RegSetValueExA(HKEY key, const char* valueName, unsigned long reserved,
    unsigned long type, const unsigned char* data, unsigned long dataSize);                              // [0x0143FBF4]
extern "C" __declspec(dllimport) long __stdcall RegCloseKey(HKEY key);                                  // [0x0143FBFC]
extern "C" __declspec(dllimport) int __stdcall lstrlenA(const char* text);                              // [0x0143FD88]

#define HKEY_CURRENT_USER_ID ((HKEY)(unsigned long)0x80000001)
#define KEY_WRITE_ACCESS     0x20006
#define REG_SZ_TYPE          1

extern "C" void __fastcall ClearDirtyExitFlag()
{
    HKEY key;
    RegCreateKeyExA(HKEY_CURRENT_USER_ID, "Software\\Microsoft\\Microsoft Games\\Fable TLC", 0, 0, 0, KEY_WRITE_ACCESS,
                    0, &key, 0);
    RegSetValueExA(key, "ExitFlag", 0, REG_SZ_TYPE, (const unsigned char*)"clean", lstrlenA("clean") + 1);
    RegCloseKey(key);
}