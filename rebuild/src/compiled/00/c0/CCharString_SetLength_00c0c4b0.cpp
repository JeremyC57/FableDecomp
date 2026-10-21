// CCharString::SetLength(int) -- MFC-style CString::GetBuffer: assert the minimum length is not negative; when the buffer is shared or too small, allocate a private one (at least the old length), copy the old text and its terminator, keep the old length and release the old block; then assert it is unshared and return the characters
// Leaf: GetBuffer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0C4B0, 378 bytes (384 with padding). `ret 4`.
// Label correction: catalogued as the engine "CCharString::SetLength"; the body belongs to the MFC-style CString
//   of the 0x00C0xxxx library (m_pchData points past a 12-byte CStringData header {nRefs, nDataLength,
//   nAllocLength}), the class of the landed ReleaseBuffer 0x00C0C630 and AllocBeforeWrite 0x00C0BEE0.
// Callees: CString::AllocBuffer 0x00C0BDA0, static CString::Release(CStringData*) 0x00C0B9F0 (`__stdcall`,
//          `ret 4`), the assertion hook through the global pointer 0x0143BB2C (__fastcall, the message in ecx:
//          "nMinBufLength >= 0\n" 0x012B2008, "m_pchData != NULL\n" 0x012B1EC0, "GetData()->nRefs <= 1\n"
//          0x012B1F84), CDebugBreak::CDebugBreak 0x00C16750; the memcpy intrinsic (`rep movsd ; rep movsb`).
//
// Shape: the MFC 7 GetBuffer text: every GetData() inlines ASSERT(m_pchData != NULL); the old block pointer lives
// in the frame across AllocBuffer, the old length in ebp; the empty CDebugBreak temporaries are homed in the
// dead nMinBufLength argument slot (the value lives in edi) and in a frame byte.
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
    void AllocBuffer(int nLen);                 // 0x00C0BDA0
    static void __stdcall Release(CStringData* pData);   // 0x00C0B9F0
    char* GetBuffer(int nMinBufLength);
};

char* CString::GetBuffer(int nMinBufLength)
{
    LIB_ASSERT(nMinBufLength >= 0)
    if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength) {
        CStringData* pOldData = GetData();
        int nOldLen = GetData()->nDataLength;
        if (nMinBufLength < nOldLen)
            nMinBufLength = nOldLen;
        AllocBuffer(nMinBufLength);
        memcpy(m_pchData, pOldData->data(), (nOldLen + 1) * sizeof(char));
        GetData()->nDataLength = nOldLen;
        CString::Release(pOldData);
    }
    LIB_ASSERT(GetData()->nRefs <= 1)
    LIB_ASSERT(m_pchData != NULL)
    return m_pchData;
}