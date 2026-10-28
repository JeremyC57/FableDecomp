// CWaterSeaStaticMapInfoBlock::Load(CMemoryDataInputStream&, long) -- read the 8-byte info header from a memory stream
// Retail: Fable.exe @ 0x00B6D6E0, 118 bytes. PDB ?Load@CWaterSeaStaticMapInfoBlock@@QAE_NAAVCMemoryDataInputStream@@J@Z
// Callee: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) 0x00993CA0 (chunk overflow path).
// CWaterSeaStaticMapInfoBlock (12 bytes, PDB): +0 SeaStaticMapVersionNumber, +4 DataStreamPosition, +8 CutoutBlockCRC.
// A block size other than 8 fails; otherwise the inlined CMemoryDataInputStream::Read(this, 8) copies the first two
// longs (the same inline Read as the landed 0x007AD7F0 family: 64-bit [0, 0x7FFFFFFF] position check, memcpy from
// the chunk pointer +0xC, remaining +0x14, position +4) and the block reports success either way.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void* m_pVtable;                       // +0x00
    unsigned long m_position;              // +0x04
    long StreamSize;
    const unsigned char* m_pData;          // +0x0C current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
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

struct CWaterSeaStaticMapInfoBlock {
    long SeaStaticMapVersionNumber;   // +0x00
    long DataStreamPosition;          // +0x04
    long CutoutBlockCRC;              // +0x08
    bool Load(CMemoryDataInputStream& stream, long blockSize);
};

bool CWaterSeaStaticMapInfoBlock::Load(CMemoryDataInputStream& stream, long blockSize)
{
    if (blockSize != 8)
        return false;
    stream.Read(this, 8);
    return true;
}