// CString::SafeStrlen(const char* lpsz) -- length of a C string, 0 for a null pointer (MFC-style string library, PASCAL = __stdcall static)
// Retail: Fable.exe @ 0x00C0BCC0, 21 bytes, `ret 4`; tail-jumps to lstrlenA (IAT 0x0143FD88). Catalog label
// "_Cons_val<pair<CCharString,CCharString>>" is wrong: ~20 callers in the CString library TU (0x00C0A943..0x00C0DD49),
// next to landed CString::ReleaseBuffer 0x00C0C630 which uses the same import.
// Shape: the null arm returns the tested zero register.
extern "C" __declspec(dllimport) int __stdcall lstrlenA(const char* text);   // IAT 0x0143FD88

class CString {
public:
    char* m_pchData;                            // +0x00
    static int __stdcall SafeStrlen(const char* lpsz);
};

int __stdcall CString::SafeStrlen(const char* lpsz)
{
    return (lpsz == 0) ? 0 : lstrlenA(lpsz);
}