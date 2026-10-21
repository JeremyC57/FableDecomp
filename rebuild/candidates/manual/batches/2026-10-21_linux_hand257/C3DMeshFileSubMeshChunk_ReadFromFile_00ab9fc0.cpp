// C3DMeshFileSubMeshChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read Name and the four hierarchy indices, then the sub-chunks
// Leaf: ReadFromFile@C3DMeshFileSubMeshChunk
// Retail @ 0x00AB9FC0, 432 bytes (virtual, slot 7, ret 0Ch).
// Callees: CDataInputStream::GetPosition (vtable slot 2), CDataInputStream::ReadNullTerminatedString (vtable slot 6,
//   hidden CCharString result), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CCharString::operator= 0x0099EFB0,
//   ~CCharString 0x0099EAE0, CChunkedFileSuperChunk::ReadFromFile 0x00AA8830 (base reader, given the bytes left).
// Shape: the name first through the stream's virtual string reader (its temporary takes the dead stream-argument
// slot), then four inlined 4-byte reads through a by-value ReadLong() (value in the same slot), then the base call
// with chunkSize - (GetPosition() - start) -- the sibling HelperPoint reader 0x00AA9BA0 in another order.
// Layout: retail C3DMeshFileChunk base is 0x2c bytes (the engine header C3DMeshFileSubMeshChunk.h has 0x34): Name +0x2c,
// SubMeshIndex +0x30, ParentIndex +0x34, FirstChildIndex +0x38, NextSiblingIndex +0x3c (header +8 each), so the
// header is not used.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                  // slot 2 (+0x08)
    virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadNullTerminatedString();       // slot 6 (+0x18)
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
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
};

struct CChunkedFileSuperChunk {
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};

struct C3DMeshFileSubMeshChunk : CChunkedFileSuperChunk {
    CCharString Name;                      // +0x2c (PDB +0x34)
    long SubMeshIndex;                     // +0x30
    long ParentIndex;                      // +0x34
    long FirstChildIndex;                  // +0x38
    long NextSiblingIndex;                 // +0x3c
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileSubMeshChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    Name = in.ReadNullTerminatedString();
    SubMeshIndex = in.ReadLong();
    ParentIndex = in.ReadLong();
    FirstChildIndex = in.ReadLong();
    NextSiblingIndex = in.ReadLong();
    unsigned long headerSize = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}
