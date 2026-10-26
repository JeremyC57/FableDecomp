// CDataOutputStream::WriteDateAndTime(const CDateAndTime&) -- serialise a date stamp: the year as a long, month/day/hour/minute as single bytes, second and millisecond as 16-bit words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994180, length 176 bytes (ret 4).
// Callees: virtual slot 4 Write(const void*, long) on `this` (seven times); no direct calls.
// Layout: CDateAndTime (PDB, 0x1C): Year long +0, Month/Day/Hour/Minute/Second/Millisecond unsigned long +4..+0x18.
// Shape: every field goes through an inlined typed writer whose by-value parameter VC homes in the dead argument
// slot ([esp+0Ch] at entry). The byte writers truncate (`mov cl,[edi+N]` then a byte store); the word writers
// zero-extend through ecx (`xor ecx,ecx ; mov cx,[edi+N]`) and store the full dword, so their parameter is
// widened back to an int before it is addressed (the 2-byte Write reads its low half).
class CDateAndTime { public:
    long          Year;          // +0x00
    unsigned long Month;         // +0x04
    unsigned long Day;           // +0x08
    unsigned long Hour;          // +0x0c
    unsigned long Minute;        // +0x10
    unsigned long Second;        // +0x14
    unsigned long Millisecond;   // +0x18
};

class CDataOutputStream { public:                     // engine/CDataOutputStream.h: vptr +0, NewLine +4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);   // slot 4
    void* NewLine_PStringData;                 // +0x04
    void WriteLong(long value) { Write(&value, 4); }
    void WriteUByte(unsigned char value) { Write(&value, 1); }
    void WriteUWord(unsigned short value) { Write(&value, 2); }
    void WriteDateAndTime(const CDateAndTime& date);
};

void CDataOutputStream::WriteDateAndTime(const CDateAndTime& date)
{
    WriteLong(date.Year);
    WriteUByte((unsigned char)date.Month);
    WriteUByte((unsigned char)date.Day);
    WriteUByte((unsigned char)date.Hour);
    WriteUByte((unsigned char)date.Minute);
    WriteUWord((unsigned short)date.Second);
    WriteUWord((unsigned short)date.Millisecond);
}