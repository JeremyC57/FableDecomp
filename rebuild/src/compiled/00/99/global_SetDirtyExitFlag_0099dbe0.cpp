// SetDirtyExitFlag() -- read the "ExitFlag" value of the Fable TLC registry key into a 16-byte buffer and return 0
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099DBE0, 106 bytes, /GS (cookie @ 0x0139C8A8, __security_check_cookie
// @ 0x00BFE9F9). FableWin.pdb `@SetDirtyExitFlag@0`: extern "C" `__fastcall`, no arguments, `xor eax,eax` result.
// Callees (ADVAPI32): RegOpenKeyExA [0x0143FC08], RegQueryValueExA [0x0143FBEC], RegCloseKey [0x0143FBFC].
// Strings: "Software\\Microsoft\\Microsoft Games\\Fable TLC" 0x0129A8E4, "ExitFlag" 0x0129A9C8.
// Shape: HKEY_CURRENT_USER key opened KEY_READ (0x20019) without a result test; the size (16) and the empty-string
// store are scheduled after the argument pushes; the value read is never used -- the retail body only probes the
// key and returns 0 (the 16-byte buffer is the /GS array next to the cookie, the key handle and size below it).
// Flags: /GS
// Leaf: @SetDirtyExitFlag@0
struct HKEY__ { int unused; };
typedef HKEY__* HKEY;

extern "C" __declspec(dllimport) long __stdcall RegOpenKeyExA(HKEY key, const char* subKey, unsigned long options,
                                                              unsigned long access, HKEY* result);      // [0x0143FC08]
extern "C" __declspec(dllimport) long __stdcall RegQueryValueExA(HKEY key, const char* valueName,
    unsigned long* reserved, unsigned long* type, unsigned char* data, unsigned long* dataSize);         // [0x0143FBEC]
extern "C" __declspec(dllimport) long __stdcall RegCloseKey(HKEY key);                                  // [0x0143FBFC]

#define HKEY_CURRENT_USER_ID ((HKEY)(unsigned long)0x80000001)
#define KEY_READ_ACCESS      0x20019

extern "C" int __fastcall SetDirtyExitFlag()
{
    HKEY key;
    unsigned long size;
    char value[16];

    RegOpenKeyExA(HKEY_CURRENT_USER_ID, "Software\\Microsoft\\Microsoft Games\\Fable TLC", 0, KEY_READ_ACCESS, &key);
    size = sizeof(value);
    value[0] = 0;
    RegQueryValueExA(key, "ExitFlag", 0, 0, (unsigned char*)value, &size);
    RegCloseKey(key);
    return 0;
}