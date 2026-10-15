// COpinionPersonalityTraits::TransferBinaryOut(CDataOutputStream& stream) const -- serialise the nine personality-trait floats in declaration order
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045E863, length 138 bytes.
// Pragma: #pragma optimize("s", on) -- `fld ; push ecx ; mov ecx,edi ; fstp [esp]` is the size-TU float push.
// Callees: CDataOutputStream::WriteFloat(float) @ 0x00993EE0 (x9; the manifest label "WriteCBYTE" is wrong --
//          the landed 0x006FEDD0 TransferBinaryOut calls it with floats).
// Layout: PDB COpinionPersonalityTraits is nine floats (36 bytes): BaseOffset, HeroStatScaling,
// VillageOpinionScaling, TimeScalingRunIn, TimeScalingRunOut, PeakScalingIfPos, PeakScalingIfNeg,
// PersistScalingIfPos, PersistScalingIfNeg (+0x00..+0x20). No engine header exists for it.
// Shape: the stream reference is kept in edi and `this` in esi; one WriteFloat per member.
struct CDataOutputStream {
    void WriteFloat(float value);                       // 0x00993EE0
};

struct COpinionPersonalityTraits {
    float BaseOffset;                   // +0x00
    float HeroStatScaling;              // +0x04
    float VillageOpinionScaling;        // +0x08
    float TimeScalingRunIn;             // +0x0c
    float TimeScalingRunOut;            // +0x10
    float PeakScalingIfPos;             // +0x14
    float PeakScalingIfNeg;             // +0x18
    float PersistScalingIfPos;          // +0x1c
    float PersistScalingIfNeg;          // +0x20
    void TransferBinaryOut(CDataOutputStream& stream) const;
};

void COpinionPersonalityTraits::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteFloat(BaseOffset);
    stream.WriteFloat(HeroStatScaling);
    stream.WriteFloat(VillageOpinionScaling);
    stream.WriteFloat(TimeScalingRunIn);
    stream.WriteFloat(TimeScalingRunOut);
    stream.WriteFloat(PeakScalingIfPos);
    stream.WriteFloat(PeakScalingIfNeg);
    stream.WriteFloat(PersistScalingIfPos);
    stream.WriteFloat(PersistScalingIfNeg);
}
