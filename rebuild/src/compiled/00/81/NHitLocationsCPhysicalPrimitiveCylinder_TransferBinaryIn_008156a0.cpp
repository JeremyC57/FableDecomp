// NHitLocations::CPhysicalPrimitiveCylinder::TransferBinaryIn(CMemoryDataInputStream& stream) -- read the base name, then Radius and Height (two inlined 4-byte stream reads)
// Leaf: ?TransferBinaryIn@CPhysicalPrimitiveCylinder@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008156A0, 203 bytes (+5 int3 padding).
// Name: FableWin.pdb ?TransferBinaryIn@CPhysicalPrimitiveCylinder@NHitLocations@@UAEXAAVCMemoryDataInputStream@@@Z;
// slot 5 of the vtable 0x0125BA4C (rtti-vtables.tsv; slot 5 of the sphere is the landed TransferBinaryIn 0x00815620).
// Callees: CDefString::TransferBinaryIn(CMemoryDataInputStream*) @ 0x0044FC00 (on BaseName),
// CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Layout: Ego_r.pdb CPhysicalPrimitiveCylinder {vptr, CDefString BaseName +4, float Radius +8, float Height +0xC};
// the stream model is the landed sphere sibling's (position +4, chunk pointer +0xC, remaining +0x14).
// Shape: the sphere sibling's body with a second float: each inlined CMemoryDataInputStream::Read(&v, 4)
// (64-bit position range check, chunk copy or overflow read) fills ONE float local (shared by both reads; two locals give a second
// frame slot, `push ecx`) homed in the dead
// stream-argument slot, then the member is stored; the second read's member store is duplicated
// into both exits. `ret 4`.
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

namespace NHitLocations {

struct CPhysicalPrimitiveCylinder {
    virtual void v00();
    CDefString BaseName;                           // +0x04
    float      Radius;                             // +0x08
    float      Height;                             // +0x0C
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CPhysicalPrimitiveCylinder::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    BaseName.TransferBinaryIn(&stream);
    float value;
    stream.Read(&value, sizeof(value));
    Radius = value;
    stream.Read(&value, sizeof(value));
    Height = value;
}

} // namespace NHitLocations