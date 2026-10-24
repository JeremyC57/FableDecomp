// CString::CopyBeforeWrite() -- MFC-style CString::CopyBeforeWrite: when the buffer is shared, release it and copy the text (with its terminator) into a private buffer of the same length; then assert the buffer is unshared
// Leaf: CopyBeforeWrite
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0BE20, 187 bytes (`ret`). Label correction: the manifest's
//   "RefcountedData_Copy" is propagated; the body is MFC's CString::CopyBeforeWrite ("GetData()->nRefs <= 1\n"
//   0x012B1F84); the landed ReleaseBuffer 0x00C0C630 and TrimRight(char) 0x00C0D7C0 call it as CopyBeforeWrite.
// Callees: CString::Release() @ 0x00C0B910, CString::AllocBuffer @ 0x00C0BDA0, the assertion hook through the global
//   pointer @ 0x0143BB2C, CDebugBreak::CDebugBreak @ 0x00C16750; memcpy is the intrinsic (rep movsd ; rep movsb).
// Shape: four inlined GetData() calls (ASSERT(m_pchData != NULL)); the empty CDebugBreak temporaries sit in the
//   `push ecx` frame word (`lea ecx,[esp+7]`, the last one `[esp+3]` after ebx is popped).
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
    void Release();                             // 0x00C0B910
    void AllocBuffer(int nLen);                 // 0x00C0BDA0
    void CopyBeforeWrite();
};

void CString::CopyBeforeWrite()
{
    if (GetData()->nRefs > 1) {
        CStringData* pData = GetData();
        Release();
        AllocBuffer(pData->nDataLength);
        memcpy(m_pchData, pData->data(), (pData->nDataLength + 1) * sizeof(char));
    }
    LIB_ASSERT(GetData()->nRefs <= 1)
}