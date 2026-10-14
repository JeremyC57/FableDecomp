// CBasicString_009a07a0::CBasicString_009a07a0(unsigned long count, char fill) -- a packed string record: keep only the static-buffer bit of the capacity word, set the owned bit, null the data and length, fill `count` characters, reference count 1 at the odd offset +0xd
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A07A0, length 64 bytes.
// Callees: CBasicString<char>::SetString(unsigned long, char) @ 0x009A0390 (on `this`). Shape: the capacity word and the flag byte are read before the
// argument pushes and written back around the two null stores (`and eax,80000000h`, `or dl,1`), the reference count is an immediate dword store at +0xd
// (a `#pragma pack(1)` record); returns `this`, `ret 8`. The chunk label "CEngineInternalPrimitiveChanger::CEngineInternalPrimitiveChanger" is propagated
// (the body is the packed-string constructor); the class is named by address.
#pragma pack(push, 1)
struct CBasicString_009a07a0 {
    char*         m_pData;                         // +0x00
    unsigned long m_length;                        // +0x04
    unsigned long m_capacity;                      // +0x08: bit 31 = static buffer
    unsigned char m_flags;                         // +0x0c: bit 0 = owned
    long          m_references;                    // +0x0d
    void SetString(unsigned long count, char fill);   // 0x009A0390
    CBasicString_009a07a0(unsigned long count, char fill);
};
#pragma pack(pop)

CBasicString_009a07a0::CBasicString_009a07a0(unsigned long count, char fill)
{
    m_capacity &= 0x80000000;
    m_pData = 0;
    m_length = 0;
    m_flags |= 1;
    SetString(count, fill);
    m_references = 1;
}
