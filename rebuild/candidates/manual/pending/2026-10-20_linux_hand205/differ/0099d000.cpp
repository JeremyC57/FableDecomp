// CSystemRegistry::GetString(const CWideString&, const CWideString&) -- read a REG_SZ value, or return the default
// Grade: DIFFER(ours v retail) -- /GS lane gap: our build caches the hidden-result and default pointers in ebx/esi at entry where retail re-reads the argument slots
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099D000, length 177 bytes (+ int3 padding).
// Callees: CWideString::operator const wchar_t* @ 0x0099AF10, CWideString(const wchar_t*) @ 0x0099B6B0,
// CWideString(const CWideString&) @ 0x0099B720, ADVAPI32!RegQueryValueExW through the import slot 0x0143FC00.
//
// Shape: the 512-character buffer makes this a /GS function (cookie 0x0139C8A8, check 0x00BFE9F9 before
// each of the two returns). The result is returned by value through the hidden pointer (`ret 0Ch`: hidden
// result, name, default). Frame order (low to high): type, size (0x200 bytes -- the query is passed the
// byte count of the buffer as retail does), buffer. A closed key or a failed query both copy the default.
// No engine header exists for CSystemRegistry (the landed GetSLONG 0x0099CF80 uses the same model).
struct HKEY__;

class CWideString {
public:
    CWideString(const unsigned short* text);         // 0x0099B6B0
    CWideString(const CWideString& other);           // 0x0099B720
    operator const unsigned short*() const;          // 0x0099AF10
private:
    void* m_pData;
};

extern "C" __declspec(dllimport) long __stdcall RegQueryValueExW(
    HKEY__* hKey, const unsigned short* lpValueName, unsigned long* lpReserved, unsigned long* lpType,
    unsigned char* lpData, unsigned long* lpcbData);                   // [0x0143FC00]

class CSystemRegistry {
public:
    CWideString GetString(const CWideString& valueName, const CWideString& defaultValue) const;
private:
    HKEY__* RootKey;                             // +0x00
    HKEY__* OpenKey;                             // +0x04
};

CWideString CSystemRegistry::GetString(const CWideString& valueName, const CWideString& defaultValue) const
{
    if (OpenKey != 0) {
        unsigned short buffer[512];
        unsigned long type = 0;
        unsigned long size = 0x200;
        if (RegQueryValueExW(OpenKey, valueName, 0, &type, (unsigned char*)buffer, &size) == 0)
            return CWideString(buffer);
    }
    return CWideString(defaultValue);
}
