// CString::Find(char ch, int nStart) const -- MFC-style CString::Find: the index of the first ch at or after nStart (_mbschr, DBCS aware), -1 when nStart is past the end or ch is absent
// Leaf: ?Find@CString@@QBEHDH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0BB70, 82 bytes, thiscall, `ret 8`.
// Identity (probable): the MFC CString::Find(TCHAR, int) text over the 0x00C0xxxx MFC-style CString (landed relatives
//   ReverseFind 0x00C0BC20, GetData 0x00C0B8C0, Append 0x00C0DD40): GetData() inlined with its
//   ASSERT(m_pchData != NULL) ("m_pchData != NULL\n" @ 0x012B1EC0 through the hook @ 0x0143BB2C, then the CDebugBreak
//   temporary 0x00C16750), nDataLength (-8) against nStart (`jge`), then _tcschr over MSVCR71!_mbschr (IAT 0x0144001C).
// Shape: the empty CDebugBreak temporary lands on the dead high byte of the `push ecx` frame word (`lea ecx,[esp+7]`).
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

extern "C" __declspec(dllimport) unsigned char* __cdecl _mbschr(const unsigned char* string, unsigned int c);   // IAT 0x0144001C

// tchar.h (_MBCS, no _MB_MAP_DIRECT): _tcschr is an inline wrapper over _mbschr
static __inline char* __cdecl _tcschr(const char* s, unsigned int c)
{
    return (char*)_mbschr((const unsigned char*)s, c);
}

class CString {
public:
    char* m_pchData;                            // +0x00
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    int Find(char ch, int nStart) const;
};

int CString::Find(char ch, int nStart) const
{
    int nLength = GetData()->nDataLength;
    if (nStart >= nLength)
        return -1;

    char* lpsz = _tcschr(m_pchData + nStart, (unsigned char)ch);
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}