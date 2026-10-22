// C3DMeshFileMaterialExtChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- skip the version word, read SelfIllumination
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC02D0, length 224 bytes (virtual, slot 7).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined stream Read's slow path).
// Shape: two inlined 4-byte stream Reads through by-value ReadULong() helpers (64-bit position range check,
// chunk fast path, overflow reader); the version result is unused so its fast-path copy is dropped, and
// the helper locals live in the dead stream-argument slot. The single member store is duplicated into
// every exit of the second Read.
// Layout: SelfIllumination +0x10 behind the 0x10 CChunkedFileChunk base (PDB header engine/C3DMeshFileMaterialExtChunk.h agrees).
// Leaf: ReadFromFile@C3DMeshFileMaterialExtChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
// CDataInputStream (retail CMemoryDataInputStream layout, as in the landed 0x00701990 / 0x007AD7F0):
// +4 read position, +0xC current chunk read pointer, +0x14 bytes left in the chunk.
struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;
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
    __forceinline bool ReadBool() { bool value; Read(&value, sizeof(value)); return value; }
};

struct C3DMeshFileMaterialExtChunk {
    virtual void v0();
    unsigned char _base_body[0x0c];        // CChunkedFileChunk: ChunkType[5] + pad + Parent
    unsigned long SelfIllumination;                      // +0x10
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileMaterialExtChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    in.ReadULong();                        // version (unused)
    SelfIllumination = in.ReadULong();
}