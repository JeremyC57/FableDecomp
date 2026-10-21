// C3DMeshFileHelperPointChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read Position, SubMeshIndex, BoneIndex and Name, then the sub-chunks
// Leaf: ReadFromFile@C3DMeshFileHelperPointChunk
// Retail @ 0x00AA9BA0, 512 bytes (virtual, slot 7, ret 0Ch).
// Callees: CDataInputStream::GetPosition (vtable slot 2), CDataInputStream::ReadNullTerminatedString (vtable slot 6,
//   hidden CCharString result), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CCharString::operator= 0x0099EFB0,
//   ~CCharString 0x0099EAE0, CChunkedFileSuperChunk::ReadFromFile 0x00AA8830 (base reader, given the bytes left).
// Shape: five inlined 4-byte stream reads through by-value ReadFloat()/ReadLong() helpers (their value lives in the
// dead stream-argument slot), the name through the stream's virtual string reader (its temporary reuses that same
// slot), then the base call with chunkSize - (GetPosition() - start) -- the stream model of the landed 0x00AA5370.
// Layout: retail C3DMeshFileChunk base is 0x2c bytes (PDB header 0x34); Position +0x2c, SubMeshIndex +0x38,
// BoneIndex +0x3c, Name +0x40 (same model as the landed ctor 0x00AA9B20), so the engine header is not used.
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
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
};

struct CChunkedFileSuperChunk {
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};

struct C3DVector { float X, Y, Z; };
struct C3DMeshFileHelperPointChunk : CChunkedFileSuperChunk {
    C3DVector Position;                    // +0x2c (PDB +0x34)
    long SubMeshIndex;                     // +0x38
    long BoneIndex;                        // +0x3c
    CCharString Name;                      // +0x40
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileHelperPointChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    Position.X = in.ReadFloat();
    Position.Y = in.ReadFloat();
    Position.Z = in.ReadFloat();
    SubMeshIndex = in.ReadLong();
    BoneIndex = in.ReadLong();
    Name = in.ReadNullTerminatedString();
    unsigned long headerSize = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}