// Grade: DIFFER(210v208) -- dest CString is homed in the dead count argument slot in retail ([esp+20h], push ecx frame); ours builds it in a sub esp,8 frame | tried: Right-sibling model | next: lane gap: dead-argument-slot temporary
// CString::Left(long count) -- MFC-style CString::Left: clamp a negative count to 0; a count at or past the length returns a copy of *this, else AllocCopy(dest, count, 0, 0) into an empty string returned by value
// Leaf: Left
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0E1F0, 208 bytes incl. padding (ret 8). Manifest name CCharString::LeftSubstring is propagated; sibling of the landed Right @ 0x00C0E0F0 (same model).
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK)(const char* text);
extern PFNLOGSINK g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CTrace {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace();                     // 0x00C16750
};

struct CStringData {              // the 12-byte header in front of the characters
    long nRefs;
    long nDataLength;
    long nAllocLength;
};

struct CString {                  // one word: the character data
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    CString(const CString& other);   // 0x00C0DAB0
    ~CString();                   // 0x00C0BA40
    void AllocCopy(CString& dest, long copyLen, long copyIndex, long extraLen) const;   // 0x00C0BF90
    CStringData* GetData() const
    {
        if (m_pchData == 0) {              // the inlined null-object check
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c("Null object pointer");
            CTrace();
        }
        return ((CStringData*)m_pchData) - 1;
    }
    CString Left(long count) const;
};

CString CString::Left(long count) const
{
    if (count < 0)
        count = 0;
    if (count >= GetData()->nDataLength)
        return *this;
    CString dest;
    AllocCopy(dest, count, 0, 0);
    return dest;
}
