// CReactionMatch_SecondaryAxisMRShifted::TransferBinaryIn(CMemoryDataInputStream& stream) -- read the base reaction type, then the secondary axis and its six bound/shift floats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FF040, length 625 bytes (virtual; the mirror of the landed TransferBinaryOut 0x006FEB50).
// Callees: CReactionMatch::TransferBinaryIn @ 0x006FEFB0 (the base override, called non-virtually; it reads ReactionType +4),
// CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Layout: engine/CReactionMatch_SecondaryAxisMRShifted.h (PDB names): Axis +8 (EOpinion), LowerBoundOnAxis +0xc,
// InvIntervalOnAxis +0x10, MShiftZero +0x14, MShiftWeight +0x18, RShiftZero +0x1c, RShiftWeight +0x20.
// Shape: one non-template __forceinline ReadValue(T&) per member (local + Read + store, as in the landed 0x006FFC30);
// the inlined CMemoryDataInputStream::Read of the landed CReactionMatch_MRPyramid::TransferBinaryIn 0x006FF810
// (64-bit position range check, chunk fast path, overflow reader); the inlined locals share the dead
// stream-argument slot (two named locals in the body take a second frame word, push ecx). The last read's fast path is tail-duplicated.
// Leaf: TransferBinaryIn@CReactionMatch_SecondaryAxisMRShifted
#include "engine/CReactionMatch_SecondaryAxisMRShifted.h"

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
    __forceinline void ReadValue(float& out) { float value; Read(&value, sizeof(value)); out = value; }
    __forceinline void ReadValue(long& out) { long value; Read(&value, sizeof(value)); out = value; }
};

struct CReactionMatch_TransferBinaryIn {
    void TransferBinaryIn(CMemoryDataInputStream& stream);     // 0x006FEFB0
};

struct CReactionMatch_SecondaryAxisMRShifted_TransferBinaryIn : CReactionMatch_SecondaryAxisMRShifted {
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CReactionMatch_SecondaryAxisMRShifted_TransferBinaryIn::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    reinterpret_cast<CReactionMatch_TransferBinaryIn*>(this)->TransferBinaryIn(stream);
    stream.ReadValue(*(long*)Axis);
    stream.ReadValue(LowerBoundOnAxis);
    stream.ReadValue(InvIntervalOnAxis);
    stream.ReadValue(MShiftZero);
    stream.ReadValue(MShiftWeight);
    stream.ReadValue(RShiftZero);
    stream.ReadValue(RShiftWeight);
}