// CReactionFrequencyTraits_ControlledCount::TransferBinaryOut(CDataOutputStream& stream) const -- serialise the repeat flag, the minimum gap and the four count floats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FEF30, length 81 bytes (virtual, `?TransferBinaryOut@CReactionFrequencyTraits_ControlledCount@@UBEXAAVCDataOutputStream@@@Z`).
// Callees: CDataOutputStream::WriteBool @ 0x00993E30, WriteULong @ 0x00993EA0, WriteFloat @ 0x00993EE0 (x4).
// Layout: engine/CReactionFrequencyTraits_ControlledCount.h (PDB names): AllowIndividualRepeats +0x04,
// MinGapFrames +0x10, CountRechargePerFrame +0x14, CurrentAvailableCount +0x18, MaxCount +0x1c, InvMaxCount +0x20.
// The persistent state skips PLastReactionCreature, InRisingPhase and the two frame stamps (runtime only).
// Shape: the bool widened through a cleared eax (`xor eax,eax ; mov al,[esi+4]`), the stream reference kept in
// edi across the six calls; the method is attached through a derived shell because the generated header is a
// plain layout struct.
// Leaf: TransferBinaryOut
#include "engine/CReactionFrequencyTraits_ControlledCount.h"

class CDataOutputStream { public:
    void WriteBool(bool value);                         // 0x00993E30
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteFloat(float value);                       // 0x00993EE0
};

struct CReactionFrequencyTraits_ControlledCount_TransferBinaryOut : CReactionFrequencyTraits_ControlledCount {
    void TransferBinaryOut(CDataOutputStream& stream) const;
};

void CReactionFrequencyTraits_ControlledCount_TransferBinaryOut::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteBool(AllowIndividualRepeats);
    stream.WriteULong(MinGapFrames);
    stream.WriteFloat(CountRechargePerFrame);
    stream.WriteFloat(CurrentAvailableCount);
    stream.WriteFloat(MaxCount);
    stream.WriteFloat(InvMaxCount);
}