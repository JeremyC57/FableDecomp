// CString::CString(const CString& stringSrc) -- MFC-style CString copy constructor of the 0x00C0xxxx library: share the source buffer (add a reference) unless it is locked (nRefs < 0), in which case start empty and copy the characters
// Leaf: ??0CString@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0DAB0, length 253 bytes (`ret 4`). The catalog label
// "CShaderResource::CShaderResource::Assign" is propagated: the body is the copy constructor of the library CString of the
// landed AllocBeforeWrite 0x00C0BEE0 / ReleaseBuffer 0x00C0C630 (m_pchData points past a 12-byte CStringData header).
// Callees: the assertion hook through the global pointer @ 0x0143BB2C (`__fastcall`, message in ecx: "m_pchData != NULL\n"
// 0x012B1EC0, "stringSrc.GetData()->nRefs != 0\n" 0x012B2158, "stringSrc.GetData() != _NullString\n" 0x012B1F9C),
// CDebugBreak::CDebugBreak @ 0x00C16750, KERNEL32 InterlockedIncrement (import 0x0143FD34), CString::operator=(const char*)
// @ 0x00C0C200. Globals: _NullString (CStringData*) 0x0139C8FC, the empty string's m_pchData 0x0139C900 (Init()).
// Shape: MFC's CString(const CString&) with every GetData() inlining ASSERT(m_pchData != NULL); the empty CDebugBreak
// temporaries are homed in the dead `stringSrc` argument slot.
#include <stddef.h>
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(long volatile* addend);   // 0x0143FD34

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
extern CStringData* _NullString;                // 0x0139C8FC

class CString {
public:
    char* m_pchData;                            // +0x00
    CString(const CString& stringSrc);
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void Init();
    const CString& operator=(const char* text);   // 0x00C0C200
};
extern CString _EmptyString;                    // 0x0139C900 (its m_pchData)

inline void CString::Init()
{
    m_pchData = _EmptyString.m_pchData;
}

CString::CString(const CString& stringSrc)
{
    LIB_ASSERT(stringSrc.GetData()->nRefs != 0)
    if (stringSrc.GetData()->nRefs >= 0) {
        LIB_ASSERT(stringSrc.GetData() != _NullString)
        m_pchData = stringSrc.m_pchData;
        InterlockedIncrement(&GetData()->nRefs);
    }
    else {
        Init();
        *this = stringSrc.m_pchData;
    }
}