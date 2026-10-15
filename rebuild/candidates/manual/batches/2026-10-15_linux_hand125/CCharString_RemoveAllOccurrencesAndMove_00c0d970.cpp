// CCharString::RemoveAllOccurrencesAndMove(char) -- MFC-style CString::TrimLeft(TCHAR): unshare, skip the leading run of chTarget (DBCS-aware) and move the rest (with its terminator) to the front
// Leaf: TrimLeft
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0D970, 152 bytes. The body belongs to the MFC-style CString of the 0x00C0xxxx
// library (m_pchData points past the 12-byte CStringData header), not the engine CCharString the manifest names.
// Callees: CString::CopyBeforeWrite @ 0x00C0BE20, _mbsinc (MSVCR71, IAT 0x0143FFE4, held in edi across the loop),
//          memmove (MSVCR71, IAT 0x01440144), the assertion hook through the global pointer @ 0x0143BB2C,
//          CDebugBreak::CDebugBreak @ 0x00C16750.
// Shape: the MFC 7 TrimLeft(TCHAR) body -- `chTarget == *lpsz` keeps the argument on the left of the compare; the new
// length (old length minus the skipped prefix) is computed through one inlined GetData() before memmove and stored
// through a second one after it (each with ASSERT(m_pchData != NULL); the empty CDebugBreak temporary is homed in the
// dead chTarget argument slot).
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

void CString::TrimLeft(char chTarget)
{
    CopyBeforeWrite();
    const char* lpsz = m_pchData;
    while (chTarget == *lpsz)
        lpsz = (const char*)_mbsinc((const unsigned char*)lpsz);
    if (lpsz != m_pchData) {
        int nDataLength = GetData()->nDataLength - (int)(lpsz - m_pchData);
        memmove(m_pchData, lpsz, (nDataLength + 1) * sizeof(char));
        GetData()->nDataLength = nDataLength;
    }
}
