// CSystemRegistry::DeleteValue(const CWideString&) -- delete a named value under the open key
// Leaf: DeleteValue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099D1E0, length 27 bytes.
// Callees: CWideString::GetAsCharArray() @ 0x0099AF10, ADVAPI32!RegDeleteValueW through the import
// slot 0x0143FC04.
//
// Shape: unlike the writers there is no open-key test; the name conversion runs first (it is the last
// argument evaluated right to left that needs a call) and the key is read after it. The result of
// RegDeleteValueW is dropped. No engine header exists for CSystemRegistry.
struct HKEY__;

class CWideString {
public:
    const unsigned short* GetAsCharArray() const;      // 0x0099AF10
private:
    void* m_pData;
};

extern "C" __declspec(dllimport) long __stdcall RegDeleteValueW(
    HKEY__* hKey, const unsigned short* lpValueName);                  // [0x0143FC04]

class CSystemRegistry {
public:
    void DeleteValue(const CWideString& valueName) const;
private:
    HKEY__* RootKey;                             // +0x00
    HKEY__* OpenKey;                             // +0x04
};

void CSystemRegistry::DeleteValue(const CWideString& valueName) const
{
    RegDeleteValueW(OpenKey, valueName.GetAsCharArray());
}