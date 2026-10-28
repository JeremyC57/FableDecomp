// CReactionFrequencyTraits_ControlledCount::TransferBinaryIn(CMemoryDataInputStream& stream) -- read AllowIndividualRepeats, MinGapFrames, CountRechargePerFrame, CurrentAvailableCount, MaxCount and InvMaxCount from a binary persist stream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FFF00, length 535 bytes (virtual, ret 4; the mirror of the landed
// TransferBinaryOut 0x006FEF30).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the chunk-overflow reader).
// Layout: engine/CReactionFrequencyTraits_ControlledCount.h (PDB names): AllowIndividualRepeats +4 (bool),
// MinGapFrames +0x10, CountRechargePerFrame +0x14, CurrentAvailableCount +0x18, MaxCount +0x1c, InvMaxCount +0x20.
// PLastReactionCreature, InRisingPhase and the two frame stamps are runtime state and are not persisted.
// Shape: six copies of the inlined CMemoryDataInputStream::Read of the landed siblings 0x006FFA40/0x006FFC30
// (64-bit position range check, chunk memcpy fast path, overflow reader); every read lands in the dead
// stream-argument slot and is then stored to its member. Each read is a non-template __forceinline ReadValue(T&)
// (local + Read + store), as in the siblings; the bool is read as a byte and normalised (`test al,al; setne`).
// Leaf: TransferBinaryIn@CReactionFrequencyTraits_ControlledCount
#include "engine/CReactionFrequencyTraits_ControlledCount.h"

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
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
    __forceinline void ReadValue(bool& out) { unsigned char value; Read(&value, sizeof(value)); out = value != 0; }
    __forceinline void ReadValue(float& out) { float value; Read(&value, sizeof(value)); out = value; }
    __forceinline void ReadValue(unsigned long& out) { unsigned long value; Read(&value, sizeof(value)); out = value; }
};

struct CReactionFrequencyTraits_ControlledCount_Methods : CReactionFrequencyTraits_ControlledCount {
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CReactionFrequencyTraits_ControlledCount_Methods::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    stream.ReadValue(AllowIndividualRepeats);
    stream.ReadValue(MinGapFrames);
    stream.ReadValue(CountRechargePerFrame);
    stream.ReadValue(CurrentAvailableCount);
    stream.ReadValue(MaxCount);
    stream.ReadValue(InvMaxCount);
}