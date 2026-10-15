// CStaticBlock::LoadBinary(CMemoryDataInputStream* stream) -- load the base block, then the material index (one inlined 4-byte stream read)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8EA70, 145 bytes; `ret 4`. Callees: CBaseBlock::LoadBinary @ 0x00A8E8A0,
//          CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Layout: Ego_r.pdb CStaticBlock (0x20): the 12-byte CBaseBlock base, texture fix-ups and pointers, MaterialIndex +0x1c.
// Shape: the landed TransferBinaryIn family (0x007AD7F0): a `value` local homed in the dead stream-argument slot, the
//        inlined CMemoryDataInputStream::Read with its 64-bit position range check (each exit duplicates the member store).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long m_10;
    long m_remaining;                      // +0x14 bytes left in the current chunk
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
};
struct CTexture;
struct CBaseBlock {
    unsigned long PrimitiveCount;          // +0x00
    unsigned long StartIndex;              // +0x04
    bool          IsStrip;                 // +0x08
    bool          DegenerateTriangles;     // +0x09
    unsigned char ChangeFlags;             // +0x0a
    void LoadBinary(CMemoryDataInputStream* stream);   // 0x00A8E8A0
};
struct CStaticBlock : CBaseBlock {
    unsigned long DiffuseTextureFixupOffset;   // +0x0c
    unsigned long BumpTextureFixupOffset;      // +0x10
    CTexture*     DiffuseTexture;              // +0x14
    CTexture*     BumpTexture;                 // +0x18
    long          MaterialIndex;               // +0x1c
    void LoadBinary(CMemoryDataInputStream* stream);
};

void CStaticBlock::LoadBinary(CMemoryDataInputStream* stream)
{
    CBaseBlock::LoadBinary(stream);
    long value;
    stream->Read(&value, sizeof(value));
    MaterialIndex = value;
}
