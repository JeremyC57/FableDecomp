// CSystemRegistry::WriteString(const CWideString&, const CWideString&) -- store a REG_SZ value under the open key
// Leaf: WriteString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099D140, length 61 bytes (+ int3 padding).
// Callees: CWideString::GetLength @ 0x0099B220 and CWideString::operator const wchar_t* @ 0x0099AF10
// (manifest labels are the byte-identical CCharString twins), ADVAPI32!RegSetValueExW through the
// import slot 0x0143FBE8.
// Identity: the manifest calls this "WriteBinary", but the value type is 1 (REG_SZ) and the data is a
// second CWideString: this is ?WriteString@CSystemRegistry@@QBEXABVCWideString@@0@Z (WriteBinary is
// 0x0099D180, type 3).
//
// Shape: the value string is held in edi across its two calls; arguments are evaluated right to left, so
// the byte count (`GetLength() + 1`, as retail passes it -- characters, not bytes) is computed before the
// data pointer, and the name conversion comes last. No engine header exists for CSystemRegistry.
struct HKEY__;

class CWideString {
public:
    long GetLength() const;                      // 0x0099B220
    operator const unsigned short*() const;      // 0x0099AF10
private:
    void* m_pData;
};

extern "C" __declspec(dllimport) long __stdcall RegSetValueExW(
    HKEY__* hKey, const unsigned short* lpValueName, unsigned long Reserved, unsigned long dwType,
    const unsigned char* lpData, unsigned long cbData);                // [0x0143FBE8]

class CSystemRegistry {
public:
    void WriteString(const CWideString& valueName, const CWideString& value) const;
private:
    HKEY__* RootKey;                             // +0x00
    HKEY__* OpenKey;                             // +0x04
};

void CSystemRegistry::WriteString(const CWideString& valueName, const CWideString& value) const
{
    if (OpenKey != 0)
        RegSetValueExW(OpenKey, valueName, 0, 1,                         // REG_SZ
                       (const unsigned char*)(const unsigned short*)value, value.GetLength() + 1);
}
