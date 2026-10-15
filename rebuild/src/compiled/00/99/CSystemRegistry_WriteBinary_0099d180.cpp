// CSystemRegistry::WriteBinary(const CWideString&, void*, unsigned long) -- store a REG_BINARY block under the open key
// Leaf: WriteBinary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099D180, length 48 bytes.
// Callees: CWideString::operator const wchar_t* @ 0x0099AF10, ADVAPI32!RegSetValueExW through the import
// slot 0x0143FBE8.
// Identity: ?WriteBinary@CSystemRegistry@@QBEXABVCWideString@@PAXK@Z (value type 3 = REG_BINARY).
//
// Shape: nothing happens when the key failed to open; the size and data arguments are pushed straight
// from their stack slots, the name conversion is evaluated last (right-to-left argument order) and the
// open key is re-read after it. No engine header exists for CSystemRegistry.
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
    void WriteBinary(const CWideString& valueName, void* data, unsigned long size) const;
private:
    HKEY__* RootKey;                             // +0x00
    HKEY__* OpenKey;                             // +0x04
};

void CSystemRegistry::WriteBinary(const CWideString& valueName, void* data, unsigned long size) const
{
    if (OpenKey != 0)
        RegSetValueExW(OpenKey, valueName, 0, 3, (const unsigned char*)data, size);   // REG_BINARY
}