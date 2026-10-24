// CString::AssignCopy(int nSrcLen, const char* lpszSrcData) -- MFC-style CString::AssignCopy: make the buffer private and large enough, copy the characters, store the length and the terminator
// Leaf: AssignCopy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0C060, 86 bytes (`ret 8`). Label correction: the manifest's
//   "CCharString_SetData" is propagated; the body is MFC's CString::AssignCopy (callers 0x00C0C172 / 0x00C0C211 are the
//   operator= bodies, the landed CString::operator=(const CString&) 0x00C0C0C0 calls it as AssignCopy).
// Callees: CString::AllocBeforeWrite @ 0x00C0BEE0, the assertion hook through the global pointer @ 0x0143BB2C,
//   CDebugBreak::CDebugBreak @ 0x00C16750; memcpy is the intrinsic (rep movsd ; rep movsb).
// Shape: nSrcLen lives in ebp, so the GetData() temporary is homed in its dead argument slot (`lea ecx,[esp+14h]`).
#include <stddef.h>
extern "C" void* __cdecl memcpy(void* dst, const void* src, size_t n);
#pragma intrinsic(memcpy)

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

class CString {
public:
    char* m_pchData;                            // +0x00
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void AllocBeforeWrite(int nLen);            // 0x00C0BEE0
    void AssignCopy(int nSrcLen, const char* lpszSrcData);
};

void CString::AssignCopy(int nSrcLen, const char* lpszSrcData)
{
    AllocBeforeWrite(nSrcLen);
    memcpy(m_pchData, lpszSrcData, nSrcLen * sizeof(char));
    GetData()->nDataLength = nSrcLen;
    m_pchData[nSrcLen] = '\0';
}