// CSystemRegistry::WriteSLONG(const CWideString&, long) -- store a 4-byte REG_DWORD value under the open key
// Leaf: WriteSLONG
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099D0C0, length 53 bytes (+ int3 padding).
// Callees: CWideString::operator const wchar_t* @ 0x0099AF10, ADVAPI32!RegSetValueExW through the import
// slot 0x0143FBE8.
// Identity: the manifest calls both 0x0099D0C0 and 0x0099D100 "WriteBOOL"; the two bodies are identical
// (REG_DWORD, 4 bytes). The class's source order (other build's symbol table: GetSLONG, GetBOOL, GetString,
// WriteSLONG, WriteBOOL, WriteString, WriteBinary) matches the retail order here (GetSLONG 0x0099CF80,
// GetBOOL 0x0099CFE0, GetString 0x0099D000, then this, WriteBOOL 0x0099D100, WriteString 0x0099D140,
// WriteBinary 0x0099D180), so this first DWORD writer is WriteSLONG(const CWideString&, long)
// (?WriteSLONG@CSystemRegistry@@QBEXABVCWideString@@J@Z); it has no direct caller in the image.
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
    void WriteSLONG(const CWideString& valueName, long value) const;
private:
    HKEY__* RootKey;                             // +0x00
    HKEY__* OpenKey;                             // +0x04
};

void CSystemRegistry::WriteSLONG(const CWideString& valueName, long value) const
{
    if (OpenKey != 0) {
        long stored = value;
        RegSetValueExW(OpenKey, valueName, 0, 4, (const unsigned char*)&stored, 4);   // REG_DWORD
    }
}
