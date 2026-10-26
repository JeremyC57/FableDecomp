// NHitLocations::CPhysicalPrimitiveSphere::TransferBinaryIn(CMemoryDataInputStream* stream) -- read the base name, then the radius (one inlined 4-byte stream read)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815620, length 117 bytes (+ int3 padding).
// Leaf: TransferBinaryIn
// Callees: CDefString::TransferBinaryIn(CMemoryDataInputStream*) @ 0x0044FC00 (on BaseName),
// CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Layout: Ego_r.pdb CPhysicalPrimitiveSphere {vptr, CDefString BaseName +4, float Radius +8}; the stream as the landed
// TransferBinaryIn 0x007AD7F0 (position +4, chunk pointer +0xc, remaining +0x14).
// Shape: the inlined CMemoryDataInputStream::Read(&radius, 4) (64-bit position range check, chunk copy or overflow read;
// as the landed 0x007AD7F0) fills a local homed in the dead stream-argument slot, then Radius = radius in each exit;
// `ret 4`.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CMemoryDataInputStream { public:
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

class CDefString { public:
    long TablePos;
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};

struct CPhysicalPrimitiveSphere {
    virtual void v00();
    CDefString BaseName;                           // +0x04
    float      Radius;                             // +0x08
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};

void CPhysicalPrimitiveSphere::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    BaseName.TransferBinaryIn(stream);
    float radius;
    stream->Read(&radius, sizeof(radius));
    Radius = radius;
}