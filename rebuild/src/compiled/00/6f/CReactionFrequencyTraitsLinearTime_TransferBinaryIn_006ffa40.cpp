// CReactionFrequencyTraits_LinearTime::TransferBinaryIn(CMemoryDataInputStream& stream) -- read MinWait and WaitRange from a binary persist stream
// Leaf: TransferBinaryIn@CReactionFrequencyTraits_LinearTime
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FFA40, length 194 bytes (__fastcall, ret 4).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the chunk-overflow reader).
// Shape: two copies of the inlined CMemoryDataInputStream::Read of the landed 0x007AD7F0 / 0x00701990 family
// (64-bit position range check, chunk memcpy fast path, overflow reader), both into ONE float local homed in
// the dead stream-argument slot (two named locals give the second its own frame word), each read then stored
// to its member. FrameHappened (+0x0c) is runtime state and is
// not persisted. `this` uses engine/CReactionFrequencyTraits_LinearTime.h.
#include "engine/CReactionFrequencyTraits_LinearTime.h"

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

struct CReactionFrequencyTraits_LinearTime_Methods : CReactionFrequencyTraits_LinearTime {
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CReactionFrequencyTraits_LinearTime_Methods::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    float value;
    stream.Read(&value, sizeof(value));
    MinWait = value;
    stream.Read(&value, sizeof(value));
    WaitRange = value;
}