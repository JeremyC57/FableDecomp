// CBaseBlock::LoadBinary(CMemoryDataInputStream* stream) -- read the base mesh block: two words, the strip flag, the change-flags byte and the degenerate-triangles flag
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8E8A0, 452 bytes; `ret 4`. Callee: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Layout: Ego_r.pdb CBaseBlock (12 bytes), as in the landed siblings CBaseBlock::SaveBinary 0x00A8E6C0 and
//         CStaticBlock::LoadBinary 0x00A8EA70: PrimitiveCount +0, StartIndex +4, IsStrip +8, DegenerateTriangles +9, ChangeFlags +0xA.
// Shape: five inlined CMemoryDataInputStream::Read calls (64-bit position range check, fast in-chunk copy, else the
//        overflow reader), each value homed in the dead stream-argument slot through a by-value inline Read helper.
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
    __forceinline unsigned long ReadULong() { unsigned long v; Read(&v, sizeof(v)); return v; }
    __forceinline unsigned char ReadUChar() { unsigned char v; Read(&v, sizeof(v)); return v; }
    __forceinline bool ReadBool() { unsigned char v; Read(&v, sizeof(v)); return v != 0; }
};
struct CBaseBlock {
    unsigned long PrimitiveCount;          // +0x00
    unsigned long StartIndex;              // +0x04
    bool          IsStrip;                 // +0x08
    bool          DegenerateTriangles;     // +0x09
    unsigned char ChangeFlags;             // +0x0a
    void LoadBinary(CMemoryDataInputStream* stream);
};

void CBaseBlock::LoadBinary(CMemoryDataInputStream* stream)
{
    PrimitiveCount = stream->ReadULong();
    StartIndex = stream->ReadULong();
    IsStrip = stream->ReadBool();
    ChangeFlags = stream->ReadUChar();
    DegenerateTriangles = stream->ReadBool();
}
