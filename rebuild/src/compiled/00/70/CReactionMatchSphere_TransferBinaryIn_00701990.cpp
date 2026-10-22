// CReactionMatch_Sphere::TransferBinaryIn(CMemoryDataInputStream& stream) -- read the reaction type, the centre opinion set and the squared radius
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00701990, length 125 bytes (virtual).
// Callees: CReactionMatch::TransferBinaryIn @ 0x006FEFB0 (the base reader of ReactionType; catalog label
// "C3DMeshFileXTextureIndexChunk::ReadFromFile" is a propagated byte twin), COpinionSet::TransferBinaryIn
// @ 0x007020D0 (catalog "C3DMeshFileXTriangleStripChunk::ReadFromFile", also propagated),
// CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Layout: retail offsets as in the landed TransferBinaryOut 0x007003A0 (retail CArray<T> is three pointers, so
// COpinionSet Centre is +0x08..+0x13 and SqRadius +0x14; engine/CReactionMatch_Sphere.h has the PDB's +0x18).
// Shape: the inlined CMemoryDataInputStream::Read of the landed 0x007AD7F0 (64-bit position range check, chunk
// memcpy fast path, overflow reader) into a float local homed in the dead stream-argument slot, then one store
// of the local to SqRadius on every path.
// Leaf: TransferBinaryIn@CReactionMatch_Sphere
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

template<class T> struct CArray {                       // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
};

struct COpinionSet {
    CArray<float> OpinionValues;                        // +0x00
    void TransferBinaryIn(CMemoryDataInputStream& stream);  // 0x007020D0
};

struct CReactionMatch {
    void* __vftable;                                    // +0x00
    long  ReactionType;                                 // +0x04 EOpinionReactionType
    void TransferBinaryIn(CMemoryDataInputStream& stream);  // 0x006FEFB0
};

struct CReactionMatch_Sphere : CReactionMatch {
    COpinionSet Centre;                                 // +0x08
    float       SqRadius;                               // +0x14
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CReactionMatch_Sphere::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    CReactionMatch::TransferBinaryIn(stream);
    Centre.TransferBinaryIn(stream);
    float sqRadius;
    stream.Read(&sqRadius, sizeof(sqRadius));
    SqRadius = sqRadius;
}