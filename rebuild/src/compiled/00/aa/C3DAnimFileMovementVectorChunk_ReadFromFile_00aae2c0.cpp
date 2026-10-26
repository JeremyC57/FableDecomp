// C3DAnimFileMovementVectorChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the movement vector, then the super chunk's sub chunks
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAE2C0, 310 bytes (virtual, `ret 0Ch`).
// Callees: CDataInputStream::GetPosition (vtable slot 2), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0
// (the inlined stream Read's slow path), CChunkedFileSuperChunk::ReadFromFile 0x00AA8830.
// Shape: the chunk end (`GetPosition() + chunkSize`, edi) is taken first; three inlined 4-byte stream Reads (64-bit
// position range check, chunk fast path, overflow reader) through by-value ReadFloat() helpers, each value homed
// in the dead `in` argument slot and stored once; then the base super chunk reads the rest of the chunk
// (`end - GetPosition()`, the loader pushed first: right-to-left argument evaluation).
// Layout: retail keeps the vector at +0x2C (X), +0x30 (Y), +0x34 (Z); the engine header has the PDB offsets
// (+0x34..), 8 bytes higher (the PDB's 16-byte containers in the super chunk base), so a local model is used.
// Leaf: ReadFromFile@C3DAnimFileMovementVectorChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CChunkedFileLoader;
// CDataInputStream (retail CMemoryDataInputStream layout, as in the landed 0x00AA52B0):
// +4 read position, +0xC current chunk read pointer, +0x14 bytes left in the chunk.
class CDataInputStream { public:
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14
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
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CChunkedFileSuperChunk { public:
    virtual void v0();
    unsigned char _body[0x28];             // CChunkedFileChunk (0x10) + the sub chunk containers (retail 0x2C total)
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};

class C3DAnimFileMovementVectorChunk : public CChunkedFileSuperChunk { public:
    C3DVector Vector;                      // +0x2C
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DAnimFileMovementVectorChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long end = in.GetPosition() + chunkSize;
    Vector.X = in.ReadFloat();
    Vector.Y = in.ReadFloat();
    Vector.Z = in.ReadFloat();
    CChunkedFileSuperChunk::ReadFromFile(in, end - in.GetPosition(), loader);
}