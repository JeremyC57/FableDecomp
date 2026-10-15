// CReactionMatch_SecondaryAxisMRShifted::TransferBinaryOut(CDataOutputStream& stream) const -- write the reaction type, the secondary axis and the six axis/shift floats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FEB50, length 101 bytes (virtual, slot 4).
// Callees: CDataOutputStream::WriteLong @ 0x00993EB0 (catalog "WriteCBYTE"; it writes a long) x2,
// CDataOutputStream::WriteFloat @ 0x00993EE0 x6.
// Layout (Ego_r.pdb, engine/CReactionMatch_SecondaryAxisMRShifted.h): CReactionMatch base {vptr, ReactionType +4}, Axis +8 (EOpinion),
// LowerBoundOnAxis +0xc, InvIntervalOnAxis +0x10, MShiftZero +0x14, MShiftWeight +0x18, RShiftZero +0x1c, RShiftWeight +0x20.
// The generated header models the vptr as a plain member, so this TU uses a polymorphic retail model with the
// same names and size. Shape: stream in edi, `this` in esi, eight writes in member order.
// Leaf: TransferBinaryOut@CReactionMatch_SecondaryAxisMRShifted
#include "engine/CReactionMatch_SecondaryAxisMRShifted.h"

struct CDataOutputStream {
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
};

struct CReactionMatch_SecondaryAxisMRShiftedRetail {
    virtual void v00();
    long  ReactionType;                                 // +0x04 EOpinionReactionType
    long  Axis;                                         // +0x08 EOpinion
    float LowerBoundOnAxis;                                           // +0x0c
    float InvIntervalOnAxis;                                           // +0x10
    float MShiftZero;                                   // +0x14
    float MShiftWeight;                                 // +0x18
    float RShiftZero;                                   // +0x1c
    float RShiftWeight;                                 // +0x20
    void TransferBinaryOut(CDataOutputStream& stream) const;
};
FABLE_STATIC_ASSERT(sizeof(CReactionMatch_SecondaryAxisMRShiftedRetail) == sizeof(CReactionMatch_SecondaryAxisMRShifted));

void CReactionMatch_SecondaryAxisMRShiftedRetail::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteLong(ReactionType);
    stream.WriteLong(Axis);
    stream.WriteFloat(LowerBoundOnAxis);
    stream.WriteFloat(InvIntervalOnAxis);
    stream.WriteFloat(MShiftZero);
    stream.WriteFloat(MShiftWeight);
    stream.WriteFloat(RShiftZero);
    stream.WriteFloat(RShiftWeight);
}
