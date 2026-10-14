// CBasicString<char>::CBasicString(const char* text, unsigned long length) -- empty header, fast-extend on, then AssignFromData(text, length)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A05E0, length 59 bytes (+ int3 padding).
// Callees: CBasicString<char>::AssignFromData(const char*, unsigned long) @ 0x009A0300.
// Layout (Ego_r.pdb CBasicString<char>, 13 bytes): PData +0, StringLength +4, DataLength:31 at +8
// (the top bit of that dword is a second, untouched bitfield: retail keeps it with
// `and eax,80000000h`), UseFastExtend:1 at +0xc (retail `or dl,1`). The two bitfield writes are
// read/modify/write of their storage units; PData and StringLength take immediate zero stores.
struct CBasicString_char {
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength    : 31;           // +0x08 bits 0..30
    unsigned long m_reserved    : 1;            // +0x08 bit 31 (not initialised here)
    unsigned char UseFastExtend : 1;            // +0x0c bit 0
    void AssignFromData(const char* text, unsigned long length);   // 0x009A0300
    CBasicString_char(const char* text, unsigned long length);
};

CBasicString_char::CBasicString_char(const char* text, unsigned long length)
    : PData(0), StringLength(0), DataLength(0), UseFastExtend(1)
{
    AssignFromData(text, length);
}
