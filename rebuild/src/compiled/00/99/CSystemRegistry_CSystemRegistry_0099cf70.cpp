// CSystemRegistry::~CSystemRegistry() -- closes the open registry key handle, if any
// Retail: Fable.exe 0x0099CF70, 15 bytes: `mov eax,[ecx+4] ; test ; je ; push eax ; call [__imp__RegCloseKey@4] ; ret`.
// Identity: catalog name; Ego_r.pdb ??1CSystemRegistry@@QAE@XZ (non-virtual: no vtable store). PDB layout (8 bytes):
// hRoot +0x00, hKey +0x04. The predefined root key is never closed, only the opened subkey.
// Callees: RegCloseKey (advapi32 import thunk slot 0x0143FBFC).
// Leaf: ??1CSystemRegistry@@QAE@XZ
struct HKEY__ { int unused; };
typedef HKEY__* HKEY;
extern "C" __declspec(dllimport) long __stdcall RegCloseKey(HKEY hKey);   // [0x0143FBFC]

class CSystemRegistry {
public:
    ~CSystemRegistry();
    HKEY hRoot;     // +0x00
    HKEY hKey;      // +0x04
};

CSystemRegistry::~CSystemRegistry()
{
    if (hKey)
        RegCloseKey(hKey);
}