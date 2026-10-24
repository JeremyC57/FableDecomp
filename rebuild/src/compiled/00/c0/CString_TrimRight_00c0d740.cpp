// CString::TrimRight(const char* lpszTargetList) -- MFC-style CString::TrimRight(LPCTSTR): unshare, then cut the string at the start of its trailing run of characters found in the target list (DBCS-aware walk)
// Leaf: ?TrimRight@CString@@QAEXPBD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0D740, 128 bytes (`ret 4`). Label correction: the manifest's
//   "CCharString_TrimRight" names the engine string; the body is MFC's CString::TrimRight(LPCTSTR), the sibling of the
//   landed TrimRight(TCHAR) 0x00C0D7C0.
// Callees: CString::CopyBeforeWrite @ 0x00C0BE20, MSVCR71!_mbschr (IAT 0x0144001C; the character sign-extended:
//   `movsx eax,al`), MSVCR71!_mbsinc (IAT 0x0143FFE4), the assertion hook through the global pointer @ 0x0143BB2C,
//   CDebugBreak::CDebugBreak @ 0x00C16750.
// Shape: the MFC loop -- remember the first match of the current run, forget it on any other character, step with
//   _tcsinc; on exit the terminator register (al == 0) is stored at the run start and the length updated through
//   GetData() (its CDebugBreak temporary homed in the dead lpszTargetList slot). The calls go through tchar.h's _MBCS
//   inline wrappers _tcschr / _tcsinc: calling the imports directly keeps _mbsinc in ebp and reloads the target list
//   every pass; through the wrappers the list stays in ebp and both imports are called through memory, as in retail.
#include <stddef.h>

extern "C" __declspec(dllimport) unsigned char* __cdecl _mbschr(const unsigned char* string, unsigned int c);   // IAT 0x0144001C
extern "C" __declspec(dllimport) unsigned char* __cdecl _mbsinc(const unsigned char* current);                   // IAT 0x0143FFE4

typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

struct CDebugBreak {                            // empty temporary whose constructor calls DebugBreak
    CDebugBreak();                              // 0x00C16750
};

#define LIB_ASSERT(cond) \
    if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak(); }

struct CStringData {                            // the 12-byte header in front of the characters
    long nRefs;
    long nDataLength;
    long nAllocLength;
    char* data() { return (char*)(this + 1); }
};

// tchar.h (_MBCS, no _MB_MAP_DIRECT): _tcschr / _tcsinc are inline wrappers over the MBCS routines
static __inline char* __cdecl _tcschr(const char* s, unsigned int c)
{
    return (char*)_mbschr((const unsigned char*)s, c);
}
static __inline char* __cdecl _tcsinc(const char* p)
{
    return (char*)_mbsinc((const unsigned char*)p);
}

class CString {
public:
    char* m_pchData;                            // +0x00
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void CopyBeforeWrite();                     // 0x00C0BE20
    void TrimRight(const char* lpszTargetList);
};

void CString::TrimRight(const char* lpszTargetList)
{
    CopyBeforeWrite();
    char* lpsz = m_pchData;
    char* lpszLast = NULL;
    while (*lpsz != '\0') {
        if (_tcschr(lpszTargetList, *lpsz) != NULL) {
            if (lpszLast == NULL)
                lpszLast = lpsz;
        } else {
            lpszLast = NULL;
        }
        lpsz = _tcsinc(lpsz);
    }
    if (lpszLast != NULL) {
        *lpszLast = '\0';
        GetData()->nDataLength = (long)(lpszLast - m_pchData);
    }
}