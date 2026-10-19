// CDataOutputStream::WriteDateAndTime(const CDateAndTime*) -- write year (4 bytes), month/day/hour/minute (1 byte each), second/millisecond (2 bytes each)
// Grade: DIFFER(159v163) -- retail widens the two 16-bit fields with `xor ecx,ecx ; mov cx,[edi+N]` and stores after the pushes; the lane emits `movzx ecx,word ptr` (ushort and long helper locals, /G5, /G7, optimize s tried)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994180, length 163 bytes (+ int3 padding), `ret 4`.
// Callees: virtual slot 4 Write(const void*, long) on `this`, seven times.
// Shape: seven inlined typed writers (WriteLong / WriteUBYTE / WriteUWORD), each copying its narrowed value into a
// local that VC homes in the dead argument slot and passing its address with the byte count. The two 16-bit fields
// are read with `xor ecx,ecx ; mov cx,[edi+N]` and stored as a whole dword: the 2-byte writer's local is a long.
// Layout: CDateAndTime from the PDB (Year long, then six unsigned longs); CDataOutputStream per
// engine/CDataOutputStream.h (vptr +0, NewLine string +4) with the virtual Write slot made explicit.
struct CDateAndTime {
    long Year;                  // +0x00
    unsigned long Month;        // +0x04
    unsigned long Day;          // +0x08
    unsigned long Hour;         // +0x0c
    unsigned long Minute;       // +0x10
    unsigned long Second;       // +0x14
    unsigned long Millisecond;  // +0x18
};

struct CDataOutputStream {                     // engine/CDataOutputStream.h: vptr +0, NewLine +4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);   // slot 4
    void* NewLine_PStringData;                 // +0x04
    void WriteLong(long value) { Write(&value, 4); }
    void WriteUBYTE(unsigned char value) { Write(&value, 1); }
    void WriteUWORD(unsigned short value) { unsigned long word = value; Write(&word, 2); }
    void WriteDateAndTime(const CDateAndTime* time);
};

void CDataOutputStream::WriteDateAndTime(const CDateAndTime* time)
{
    WriteLong(time->Year);
    WriteUBYTE((unsigned char)time->Month);
    WriteUBYTE((unsigned char)time->Day);
    WriteUBYTE((unsigned char)time->Hour);
    WriteUBYTE((unsigned char)time->Minute);
    WriteUWORD((unsigned short)time->Second);
    WriteUWORD((unsigned short)time->Millisecond);
}
