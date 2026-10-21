// CString_00c0e0f0::Mid(long nFirst, long nCount) -- EH-framed MFC-style `CString::Mid` (catalog "CCharString::SubstringToNewString"): negative arguments clamp to 0, the count is clipped to the string's end (0 when nFirst is past it), the two ASSERTs report through the log sink, the whole string is returned as a copy when nFirst is 0 and the count reaches the end, otherwise an empty string receives the slice through AllocCopy and is copied into the hidden result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0DF50, 415 bytes (416 with padding). Built under /EHsc.
// Flags: /EHsc
// Leaf: Mid
// Callees: CString(const CString&) 0x00C0DAB0, ~CString 0x00C0BA40, AllocCopy 0x00C0BF90, the debug-break
//          temporary 0x00C16750, log sink pointer 0x0143BB2C; the empty data 0x0139C900. Literals 0x012B1EC0
//          ("m_pchData != NULL\n"), 0x012B220C ("nFirst >= 0\n"), 0x012B21E0 ("nFirst + nCount <= GetData()->nDataLength\n").
// Model: the landed sibling CString_00c0e0f0::Right (0x00C0E0F0).
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c0e0f0)(const char* text);
extern PFNLOGSINK_00c0e0f0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CTrace_00c0e0f0 {                   // empty temporary (a debug-break helper)
    CTrace_00c0e0f0();                     // 0x00C16750
};

struct CStringData_00c0e0f0 {              // the 12-byte header in front of the characters
    long nRefs;
    long nDataLength;
    long nAllocLength;
};

struct CString_00c0e0f0 {                  // one word: the character data
    char* m_pchData;
    CString_00c0e0f0() : m_pchData(g_pchNil_0139c900) { }
    CString_00c0e0f0(const CString_00c0e0f0& other);   // 0x00C0DAB0
    ~CString_00c0e0f0();                   // 0x00C0BA40
    void AllocCopy(CString_00c0e0f0& dest, long copyLen, long copyIndex, long extraLen) const;   // 0x00C0BF90
    CStringData_00c0e0f0* GetData() const
    {
        if (m_pchData == 0) {              // ASSERT(m_pchData != NULL)
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c("m_pchData != NULL\n");
            CTrace_00c0e0f0();
        }
        return ((CStringData_00c0e0f0*)m_pchData) - 1;
    }
    CString_00c0e0f0 Mid(long nFirst, long nCount) const;
};

CString_00c0e0f0 CString_00c0e0f0::Mid(long nFirst, long nCount) const
{
    if (nFirst < 0)
        nFirst = 0;
    if (nCount < 0)
        nCount = 0;
    if (nFirst + nCount > GetData()->nDataLength)
        nCount = GetData()->nDataLength - nFirst;
    if (nFirst > GetData()->nDataLength)
        nCount = 0;
    if (!(nFirst >= 0)) {
        if (g_pfnLogSink_0143bb2c)
            g_pfnLogSink_0143bb2c("nFirst >= 0\n");
        CTrace_00c0e0f0();
    }
    if (!(nFirst + nCount <= GetData()->nDataLength)) {
        if (g_pfnLogSink_0143bb2c)
            g_pfnLogSink_0143bb2c("nFirst + nCount <= GetData()->nDataLength\n");
        CTrace_00c0e0f0();
    }
    if (nFirst == 0 && nFirst + nCount == GetData()->nDataLength)
        return *this;
    {
        CString_00c0e0f0 dest;
        AllocCopy(dest, nCount, nFirst, 0);
        return dest;
    }
}
