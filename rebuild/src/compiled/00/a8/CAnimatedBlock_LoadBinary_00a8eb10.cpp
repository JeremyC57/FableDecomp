// CAnimatedBlock::LoadBinary(CMemoryDataInputStream* stream) -- read the skinned mesh block: the base block, the vertex count, bones per vertex, the paletted flag, the group count and the raw group bytes
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8EB10, 463 bytes; `ret 4`. Callees: CBaseBlock::LoadBinary @ 0x00A8E8A0,
//          CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Layout: Ego_r.pdb CAnimatedBlock, as in the landed CAnimatedBlock::SaveBinary 0x00A8E770: 12-byte base block,
//         BoneConstantsFixupOffset +0x0C, VertexCount +0x10, BonesPerVertex +0x14 (ushort), PalettedFlag +0x16,
//         GroupCount +0x17, Groups[18] +0x18.
// Shape: inlined CMemoryDataInputStream::Read (64-bit position range check, fast in-chunk copy, else the overflow
//        reader); the scalar values are homed in the dead stream-argument slot through by-value inline helpers;
//        the variable-size group read skips an empty count and tests `size <= m_remaining` (retail `cmp eax,[ebx+14h] ; jg`).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= m_remaining) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline unsigned long ReadULong() { unsigned long v; Read(&v, sizeof(v)); return v; }
    __forceinline unsigned short ReadUShort() { unsigned short v; Read(&v, sizeof(v)); return v; }
    __forceinline unsigned char ReadUChar() { unsigned char v; Read(&v, sizeof(v)); return v; }
    __forceinline bool ReadBool() { unsigned char v; Read(&v, sizeof(v)); return v != 0; }
};
struct CBaseBlock {
    unsigned long PrimitiveCount;          // +0x00
    unsigned long StartIndex;              // +0x04
    bool          IsStrip;                 // +0x08
    bool          DegenerateTriangles;     // +0x09
    unsigned char ChangeFlags;             // +0x0a
    void LoadBinary(CMemoryDataInputStream* stream);   // 0x00A8E8A0
};
struct CAnimatedBlock : CBaseBlock {
    unsigned long  BoneConstantsFixupOffset;    // +0x0C
    unsigned long  VertexCount;                 // +0x10
    unsigned short BonesPerVertex;              // +0x14
    bool           PalettedFlag;                // +0x16
    unsigned char  GroupCount;                  // +0x17
    unsigned char  Groups[18];                  // +0x18
    void LoadBinary(CMemoryDataInputStream* stream);
};

void CAnimatedBlock::LoadBinary(CMemoryDataInputStream* stream)
{
    CBaseBlock::LoadBinary(stream);
    VertexCount = stream->ReadULong();
    BonesPerVertex = stream->ReadUShort();
    PalettedFlag = stream->ReadBool();
    GroupCount = stream->ReadUChar();
    stream->Read(Groups, GroupCount);
}