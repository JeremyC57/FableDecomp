// CSystemRegistry::WriteBOOL(const CWideString&, int) -- store a 4-byte REG_DWORD value under the open key
// Leaf: WriteBOOL
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099D100, length 53 bytes (+ int3 padding).
// Callees: CWideString::operator const wchar_t* @ 0x0099AF10, ADVAPI32!RegSetValueExW through the import
// slot 0x0143FBE8.
// Identity: ?WriteBOOL@CSystemRegistry@@QBEXABVCWideString@@H@Z (three callers at 0x0040179E, 0x004019E4,
// 0x004033F4). Byte-identical to WriteSLONG 0x0099D0C0, which precedes it in source order.
//
// Shape: nothing happens when the key failed to open. The value's address is passed as the data
// pointer: VC7.1 homes the by-value copy in the argument's own (dead) stack slot -- `lea ecx,[esp+10h]`
// is pushed first and the value is re-stored into that slot (`mov [esp+1Ch],eax`) just before the
// string conversion call, so the source copies the argument into a named local. No engine header
// exists for CSystemRegistry: the 8-byte layout {root key, open key} is local.
struct HKEY__;

class CWideString {
public:
    operator const unsigned short*() const;      // 0x0099AF10
private:
    void* m_pData;
};

extern "C" __declspec(dllimport) long __stdcall RegSetValueExW(
    HKEY__* hKey, const unsigned short* lpValueName, unsigned long Reserved, unsigned long dwType,
    const unsigned char* lpData, unsigned long cbData);                // [0x0143FBE8]

class CSystemRegistry {
public:
    void WriteBOOL(const CWideString& valueName, int value) const;
private:
    HKEY__* RootKey;                             // +0x00
    HKEY__* OpenKey;                             // +0x04
};

void CSystemRegistry::WriteBOOL(const CWideString& valueName, int value) const
{
    if (OpenKey != 0) {
        int stored = value;
        RegSetValueExW(OpenKey, valueName, 0, 4, (const unsigned char*)&stored, 4);   // REG_DWORD
    }
}