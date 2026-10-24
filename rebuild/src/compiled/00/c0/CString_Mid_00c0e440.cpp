// CString::Mid(int nFirst) const -- MFC-style CString::Mid(int): the tail from nFirst, returned by value through Mid(nFirst, length - nFirst)
// Flags: /EHsc
// Leaf: ?Mid@CString@@QBE?AV1@H@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0E440, 80 bytes (`ret 8`: hidden result + nFirst). Label correction: the
//   manifest's "CCharString_SubstringRange" names the engine string; the body is MFC's
//   `CString CString::Mid(int nFirst) const { return Mid(nFirst, GetData()->nDataLength - nFirst); }` of the
//   0x00C0xxxx library (the two-argument Mid is the landed 0x00C0DF50). Built under /EHsc like its siblings.
// Callees: CString::Mid(int, int) const @ 0x00C0DF50 (result constructed straight into the hidden return slot), the
//   assertion hook through the global pointer @ 0x0143BB2C, CDebugBreak::CDebugBreak @ 0x00C16750.
// Shape: the zeroed frame dword is VC's "return value constructed" flag of a by-value class result.
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
    CString(const CString& stringSrc);          // 0x00C0DAB0
    ~CString();                                 // 0x00C0BA40
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    CString Mid(int nFirst, int nCount) const;  // 0x00C0DF50
    CString Mid(int nFirst) const;
};

CString CString::Mid(int nFirst) const
{
    return Mid(nFirst, GetData()->nDataLength - nFirst);
}