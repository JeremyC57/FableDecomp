// CReactionMatch_AsymmetricPyramid::TransferBinaryIn(CMemoryDataInputStream& stream) -- read the base reaction type, then the two axis opinion types and the six centre/radius floats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FF540, length 683 bytes (virtual; the mirror of the landed TransferBinaryOut 0x006FEC30).
// Callees: CReactionMatch::TransferBinaryIn @ 0x006FEFB0 (the base override, called non-virtually; it reads ReactionType +4),
// CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0.
// Layout: engine/CReactionMatch_AsymmetricPyramid.h (PDB names): XAxisOpinionType +8, YAxisOpinionType +0xc (EOpinion),
// XCentre +0x10, NegInvXRadius +0x14, PosInvXRadius +0x18, YCentre +0x1c, NegInvYRadius +0x20, PosInvYRadius +0x24.
// Shape: one non-template __forceinline ReadValue(T&) per member (local + Read + store, as in the landed 0x006FFC30);
// the inlined CMemoryDataInputStream::Read of the landed CReactionMatch_MRPyramid::TransferBinaryIn 0x006FF810
// (64-bit position range check, chunk fast path, overflow reader); the inlined locals share the dead
// stream-argument slot (two named locals in the body take a second frame word, push ecx). The last read's fast path is tail-duplicated.
// Leaf: TransferBinaryIn@CReactionMatch_AsymmetricPyramid
#include "engine/CReactionMatch_AsymmetricPyramid.h"

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

struct CReactionMatch_AsymmetricPyramid_TransferBinaryIn : CReactionMatch_AsymmetricPyramid {
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CReactionMatch_AsymmetricPyramid_TransferBinaryIn::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    reinterpret_cast<CReactionMatch_TransferBinaryIn*>(this)->TransferBinaryIn(stream);
    stream.ReadValue(*(long*)XAxisOpinionType);
    stream.ReadValue(*(long*)YAxisOpinionType);
    stream.ReadValue(XCentre);
    stream.ReadValue(NegInvXRadius);
    stream.ReadValue(PosInvXRadius);
    stream.ReadValue(YCentre);
    stream.ReadValue(NegInvYRadius);
    stream.ReadValue(PosInvYRadius);
}
