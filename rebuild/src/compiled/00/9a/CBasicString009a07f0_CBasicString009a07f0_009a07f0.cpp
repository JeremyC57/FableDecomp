// CBasicString_009a07f0::CBasicString_009a07f0(const char* text, unsigned long length) -- the CBasicString<char> constructor inlined, then the derived class's own flag set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A07F0, length 66 bytes.
// Callees: CBasicString<char>::AssignFromData(const char*, unsigned long) @ 0x009A0300.
// Shape: byte-identical to the base constructor @ 0x009A05E0 up to the call, then
// `mov dword ptr [esi+0Dh],1`: a derived class (packed, the base is 13 bytes) whose single long
// member at +0x0d is set to 1 in its body. The derived class cannot be named from the body; it is
// named by its address. The PDB base layout: PData +0, StringLength +4, DataLength:31 +8 (bit 31
// is a second untouched bitfield), UseFastExtend:1 +0xc.
#pragma pack(push, 1)
struct CBasicString_char {
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength    : 31;           // +0x08 bits 0..30
    unsigned long m_reserved    : 1;            // +0x08 bit 31 (not initialised here)
    unsigned char UseFastExtend : 1;            // +0x0c bit 0
    void AssignFromData(const char* text, unsigned long length);   // 0x009A0300
    CBasicString_char(const char* text, unsigned long length)
        : PData(0), StringLength(0), DataLength(0), UseFastExtend(1)
    {
        AssignFromData(text, length);
    }
};

struct CBasicString_009a07f0 : CBasicString_char {
    long m_mode;                                // +0x0d (unaligned: the base is 13 bytes)
    CBasicString_009a07f0(const char* text, unsigned long length);
};
#pragma pack(pop)

CBasicString_009a07f0::CBasicString_009a07f0(const char* text, unsigned long length)
    : CBasicString_char(text, length)
{
    m_mode = 1;
}