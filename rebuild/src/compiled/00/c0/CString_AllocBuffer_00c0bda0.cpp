// CString::AllocBuffer(int nLen) -- MFC-style CString::AllocBuffer: assert 0 <= nLen <= INT_MAX-1; an empty length points the string at the shared empty text, otherwise a fresh header + nLen+1 characters is allocated with one reference, both lengths nLen and a terminator
// Leaf: AllocBuffer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0BDA0, 123 bytes (`ret 4`). Label correction: the manifest's
//   "CCharString_AllocFromSize" is propagated; the body is MFC's CString::AllocBuffer (the release-build arm without
//   the CFixedAlloc pools; "nLen >= 0\n" 0x012B1F78, "nLen <= INT_MAX-1\n" 0x012B1F64). The landed AllocBeforeWrite
//   0x00C0BEE0, ConcatCopy 0x00C0C220 and GetBuffer 0x00C0C4B0 call it as AllocBuffer.
// Callees: the assertion hook through the global pointer @ 0x0143BB2C, CDebugBreak::CDebugBreak @ 0x00C16750,
//   MSVCR71 scalar `operator new` (thunk 0x00BFEA1A, not new[]).
// Globals: _afxPchNil @ 0x0139C900 (Init()).
// Shape: nLen in esi, so both CDebugBreak temporaries are homed in its dead argument slot; the four header stores
//   keep MFC's order (nAllocLength, nRefs, terminator, nDataLength).
#include <stddef.h>
#include <limits.h>

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
extern char* _afxPchNil;                        // 0x0139C900

class CString {
public:
    char* m_pchData;                            // +0x00
    void Init() { m_pchData = _afxPchNil; }
    void AllocBuffer(int nLen);
};

void CString::AllocBuffer(int nLen)
{
    LIB_ASSERT(nLen >= 0)
    LIB_ASSERT(nLen <= INT_MAX-1)
    if (nLen == 0)
        Init();
    else {
        CStringData* pData = (CStringData*)operator new(sizeof(CStringData) + (nLen + 1) * sizeof(char));
        pData->nAllocLength = nLen;
        pData->nRefs = 1;
        pData->data()[nLen] = '\0';
        pData->nDataLength = nLen;
        m_pchData = pData->data();
    }
}