// CCharString::RemoveAllOccurrences(char) -- MFC-style CString::TrimRight(TCHAR): unshare, then cut the string at the first character of its trailing run of chTarget (DBCS-aware walk)
// Leaf: TrimRight
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0D7C0, 108 bytes. The body belongs to the MFC-style CString of the 0x00C0xxxx
// library (m_pchData points past the 12-byte CStringData header), not the engine CCharString the manifest names.
// Callees: CString::CopyBeforeWrite @ 0x00C0BE20, _mbsinc (MSVCR71, IAT 0x0143FFE4, held in ebp across the loop),
//          the assertion hook through the global pointer @ 0x0143BB2C, CDebugBreak::CDebugBreak @ 0x00C16750.
// Shape: the MFC 7 TrimRight(TCHAR) loop -- remember the first match of the current run, forget it on any other
// character, step with _tcsinc; on exit the terminator register (cl == 0) is stored at the run start and the length
// updated through GetData() (inlined ASSERT(m_pchData != NULL); its empty CDebugBreak temporary is homed in the dead
// chTarget argument slot).
#include <stddef.h>

extern "C" __declspec(dllimport) unsigned char* __cdecl _mbsinc(const unsigned char* current);   // IAT 0x0143FFE4 (MSVCR71)
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dest, const void* src, size_t count); // IAT 0x01440144 (MSVCR71)

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
};

class CString {
public:
    char* m_pchData;                            // +0x00
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void CopyBeforeWrite();                     // 0x00C0BE20
    void TrimRight(char chTarget);
    void TrimLeft(char chTarget);
};

void CString::TrimRight(char chTarget)
{
    CopyBeforeWrite();
    char* lpsz = m_pchData;
    char* lpszLast = NULL;
    while (*lpsz != '\0') {
        if (*lpsz == chTarget) {
            if (lpszLast == NULL)
                lpszLast = lpsz;
        } else {
            lpszLast = NULL;
        }
        lpsz = (char*)_mbsinc((const unsigned char*)lpsz);
    }
    if (lpszLast != NULL) {
        *lpszLast = '\0';
        GetData()->nDataLength = (long)(lpszLast - m_pchData);
    }
}