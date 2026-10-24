// CString::CompareNoCase(const char* lpsz) const -- MFC-style CString::CompareNoCase: ASSERT the argument, then _stricmp of the text against it
// Leaf: CompareNoCase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0BD60, 53 bytes (`ret 4`). Label correction: the manifest's
//   "MessageEvent_Validate" is propagated; the body is MFC's `ASSERT(AfxIsValidString(lpsz)); return _tcsicmp(m_pchData, lpsz);`
//   ("IsValidString(lpsz)\n" @ 0x012B1F4C, the same assert as the landed CString::Compare 0x00C0BCE0, whose
//   case-insensitive arm calls the same import).
// Callees: the assertion hook through the global pointer @ 0x0143BB2C, CDebugBreak::CDebugBreak @ 0x00C16750,
//   MSVCR71!_stricmp through the import slot 0x014400EC.
// Shape: lpsz is kept in edi, so the empty CDebugBreak temporary is homed in its dead argument slot.
#include <stddef.h>

extern "C" __declspec(dllimport) int __cdecl _stricmp(const char* a, const char* b);   // IAT 0x014400EC (MSVCR71)

typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

struct CDebugBreak {                            // empty temporary whose constructor calls DebugBreak
    CDebugBreak();                              // 0x00C16750
};

#define LIB_ASSERT(cond) \
    if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak(); }

static inline bool IsValidString(const char* lpsz) { return lpsz != NULL; }

class CString {
public:
    char* m_pchData;                            // +0x00
    int CompareNoCase(const char* lpsz) const;
};

int CString::CompareNoCase(const char* lpsz) const
{
    LIB_ASSERT(IsValidString(lpsz))
    return _stricmp(m_pchData, lpsz);
}