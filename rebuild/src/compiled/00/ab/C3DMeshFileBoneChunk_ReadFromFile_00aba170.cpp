// C3DMeshFileBoneChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the bone indices and matrix, the name only if bytes remain, then the sub-chunks
// Leaf: ReadFromFile@C3DMeshFileBoneChunk
// Retail @ 0x00ABA170, 480 bytes (virtual, slot 7, ret 0Ch).
// Callees: CDataInputStream::GetPosition (vtable slot 2), CDataInputStream::ReadNullTerminatedString (vtable slot 6,
//   hidden CCharString result), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CDataInputStream::ReadMatrix3x4
//   0x00995340 (48-byte result through the hidden pointer, copied with rep movsd), CCharString::operator= 0x0099EFB0,
//   ~CCharString 0x0099EAE0, CChunkedFileSuperChunk::ReadFromFile 0x00AA8830.
// Shape: four inlined 4-byte reads through a by-value ReadLong() (value in the dead stream-argument slot), the
// localisation matrix, then the name only when GetPosition() - start < chunkSize (older files end before it); the
// base reader gets chunkSize - (GetPosition() - start). start and chunkSize stay in edi/esi across the tail.
// Layout: retail C3DMeshFileChunk base is 0x2c bytes (Ego_r.pdb 0x34): BoneIndex +0x2c, ParentIndex +0x30,
// FirstChildIndex +0x34, NextSiblingIndex +0x38, LocalizationMatrix +0x3c, Name +0x6c (PDB +8 each); no engine header.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CChunkedFileLoader;
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct CMatrix3x4 { float m_row[3][4]; };

struct CDataInputStream {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                  // slot 2 (+0x08)
    virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadNullTerminatedString();       // slot 6 (+0x18)
    CMatrix3x4 ReadMatrix3x4();                           // 0x00995340 (by value through the hidden pointer)
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
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
};

struct CChunkedFileSuperChunk {
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};

struct C3DMeshFileBoneChunk : CChunkedFileSuperChunk {
    long BoneIndex;                        // +0x2c (PDB +0x34)
    long ParentIndex;                      // +0x30
    long FirstChildIndex;                  // +0x34
    long NextSiblingIndex;                 // +0x38
    CMatrix3x4 LocalizationMatrix;         // +0x3c
    CCharString Name;                      // +0x6c
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

void C3DMeshFileBoneChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long start = in.GetPosition();
    BoneIndex = in.ReadLong();
    ParentIndex = in.ReadLong();
    FirstChildIndex = in.ReadLong();
    NextSiblingIndex = in.ReadLong();
    LocalizationMatrix = in.ReadMatrix3x4();
    if (in.GetPosition() - start < chunkSize)
        Name = in.ReadNullTerminatedString();
    unsigned long headerSize = in.GetPosition() - start;
    CChunkedFileSuperChunk::ReadFromFile(in, chunkSize - headerSize, loader);
}