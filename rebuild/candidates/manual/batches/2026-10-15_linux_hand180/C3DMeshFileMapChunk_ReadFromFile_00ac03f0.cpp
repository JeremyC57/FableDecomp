// C3DMeshFileMapChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read MapType, MapDescription and Filename, then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC03F0, length 208 bytes (virtual, slot 7).
// Callees: CDataInputStream::GetPosition (slot 2), ReadNullTerminatedString (slot 6, 0x00994C80, by value),
// CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0, ReadWithSrcChunkOverflow @ 0x00993CA0,
// CChunkedFileSuperChunk::ReadFromFile @ 0x00AA8830.
// Shape: start position; one inlined 4-byte read through a by-value ReadLong() (its local in the dead
// stream-argument slot); two strings assigned from by-value temporaries; the base reader gets chunkSize
// minus a named headerSize local.
// Layout: retail C3DMeshFileChunk base 0x2c (PDB 0x34): MapType +0x2c, MapDescription +0x30, Filename +0x34
// (PDB +0x34/+0x38/+0x3c; CRC follows).
// Leaf: ReadFromFile@C3DMeshFileMapChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
struct CCharString {
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    void* m_pStringData;                                // CCharStringData*
};
// CDataInputStream (retail CMemoryDataInputStream layout, as in the landed 0x00701990 / 0x007AD7F0):
// +4 read position, +0xC current chunk read pointer, +0x14 bytes left in the chunk.
struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
    virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadNullTerminatedString();    // slot 6 (+0x18), 0x00994C80 by value
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC
    long m_10;
    long m_remaining;                      // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    // The typed readers return by value: their locals live in dead argument slots, and an unused result
    // (a skipped version word) loses its fast-path copy entirely.
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
    __forceinline bool ReadBool() { unsigned char value; Read(&value, sizeof(value)); return value != 0; }
};

struct CChunkedFileSuperChunk {
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};

struct C3DMeshFileMapChunk : CChunkedFileSuperChunk {
    long MapType;                          // +0x2c EMaterialMapType (PDB +0x34)
    CCharString MapDescription;            // +0x30 (PDB +0x38)
    CCharString Filename;                  // +0x34 (PDB +0x3c)
    unsigned long CRC;                     // +0x38 (PDB +0x40)
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileMapChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    MapType = in.ReadLong();
    MapDescription = in.ReadNullTerminatedString();
    Filename = in.ReadNullTerminatedString();
    unsigned long headerSize = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}
