// CString::Compare(const char* lpsz) const -- MFC-style compare of the string with a C string: assert the argument, then _stricmp when the library's global case-insensitive flag is set, the strcmp intrinsic otherwise
// Leaf: Compare
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0BCE0, length 120 bytes.
// Label correction: the manifest's "CDefString::operator==(const char*)" is propagated; the body belongs to the MFC-style
// CString of the 0x00C0xxxx library (m_pchData at +0, as the landed 0x00C0D7C0 TrimRight) and returns the raw comparison
// (no `sete`): CString::Compare.
// Callees: the assertion hook through the global pointer @ 0x0143BB2C ("IsValidString(lpsz)\n" @ 0x012B1F4C),
// CDebugBreak::CDebugBreak @ 0x00C16750, MSVCR71!_stricmp through the import slot 0x014400EC; the case flag is the byte
// @ 0x0143BB30.
// Shape: the library's LIB_ASSERT (the empty CDebugBreak temporary homed in the dead lpsz argument slot, the pointer
// itself kept in esi); m_pchData is loaded before the flag branch; the two-bytes-per-iteration inline strcmp.
#include <stddef.h>

extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)
extern "C" __declspec(dllimport) int __cdecl _stricmp(const char* a, const char* b);   // IAT 0x014400EC (MSVCR71)

typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C
extern bool g_bCaseInsensitiveStrings;          // 0x0143BB30

struct CDebugBreak {                            // empty temporary whose constructor calls DebugBreak
    CDebugBreak();                              // 0x00C16750
};

#define LIB_ASSERT(cond) \
    if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak(); }

static inline bool IsValidString(const char* lpsz) { return lpsz != NULL; }

class CString {
public:
    char* m_pchData;                            // +0x00
    int Compare(const char* lpsz) const;
};

int CString::Compare(const char* lpsz) const
{
    LIB_ASSERT(IsValidString(lpsz))
    if (g_bCaseInsensitiveStrings)
        return _stricmp(m_pchData, lpsz);
    return strcmp(m_pchData, lpsz);
}
