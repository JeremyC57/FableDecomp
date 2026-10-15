// CReactionFrequencyTraits_PeriodicFrequency::TransferBinaryOut(CDataOutputStream& stream) const -- serialise the two frequency floats and the three frame counts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FEDD0, length 68 bytes (virtual, `?TransferBinaryOut@CReactionFrequencyTraits_PeriodicFrequency@@UBEXAAVCDataOutputStream@@@Z`).
// Callees: CDataOutputStream::WriteFloat @ 0x00993EE0 (x2), WriteULong @ 0x00993EA0 (x3).
// Layout: engine/CReactionFrequencyTraits_PeriodicFrequency.h (PDB names): LowFreqPerMin +0x04,
// HighLowFreqInterval +0x08, FramesPerMinute +0x0c, RampUpTimeFrames +0x14, CutoffIntervalFrames +0x18;
// LastCutoffFrame (+0x10) and LastHappenedFrame (+0x1c) are runtime state and are not written.
// Shape: plain member pushes in declaration order with the stream reference kept in edi; the method is attached
// through a derived shell because the generated header is a plain layout struct.
// Leaf: TransferBinaryOut
#include "engine/CReactionFrequencyTraits_PeriodicFrequency.h"

struct CDataOutputStream {
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteFloat(float value);                       // 0x00993EE0
};

struct CReactionFrequencyTraits_PeriodicFrequency_TransferBinaryOut : CReactionFrequencyTraits_PeriodicFrequency {
    void TransferBinaryOut(CDataOutputStream& stream) const;
};

void CReactionFrequencyTraits_PeriodicFrequency_TransferBinaryOut::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteFloat(LowFreqPerMin);
    stream.WriteFloat(HighLowFreqInterval);
    stream.WriteULong(FramesPerMinute);
    stream.WriteULong(RampUpTimeFrames);
    stream.WriteULong(CutoffIntervalFrames);
}