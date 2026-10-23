// Grade: DIFFER(93v1533) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CLocalDetailPrimitiveRepeatedMesh::Load(CDataInputStream* stream) -- read the repeated-mesh local-detail primitive from
//   the stream: header counts (inline 4-byte reads with ReadWithSrcChunkOverflow fallback), allocate the instance and
//   vertex blocks from the landscape memory manager (__allmul sizes) and read them, building the four per-block managers
// Leaf: Load
// Stub: first-pass skeleton only (the first inline read); the allocation and the block reads are not reconstructed yet.
struct CDataInputStream {
    virtual void v00();
    unsigned long        m_position;                    // +0x04
    long                 StreamSize;                    // +0x08
    const unsigned char* m_pData;                       // +0x0c
    long                 SourceChunkStreamPos;          // +0x10
    long                 m_remaining;                   // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    __forceinline void ReadULONG(unsigned long& value)
    {
        __int64 newPosition = (__int64)(m_position + 4);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (m_remaining >= 4) {
            value = *(const unsigned long*)m_pData;
            m_pData += 4;
            m_remaining -= 4;
            m_position += 4;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)&value, 4);
    }
};
class CLocalDetailPrimitiveRepeatedMesh {
public:
    unsigned char _pad00[0x18];
    unsigned long Count;                                // +0x18 (descriptive)
    void Load(CDataInputStream* stream);
};
void CLocalDetailPrimitiveRepeatedMesh::Load(CDataInputStream* stream)
{
    stream->ReadULONG(Count);
}
