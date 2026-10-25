// CRegistryKey::Create(HKEY parent, const char* subKey, unsigned long access) -- open or create a registry subkey into the key's handle; TRUE on ERROR_SUCCESS
// Leaf: ?Create@CRegistryKey
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C737D0, length 46 bytes, ret 0Ch. Callee: ADVAPI32
//   RegCreateKeyExA (IAT 0x0143FBF0) with class "" (0x0122D70E), no options/security/disposition.
// Layout (descriptive; libpk audio, no PDB/RTTI): CRegistryKey -- vtable 0x012BEFB0 (ctor 0x00C73790 nulls the
//   handle), HKEY +4 (Close 0x00C73800 through RegCloseKey). The audio driver's registry probe 0x00C5DE44 /
//   0x00C5DEA3 opens "...\\Drivers..." keys with KEY_ALL_ACCESS (0xF003F) and tests the result as a BOOL.
// Shape: `ERROR_SUCCESS == result` returned as an int gives `neg ; sbb ; inc`; the handle address `this+4`
//   is formed in ecx right after the first push.
typedef struct HKEY__* HKEY;
extern "C" __declspec(dllimport) long __stdcall RegCreateKeyExA(HKEY key, const char* subKey, unsigned long reserved,
    char* className, unsigned long options, unsigned long access, void* security, HKEY* result, unsigned long* disposition);

struct CRegistryKey {
    virtual void Dummy();                  // vtable 0x012BEFB0
    HKEY m_hKey;                           // +0x04
    int Create(HKEY parent, const char* subKey, unsigned long access);
};

int CRegistryKey::Create(HKEY parent, const char* subKey, unsigned long access)
{
    return RegCreateKeyExA(parent, subKey, 0, "", 0, access, 0, &m_hKey, 0) == 0;
}