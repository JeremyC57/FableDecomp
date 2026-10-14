// CMemoryDataInputStream::CMemoryDataInputStream(const CCharString& text) -- the CBaseClassNonCopyable base (0x0099A2F0), the stream state zeroed (+4..+0x14), this class's vtable (0x0123675C), then the source set from the string: its chars (the empty literal 0x0129AAF4 for a null string) at +0xc / +0x18, the position (+4) and flags (+0x10) cleared, its length at +8 / +0x14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00995F50, length 96 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Shape: the zeroing state is a second, non-polymorphic base; the two accessors are inlined with their own null tests (length first: the arguments of the inline setter are evaluated right to left), `ret 4`.
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};
struct CCharStringData {
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];       // 0x0129AAF4: ""
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};

struct SMemoryStreamState_00995f50 {              // a second, non-polymorphic base
    long                 m_position;               // +0x04
    long                 m_size;                   // +0x08
    const char*          m_pData;                  // +0x0c
    long                 m_flags;                  // +0x10
    long                 m_sizeCopy;               // +0x14
    SMemoryStreamState_00995f50() : m_position(0), m_size(0), m_pData(0), m_flags(0), m_sizeCopy(0) { }
};

struct CMemoryDataInputStream : CBaseClassNonCopyable, SMemoryStreamState_00995f50 {   // vtable 0x0123675C
    const char* m_pDataCopy;                       // +0x18
    CMemoryDataInputStream(const CCharString& text);
    __forceinline void SetSource(const char* data, unsigned long length)
    {
        m_pData = data;
        m_pDataCopy = data;
        m_position = 0;
        m_flags = 0;
        m_size = length;
        m_sizeCopy = length;
    }
};

CMemoryDataInputStream::CMemoryDataInputStream(const CCharString& text)
    : CBaseClassNonCopyable(), SMemoryStreamState_00995f50()
{
    SetSource(text.GetChars(), text.GetLength());
}
