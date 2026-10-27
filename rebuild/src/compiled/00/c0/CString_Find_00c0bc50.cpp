// CString::Find(const char* lpszSub, int nStart) const -- MFC-style CString::Find: the index of the first lpszSub at or after nStart (_mbsstr, DBCS aware), -1 when nStart is past the end or lpszSub is absent
// Leaf: ?Find@CString@@QBEHPBDH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0BC50, 112 bytes, thiscall, `ret 8`.
// Identity (probable): the MFC CString::Find(LPCTSTR, int) text over the 0x00C0xxxx MFC-style CString:
//   ASSERT(IsValidString(lpszSub)) ("IsValidString(lpszSub)\n" @ 0x012B1F34; the libpk IsValidString is an inline null
//   test), then GetData() inlined with its ASSERT(m_pchData != NULL) (@ 0x012B1EC0), nDataLength (-8) against nStart
//   (`jg`: a start equal to the length is allowed), then _tcsstr over MSVCR71!_mbsstr (IAT 0x0143FFDC).
// Shape: both CDebugBreak temporaries land on the dead lpszSub argument slot (`lea ecx,[esp+0Ch]`); lpszSub lives in edi.
#include <stddef.h>

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

extern "C" __declspec(dllimport) unsigned char* __cdecl _mbsstr(const unsigned char* string, const unsigned char* sub);   // IAT 0x0143FFDC

// tchar.h (_MBCS, no _MB_MAP_DIRECT): _tcsstr is an inline wrapper over _mbsstr
static __inline char* __cdecl _tcsstr(const char* s, const char* sub)
{
    return (char*)_mbsstr((const unsigned char*)s, (const unsigned char*)sub);
}

static inline bool IsValidString(const char* lpsz) { return lpsz != NULL; }   // libpk: a null test only

class CString {
public:
    char* m_pchData;                            // +0x00
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    int Find(const char* lpszSub, int nStart) const;
};

int CString::Find(const char* lpszSub, int nStart) const
{
    LIB_ASSERT(IsValidString(lpszSub))

    int nLength = GetData()->nDataLength;
    if (nStart > nLength)
        return -1;

    char* lpsz = _tcsstr(m_pchData + nStart, lpszSub);
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}