// CString::Insert(int nIndex, char ch) -- MFC-style CString::Insert of one character: copy-before-write, clamp the index into [0, length], grow the buffer when it is full (copying the old text and releasing the old block), shift the tail up one (forward rep movs, as retail), store the character and the new length
// Leaf: ?Insert@CString@@QAEHHD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0CB50, 278 bytes, thiscall `ret 8`, returns the new length.
// Identity (probable): the libpk MFC-style CString (landed GetData 0x00C0B8C0, Release 0x00C0B9F0, AllocBuffer 0x00C0BDA0,
//   CopyBeforeWrite 0x00C0BE20, ConcatCopy 0x00C0C220); MFC's CString::Insert(int, TCHAR) body line for line (CopyBeforeWrite,
//   the nIndex clamps, AllocBuffer + memcpy + Release when nAllocLength < nNewLength, the move, the store). The catalog label
//   "CCharString_InsertAt" is propagated. No PDB covers libpk.
// Callees: CopyBeforeWrite 0x00C0BE20, AllocBuffer 0x00C0BDA0, Release(CStringData*) 0x00C0B9F0 (__stdcall), the assertion hook
//   through 0x0143BB2C ("m_pchData != NULL\n" 0x012B1EC0) and CDebugBreak::CDebugBreak 0x00C16750; memcpy intrinsic (also for
//   the overlapping tail move: retail copies forward with rep movsd/movsb).
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
    int Insert(int nIndex, char ch);
};

int CString::Insert(int nIndex, char ch)
{
    CopyBeforeWrite();
    if (nIndex < 0)
        nIndex = 0;
    int nNewLength = GetData()->nDataLength;
    if (nIndex > nNewLength)
        nIndex = nNewLength;
    nNewLength++;
    if (GetData()->nAllocLength < nNewLength) {
        CStringData* pOldData = GetData();
        char* pstr = m_pchData;
        AllocBuffer(nNewLength);
        memcpy(m_pchData, pstr, (pOldData->nDataLength + 1) * sizeof(char));
        CString::Release(pOldData);
    }
    memcpy(m_pchData + nIndex + 1, m_pchData + nIndex, (nNewLength - nIndex) * sizeof(char));
    m_pchData[nIndex] = ch;
    GetData()->nDataLength = nNewLength;
    return nNewLength;
}