// C3DAnimFileObjectChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read Name and SubMeshIndex, then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB0130, length 176 bytes (virtual, slot 7).
// Callees: CDataInputStream::GetPosition (slot 2), ReadNullTerminatedString (slot 6, 0x00994C80, by value),
// CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0, ReadWithSrcChunkOverflow @ 0x00993CA0,
// CChunkedFileSuperChunk::ReadFromFile @ 0x00AA8830.
// Shape: start position; Name assigned from the by-value string temporary (built in the dead stream-argument
// slot, destroyed right after); one inlined 4-byte read through a by-value ReadLong(); the base reader gets
// chunkSize minus a named headerSize local (keeps retail's load order).
// Layout: retail C3DMeshFileChunk base 0x2c (PDB 0x34): Name +0x2c, SubMeshIndex +0x30 (PDB +0x34/+0x38).
// Leaf: ReadFromFile@C3DAnimFileObjectChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CChunkedFileLoader;
class CCharString { public:
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    void* m_pStringData;                                // CCharStringData*
};
// CDataInputStream (retail CMemoryDataInputStream layout, as in the landed 0x00701990 / 0x007AD7F0):
// +4 read position, +0xC current chunk read pointer, +0x14 bytes left in the chunk.
class CDataInputStream { public:
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
    virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadNullTerminatedString();    // slot 6 (+0x18), 0x00994C80 by value
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
    __forceinline bool ReadBool() { unsigned char value; Read(&value, sizeof(value)); return value != 0; }
};

class CChunkedFileSuperChunk { public:
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};

class C3DAnimFileObjectChunk : public CChunkedFileSuperChunk { public:
    CCharString Name;                      // +0x2c (PDB +0x34)
    long SubMeshIndex;                     // +0x30 (PDB +0x38)
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DAnimFileObjectChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    Name = in.ReadNullTerminatedString();
    SubMeshIndex = in.ReadLong();
    unsigned long headerSize = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}