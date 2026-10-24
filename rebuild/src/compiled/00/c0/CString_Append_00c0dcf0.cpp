// CString::Append(const CString& string) -- MFC-style append of another CString in place: ConcatInPlace with the source's length (through its asserted GetData()) and text; returns nothing
// Leaf: Append
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0DCF0, 55 bytes (`ret 4`, no `mov eax,esi`: a void member). Descriptive name
//   (no PDB covers this library): the manifest's "CCharString_InsertDataWrapper" is propagated. The only caller is
//   0x00C0DEA0 (`push edx ; mov esi,ecx ; call 0x00C0DCF0 ; mov eax,esi`), i.e. `const CString& operator+=(const
//   CString&)` built as { Append(s); return *this; } under the LTCG register convention.
// Callees: CString::ConcatInPlace(int, const char*) @ 0x00C0C270, the assertion hook through the global pointer
//   @ 0x0143BB2C, CDebugBreak::CDebugBreak @ 0x00C16750.
// Shape: the source reference lives in edi, so the CDebugBreak temporary is homed in its dead argument slot; the
//   text is pushed before the length is loaded (right-to-left argument order).
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
    void ConcatInPlace(int nSrcLen, const char* lpszSrcData);   // 0x00C0C270
    void Append(const CString& string);
};

void CString::Append(const CString& string)
{
    ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
}