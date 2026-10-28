// CString::ConcatInPlace(int nSrcLen, const char* lpszSrcData) -- MFC-style in-place append: no-op for an empty source; when the buffer is shared or too small concatenate into a new buffer (ConcatCopy) and release the old one, otherwise copy the text after the current end, bump the length, assert it fits and terminate
// Grade: DIFFER(383v433) -- VC7.1 inliner budget: retail expands 7 of the 10 GetData() calls (the condition's three, the grow path's two and the fast path's ASSERT pair) and calls 0x00C0B8C0 out of line for the fast path's memcpy offset, `+=` and terminator; the same source here expands only the condition and grow-path calls (all five fast-path calls go out of line)
// Leaf: ?ConcatInPlace@CString@@QAEXHPBD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0C270, 433 bytes, thiscall `ret 8`. 1 grade used.
// Identity (probable): libpk MFC-style CString (landed GetData 0x00C0B8C0, ConcatCopy 0x00C0C220, Release 0x00C0B9F0); MFC's
//   CString::ConcatInPlace line for line, with its assertion strings "m_pchData != NULL\n" 0x012B1EC0, "pOldData != NULL\n"
//   0x012B1FC0 and "GetData()->nDataLength <= GetData()->nAllocLength\n" 0x012B1FD4. Catalog label "CCharString_InsertData" is wrong.
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
    static void __stdcall Release(CStringData* pData);   // 0x00C0B9F0
    void AllocBuffer(int nLen);                 // 0x00C0BDA0
    void CopyBeforeWrite();                     // 0x00C0BE20
    void ConcatCopy(int nSrc1Len, const char* lpszSrc1Data, int nSrc2Len, const char* lpszSrc2Data);   // 0x00C0C220
    void ConcatInPlace(int nSrcLen, const char* lpszSrcData);
};

void CString::ConcatInPlace(int nSrcLen, const char* lpszSrcData)
{
    if (nSrcLen == 0)
        return;
    if (GetData()->nRefs > 1 || GetData()->nDataLength + nSrcLen > GetData()->nAllocLength) {
        CStringData* pOldData = GetData();
        ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData);
        LIB_ASSERT(pOldData != NULL)
        CString::Release(pOldData);
    } else {
        memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen * sizeof(char));
        GetData()->nDataLength += nSrcLen;
        LIB_ASSERT(GetData()->nDataLength <= GetData()->nAllocLength)
        m_pchData[GetData()->nDataLength] = 0;
    }
}
