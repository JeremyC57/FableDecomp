// CCharString::SetDataLength(int) -- MFC-style CString::AllocBeforeWrite: when the buffer is shared or too small, release it and allocate a private one of the length; then assert it is unshared
// Leaf: AllocBeforeWrite
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0BEE0, 176 bytes incl. padding (`ret 4`). The manifest's class/name are propagated:
//          the body belongs to the MFC-style CString of the 0x00C0xxxx library (m_pchData points past a 12-byte
//          CStringData header {nRefs, nDataLength, nAllocLength}), the class of the landed ReleaseBuffer 0x00C0C630.
// Callees: CString::Release @ 0x00C0B910, CString::AllocBuffer @ 0x00C0BDA0, the assertion hook through the global
//          pointer @ 0x0143BB2C (__fastcall, message in ecx: "m_pchData != NULL\n" 0x012B1EC0, "GetData()->nRefs <= 1\n"
//          0x012B1F84), CDebugBreak::CDebugBreak @ 0x00C16750.
// Shape: each GetData() inlines ASSERT(m_pchData != NULL); the empty CDebugBreak temporaries are homed in the
//          `push ecx` frame word and in the dead nLen argument slot (nLen lives in edi).
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

class CString {
public:
    char* m_pchData;                            // +0x00
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void Release();                             // 0x00C0B910
    void AllocBuffer(int nLen);                 // 0x00C0BDA0
    void AllocBeforeWrite(int nLen);
};

void CString::AllocBeforeWrite(int nLen)
{
    if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength) {
        Release();
        AllocBuffer(nLen);
    }
    LIB_ASSERT(GetData()->nRefs <= 1)
}