// CString::GetData() const -- MFC-style CString::GetData out of line: ASSERT(m_pchData != NULL), then the 12-byte CStringData header that sits in front of the characters
// Leaf: GetData
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0B8C0, 42 bytes (`ret`). Label correction: the manifest's
//   "CCharStringData_GetOffset" is propagated; the body is the MFC CString::GetData of the 0x00C0xxxx library
//   (m_pchData points past CStringData {nRefs, nDataLength, nAllocLength}; landed relatives ~CString 0x00C0BA40,
//   AllocBeforeWrite 0x00C0BEE0, GetBuffer 0x00C0C4B0 inline the same body). Callers 0x00C0C300.. 0x00C0CF7D sit in
//   the library's ConcatInPlace / FreeExtra, where /Ob1 left this one call out of line.
// Callees: the assertion hook through the global pointer @ 0x0143BB2C (__fastcall, "m_pchData != NULL\n"
//   @ 0x012B1EC0 in ecx), CDebugBreak::CDebugBreak @ 0x00C16750.
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

class CString {
public:
    char* m_pchData;                            // +0x00
    CStringData* GetData() const;
};

CStringData* CString::GetData() const
{
    LIB_ASSERT(m_pchData != NULL)
    return ((CStringData*)m_pchData) - 1;
}