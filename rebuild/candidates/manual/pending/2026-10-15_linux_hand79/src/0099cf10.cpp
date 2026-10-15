// CSystemRegistry::CSystemRegistry(const CWideString&, long) -- pick the root hive, then create/open the sub key
// Leaf: ??0CSystemRegistry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099CF10, length 96 bytes (+ int3 padding).
// Callees: CWideString::operator const wchar_t* @ 0x0099AF10 (manifest "CCharString::operator const char*"
// is the byte-identical narrow twin), ADVAPI32!RegCreateKeyExW through the import slot 0x0143FBF8.
// Mangled name (other build's symbol table): ??0CSystemRegistry@@QAE@ABVCWideString@@J@Z.
//
// Shape: a `switch` on the root selector -- `sub eax,0 ; je` / `sub eax,2 ; je` -- with case 0 =
// HKEY_CURRENT_USER, case 2 = HKEY_CLASSES_ROOT and the default HKEY_LOCAL_MACHINE (the default arm
// is laid out first). The open-key address is formed once (`lea edi,[esi+4]`) and reused for the
// failure reset; RegCreateKeyExW gets KEY_ALL_ACCESS (0x2001F) and no class/options/security/
// disposition. The string conversion is evaluated last (right-to-left argument order). No engine
// header exists for CSystemRegistry: the 8-byte layout {root key, open key} is local.
struct HKEY__;

class CWideString {
public:
    operator const unsigned short*() const;      // 0x0099AF10
private:
    void* m_pData;
};

extern "C" __declspec(dllimport) long __stdcall RegCreateKeyExW(
    HKEY__* hKey, const unsigned short* lpSubKey, unsigned long Reserved, unsigned short* lpClass,
    unsigned long dwOptions, unsigned long samDesired, void* lpSecurityAttributes,
    HKEY__** phkResult, unsigned long* lpdwDisposition);              // [0x0143FBF8]

class CSystemRegistry {
public:
    CSystemRegistry(const CWideString& subKey, long rootSelection);
private:
    HKEY__* RootKey;                             // +0x00
    HKEY__* OpenKey;                             // +0x04
};

CSystemRegistry::CSystemRegistry(const CWideString& subKey, long rootSelection)
{
    switch (rootSelection) {
    case 0:
        RootKey = (HKEY__*)0x80000001;           // HKEY_CURRENT_USER
        break;
    case 2:
        RootKey = (HKEY__*)0x80000000;           // HKEY_CLASSES_ROOT
        break;
    default:
        RootKey = (HKEY__*)0x80000002;           // HKEY_LOCAL_MACHINE
        break;
    }
    if (RegCreateKeyExW(RootKey, subKey, 0, 0, 0, 0x2001F, 0, &OpenKey, 0) != 0)
        OpenKey = 0;
}
