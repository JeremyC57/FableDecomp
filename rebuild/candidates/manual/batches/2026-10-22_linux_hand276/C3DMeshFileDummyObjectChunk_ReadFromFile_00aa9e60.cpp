// C3DMeshFileDummyObjectChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the version, direction, transform and local-origin flag, then the helper-point base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA9E60, length 608 bytes (virtual, chunk reader slot, ret 0Ch).
// Callees: CDataInputStream::GetPosition (vtable slot 2) x2, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0,
//          CDataInputStream::ReadMatrix3x4 @ 0x00995340 (48-byte result by value, copied with rep movsd),
//          C3DMeshFileHelperPointChunk::ReadFromFile @ 0x00AA9BA0 (the base reader, given the bytes left).
// Shape: start = GetPosition(); version (kept in the dead stream-argument slot); Direction X/Y/Z through by-value
// ReadFloat helpers; version >= 2 reads the transform, older files get the inline SetIdentity chained assignment
// (nine zero stores high to low, then the 1.0f diagonal); version >= 3 reads a one-byte bool, older files clear it;
// then the base reader with chunkSize - (GetPosition() - start).
// Layout: retail C3DMeshFileHelperPointChunk is 0x44 bytes (landed 0x00AA9BA0 model: retail C3DMeshFileChunk base
// 0x2c vs PDB 0x34), so Direction +0x44, Transform +0x50, UseLocalOrigin +0x80 (the PDB header's +0x4c/+0x58/+0x88
// are 8 bytes high); the engine header is not used.
// Leaf: ReadFromFile@C3DMeshFileDummyObjectChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    void SetIdentity()
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};
struct CCharString { void* m_pStringData; };

struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                  // slot 2 (+0x08)
    CMatrix3x4 ReadMatrix3x4();                           // 0x00995340 (by value through the hidden pointer)
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
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
    __forceinline bool ReadBool() { char value; Read(&value, sizeof(value)); return value != 0; }
};

struct CChunkedFileSuperChunk {
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
};
struct C3DMeshFileHelperPointChunk : CChunkedFileSuperChunk {
    C3DVector Position;                    // +0x2c
    long SubMeshIndex;                     // +0x38
    long BoneIndex;                        // +0x3c
    CCharString Name;                      // +0x40
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);  // 0x00AA9BA0
};
struct C3DMeshFileDummyObjectChunk : C3DMeshFileHelperPointChunk {
    C3DVector Direction;                   // +0x44 (PDB +0x4c)
    CMatrix3x4 Transform;                  // +0x50
    bool UseLocalOrigin;                   // +0x80
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileDummyObjectChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    unsigned long version = in.ReadULong();
    Direction.X = in.ReadFloat();
    Direction.Y = in.ReadFloat();
    Direction.Z = in.ReadFloat();
    if (version >= 2)
        Transform = in.ReadMatrix3x4();
    else
        Transform.SetIdentity();
    if (version >= 3)
        UseLocalOrigin = in.ReadBool();
    else
        UseLocalOrigin = false;
    unsigned long headerSize = in.GetPosition() - start;
    C3DMeshFileHelperPointChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}
