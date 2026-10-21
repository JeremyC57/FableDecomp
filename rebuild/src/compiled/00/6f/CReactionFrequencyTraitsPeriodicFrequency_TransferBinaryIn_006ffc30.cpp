// CReactionFrequencyTraits_PeriodicFrequency::TransferBinaryIn(CMemoryDataInputStream& stream) -- read LowFreqPerMin, HighLowFreqInterval, FramesPerMinute, RampUpTimeFrames and CutoffIntervalFrames from a binary persist stream
// Leaf: TransferBinaryIn@CReactionFrequencyTraits_PeriodicFrequency
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FFC30, length 467 bytes (__fastcall, ret 4).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the chunk-overflow reader).
// Shape: five copies of the inlined CMemoryDataInputStream::Read of the landed sibling 0x006FFA40
// (CReactionFrequencyTraits_LinearTime::TransferBinaryIn): 64-bit position range check, chunk memcpy fast path,
// overflow reader; every read lands in the dead stream-argument slot and is then stored to its member.
// LastCutoffFrame (+0x10) and LastHappenedFrame (+0x1c) are runtime state and are not persisted.
// Each read is a non-template __forceinline ReadValue(T&) (local + Read + store): the inlined locals share the one
// dead-argument slot and keep this in ebx (named locals in the body give a second frame word or swap ebx/edi).
// `this` uses engine/CReactionFrequencyTraits_PeriodicFrequency.h.
#include "engine/CReactionFrequencyTraits_PeriodicFrequency.h"
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
    __forceinline void ReadValue(float& out) { float value; Read(&value, sizeof(value)); out = value; }
    __forceinline void ReadValue(unsigned long& out) { unsigned long value; Read(&value, sizeof(value)); out = value; }
};

struct CReactionFrequencyTraits_PeriodicFrequency_Methods : CReactionFrequencyTraits_PeriodicFrequency {
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CReactionFrequencyTraits_PeriodicFrequency_Methods::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    stream.ReadValue(LowFreqPerMin);
    stream.ReadValue(HighLowFreqInterval);
    stream.ReadValue(FramesPerMinute);
    stream.ReadValue(RampUpTimeFrames);
    stream.ReadValue(CutoffIntervalFrames);
}