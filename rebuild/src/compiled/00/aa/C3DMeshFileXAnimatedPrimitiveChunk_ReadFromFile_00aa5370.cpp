// C3DMeshFileXAnimatedPrimitiveChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read BonesPerBlock and BonesPerVertex, then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA5370, length 240 bytes (virtual, slot 7).
// Callees: CDataInputStream::GetPosition (vtable slot 2), CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0,
// CChunkedFileSuperChunk::ReadFromFile @ 0x00AA8830 (the base reader, given the bytes left after the header).
// Shape (headerSize named local keeps retail load order): start position, two inlined 4-byte reads through by-value ReadULong() helpers (their locals live in
// the dead stream-argument slot), then the base call with chunkSize - (GetPosition() - start).
// Layout: retail C3DMeshFileChunk base is 0x2c (PDB 0x34), so BonesPerBlock +0x2c / BonesPerVertex +0x30
// (engine header +0x34/+0x38); local model for that reason.
// Leaf: ReadFromFile@C3DMeshFileXAnimatedPrimitiveChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CChunkedFileLoader;
// CDataInputStream (retail CMemoryDataInputStream layout, as in the landed 0x00701990 / 0x007AD7F0):
// +4 read position, +0xC current chunk read pointer, +0x14 bytes left in the chunk.
class CDataInputStream { public:
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

class CChunkedFileSuperChunk { public:
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};

class C3DMeshFileXAnimatedPrimitiveChunk : public CChunkedFileSuperChunk { public:
    unsigned long BonesPerBlock;           // +0x2c (PDB +0x34)
    unsigned long BonesPerVertex;          // +0x30 (PDB +0x38)
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileXAnimatedPrimitiveChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    BonesPerBlock = in.ReadULong();
    BonesPerVertex = in.ReadULong();
    unsigned long headerSize = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}