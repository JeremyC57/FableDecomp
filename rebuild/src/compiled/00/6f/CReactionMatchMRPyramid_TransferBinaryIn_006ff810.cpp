// CReactionMatch_MRPyramid::TransferBinaryIn(CMemoryDataInputStream& stream) -- read the asymmetric-pyramid base state, then the three MR shift floats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FF810, length 285 bytes (virtual).
// Callees: CReactionMatch_AsymmetricPyramid::TransferBinaryIn @ 0x006FF540 (the base override, called non-virtually),
// CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Layout: engine/CReactionMatch_MRPyramid.h (PDB names): ScarinessShift +0x28, AgreeablenessShift +0x2c, AttractivenessShift +0x30
// (the mirror of the landed TransferBinaryOut 0x006FECA0).
// Shape: the inlined CMemoryDataInputStream::Read of the landed CReactionMatch_Sphere::TransferBinaryIn 0x00701990 (64-bit
// position range check, chunk fast path, overflow reader) three times into one float local homed in the dead stream-argument
// slot, each followed by one store of the local to its member.
// Leaf: TransferBinaryIn@CReactionMatch_MRPyramid
#include "engine/CReactionMatch_AsymmetricPyramid.h"
#include "engine/CReactionMatch_MRPyramid.h"

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

struct CReactionMatch_AsymmetricPyramid_TransferBinaryIn : CReactionMatch_AsymmetricPyramid {
    void TransferBinaryIn(CMemoryDataInputStream& stream);     // 0x006FF540
};

struct CReactionMatch_MRPyramid_TransferBinaryIn : CReactionMatch_MRPyramid {
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CReactionMatch_MRPyramid_TransferBinaryIn::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    reinterpret_cast<CReactionMatch_AsymmetricPyramid_TransferBinaryIn*>(this)->TransferBinaryIn(stream);
    float value;
    stream.Read(&value, sizeof(value));
    ScarinessShift = value;
    stream.Read(&value, sizeof(value));
    AgreeablenessShift = value;
    stream.Read(&value, sizeof(value));
    AttractivenessShift = value;
}