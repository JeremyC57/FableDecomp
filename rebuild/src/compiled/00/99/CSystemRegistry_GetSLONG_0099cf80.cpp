// CSystemRegistry::GetSLONG(const CWideString&, long) -- read a REG_DWORD value, or return the default
// Leaf: GetSLONG
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099CF80, length 86 bytes (+ int3 padding).
// Callees: CWideString::GetAsCharArray() @ 0x0099AF10, ADVAPI32!RegQueryValueExW through the import
// slot 0x0143FC00.
// Identity: the manifest calls this "GetBOOL", but GetBOOL is the next function (0x0099CFE0), which calls
// this one and returns `result != 0` as an int; this is GetSLONG (other build's symbol table:
// ?GetSLONG@CSystemRegistry@@QBEJABVCWideString@@J@Z).
//
// Shape: the zero register serves the open-key test (`xor eax,eax ; cmp ecx,eax`), the reserved argument
// and the two zero-initialised locals; the size local starts at 4. Frame order (low to high): data,
// size, type. The value read is returned only when the query succeeds; a closed key or a failed query
// both reach the shared `return defaultValue` tail. No engine header exists for CSystemRegistry.
struct HKEY__;

class CWideString {
public:
    const unsigned short* GetAsCharArray() const;      // 0x0099AF10
private:
    void* m_pData;
};

extern "C" __declspec(dllimport) long __stdcall RegQueryValueExW(
    HKEY__* hKey, const unsigned short* lpValueName, unsigned long* lpReserved, unsigned long* lpType,
    unsigned char* lpData, unsigned long* lpcbData);                   // [0x0143FC00]

class CSystemRegistry {
public:
    long GetSLONG(const CWideString& valueName, long defaultValue) const;
private:
    HKEY__* RootKey;                             // +0x00
    HKEY__* OpenKey;                             // +0x04
};

long CSystemRegistry::GetSLONG(const CWideString& valueName, long defaultValue) const
{
    if (OpenKey != 0) {
        unsigned long type = 0;
        long value = 0;
        unsigned long size = 4;
        if (RegQueryValueExW(OpenKey, valueName.GetAsCharArray(), 0, &type, (unsigned char*)&value, &size) == 0)
            return value;
    }
    return defaultValue;
}