// CDefStringTable::WriteTable(CDataOutputStream&) -- write the magic, random id, string count, blob size and the string blob
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D4A50, length 96 bytes.
// Callees: CDataOutputStream::WriteULong @ 0x00993EA0 (four times), CMemoryDataOutputStream::GetSize @ 0x00994B40 and
// ::PeekData @ 0x00995E70 on the stream at +0xc, CDataOutputStream vslot 4 WriteData(const void*, unsigned long).
// Shape: the stream sub-object is addressed once (`add edi,0Ch`) and read through a named pointer; the virtual
// WriteData pushes its size argument before evaluating PeekData() (right-to-left). PDB CDefStringTable: StringStream
// +16, StringCount +44, RandomID +52; retail map (12) and stream (24) are 8 bytes smaller in all.
struct CDataOutputStream {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void WriteData(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteULong(unsigned long value);                            // 0x00993EA0
};

struct CMemoryDataOutputStream {
    unsigned long GetSize();                       // 0x00994B40
    void* PeekData();                              // 0x00995E70
};

struct CDefStringTable {
    unsigned char           StringMap[0xc];        // +0x00 map<CCharString, long>
    CMemoryDataOutputStream StringStream;          // +0x0c (24 bytes)
    unsigned char           _stream[0x18 - sizeof(CMemoryDataOutputStream)];
    unsigned long           StringCount;           // +0x24
    unsigned long           SharedCount;           // +0x28
    unsigned long           RandomID;              // +0x2c
    void WriteTable(CDataOutputStream& stream);
};

void CDefStringTable::WriteTable(CDataOutputStream& stream)
{
    stream.WriteULong(0x7ab1e);
    stream.WriteULong(RandomID);
    stream.WriteULong(StringCount);
    CMemoryDataOutputStream* data = &StringStream;
    unsigned long size = data->GetSize();
    stream.WriteULong(size);
    stream.WriteData(data->PeekData(), size);
}
