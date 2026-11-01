// CString::Mid(int nFirst, int nCount) -- EH-framed MFC-style `CString::Mid` (catalog "CCharString::SubstringToNewString"): negative arguments clamp to 0, the count is clipped to the string's end (0 when nFirst is past it), the two ASSERTs report through the log sink, the whole string is returned as a copy when nFirst is 0 and the count reaches the end, otherwise an empty string receives the slice through AllocCopy and is copied into the hidden result
// Leaf: ?Mid@CString@@QBE?AV1@HH@Z
// Naming (naming wave 23, DECISIONS_W22_RESOLVED item 3): on the library CString (was the stand-in CString_00c0e0f0); int counts as the MFC interface and the bound 0x00C0E440 spelling.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0DF50, 415 bytes (416 with padding). Built under /EHsc.
// Flags: /EHsc
// Callees: CString(const CString&) 0x00C0DAB0, ~CString 0x00C0BA40, AllocCopy 0x00C0BF90, CDebugBreak::CDebugBreak
//          0x00C16750, log sink pointer 0x0143BB2C; the empty data 0x0139C900. Literals 0x012B1EC0
//          ("m_pchData != NULL\n"), 0x012B220C ("nFirst >= 0\n"), 0x012B21E0 ("nFirst + nCount <= GetData()->nDataLength\n").
// Model: the landed sibling CString::Right (0x00C0E0F0).
#include <stddef.h>
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

struct CDebugBreak {                            // empty temporary whose constructor calls DebugBreak
    CDebugBreak();                              // 0x00C16750
};

struct CStringData {                            // the 12-byte header in front of the characters
    long nRefs;
    long nDataLength;
    long nAllocLength;
};

#define LIB_ASSERT(cond) \
    if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak(); }

class CString {
public:
    char* m_pchData;                            // +0x00
    CString() { Init(); }
    CString(const CString& stringSrc);          // 0x00C0DAB0
    ~CString();                                 // 0x00C0BA40
    void Init();
    void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;   // 0x00C0BF90
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    CString Mid(int nFirst, int nCount) const;
};
extern CString _EmptyString;                    // 0x0139C900 (its m_pchData)

inline void CString::Init()
{
    m_pchData = _EmptyString.m_pchData;
}

CString CString::Mid(int nFirst, int nCount) const
{
    if (nFirst < 0)
        nFirst = 0;
    if (nCount < 0)
        nCount = 0;
    if (nFirst + nCount > GetData()->nDataLength)
        nCount = GetData()->nDataLength - nFirst;
    if (nFirst > GetData()->nDataLength)
        nCount = 0;
    LIB_ASSERT(nFirst >= 0)
    LIB_ASSERT(nFirst + nCount <= GetData()->nDataLength)
    if (nFirst == 0 && nFirst + nCount == GetData()->nDataLength)
        return *this;
    {
        CString dest;
        AllocCopy(dest, nCount, nFirst, 0);
        return dest;
    }
}