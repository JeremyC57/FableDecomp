// CDataOutputStream::WriteNullTerminatedString(const CCharString&) -- write the string's characters plus its terminating zero through the virtual Write(const void*, long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995AE0, length 48 bytes.
// Callees: virtual slot 4 Write(const void*, long) on `this`; the shared empty string at 0x0129AAF4 (CCharString's null-data text).
// Shape: the text and the length come from ONE null test of the data block: the empty-string pointer is assigned
// first (preloaded into edx), the non-null arm overwrites both, the null arm only zeroes the length (`xor eax,eax`)
// and jumps back into the push sequence; two separate accessors would test twice. Then the empty-string pointer is preloaded
// into edx and the null arm (`xor eax,eax`) duplicates the call sequence; `inc eax` is the `+ 1` for the terminator.
class CCharStringData { public:
    char* text;                                // +0x00
    long  length;                              // +0x04
};
extern char g_EmptyCharString_0129AAF4[1];    // 0x0129AAF4

class CCharString { public:
    CCharStringData* m_pStringData;            // +0x00 CCharString::PStringData
    // null case written first: the data load is the taken branch (the landed CAnimFilterOfGroup accessors)
    const char* GetText() const { return m_pStringData == 0 ? g_EmptyCharString_0129AAF4 : m_pStringData->text; }
    long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->length; }
};

class CDataOutputStream { public:                     // engine/CDataOutputStream.h: vptr +0, NewLine +4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);   // slot 4
    CCharString NewLine;                       // +0x04
    void WriteNullTerminatedString(const CCharString& text);
};

void CDataOutputStream::WriteNullTerminatedString(const CCharString& text)
{
    const char* characters = g_EmptyCharString_0129AAF4;
    long length;
    if (text.m_pStringData) {
        characters = text.m_pStringData->text;
        length = text.m_pStringData->length;
    } else {
        length = 0;
    }
    Write(characters, length + 1);
}