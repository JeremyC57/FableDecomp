// CBasicString<char>::CBasicString(unsigned long count, char fill) -- empty header, fast-extend on, then SetString(count, fill)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A0550, length 59 bytes (+ int3 padding).
// Callees: CBasicString<char>::SetString(unsigned long, char) @ 0x009A0390.
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
    void SetString(unsigned long count, char fill);   // 0x009A0390
    CBasicString_char(unsigned long count, char fill);
};

CBasicString_char::CBasicString_char(unsigned long count, char fill)
    : PData(0), StringLength(0), DataLength(0), UseFastExtend(1)
{
    SetString(count, fill);
}
