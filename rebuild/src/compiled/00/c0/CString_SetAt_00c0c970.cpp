// CString::SetAt(int nIndex, char ch) -- MFC-style CString::SetAt: assert 0 <= nIndex < length, make the buffer private and store the character
// Leaf: SetAt
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0C970, 120 bytes (`ret 8`). Label correction: the manifest's
//   "CCharString_SetCharAt" names the engine string; the body is MFC's CString::SetAt ("nIndex >= 0\n" 0x012B20BC,
//   "nIndex < GetData()->nDataLength\n" 0x012B2098).
// Callees: CString::CopyBeforeWrite @ 0x00C0BE20, the assertion hook through the global pointer @ 0x0143BB2C,
//   CDebugBreak::CDebugBreak @ 0x00C16750.
// Shape: nIndex lives in edi, so the three CDebugBreak temporaries are homed in its dead argument slot.
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
    void CopyBeforeWrite();                     // 0x00C0BE20
    void SetAt(int nIndex, char ch);
};

void CString::SetAt(int nIndex, char ch)
{
    LIB_ASSERT(nIndex >= 0)
    LIB_ASSERT(nIndex < GetData()->nDataLength)
    CopyBeforeWrite();
    m_pchData[nIndex] = ch;
}