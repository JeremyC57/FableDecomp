// C3DAnimFileRootChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read LoopFlag (a byte normalised to bool) and Duration, then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A98AF0, length 240 bytes (virtual, slot 7).
// Callees: CDataInputStream::GetPosition (vtable slot 2), CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0,
// CChunkedFileSuperChunk::ReadFromFile @ 0x00AA8830 (the base reader, given the bytes left after the header).
// Shape (headerSize named local keeps retail load order): start position, an inlined 1-byte read through a by-value ReadBool() helper (`unsigned char` read, `!= 0` gives retail's test/setne) and a 4-byte ReadFloat() (their locals live in
// the dead stream-argument slot), then the base call with chunkSize - (GetPosition() - start).
// Layout: retail C3DMeshFileChunk base is 0x2c (PDB 0x34), so LoopFlag +0x2c / Duration +0x30 (PDB +0x34/+0x38),
// as in the landed HasMovementVector 0x00AAE4D0.
// Leaf: ReadFromFile@C3DAnimFileRootChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
// CDataInputStream (retail CMemoryDataInputStream layout, as in the landed 0x00701990 / 0x007AD7F0):
// +4 read position, +0xC current chunk read pointer, +0x14 bytes left in the chunk.
struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
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

struct C3DAnimFileRootChunk : CChunkedFileSuperChunk {
    bool  LoopFlag;                        // +0x2c (PDB +0x34)
    float Duration;                        // +0x30 (PDB +0x38)
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DAnimFileRootChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    LoopFlag = in.ReadBool();
    Duration = in.ReadFloat();
    unsigned long headerSize = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}
