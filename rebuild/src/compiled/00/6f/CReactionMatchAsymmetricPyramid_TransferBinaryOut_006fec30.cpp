// CReactionMatch_AsymmetricPyramid::TransferBinaryOut(CDataOutputStream& stream) const -- write the reaction type, both opinion axes and the six pyramid floats
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FEC30, length 112 bytes (vtable 0x0125B718 slot 4).
// Callees: CDataOutputStream::WriteSLONG @ 0x00993EB0 (catalog "WriteCBYTE"; it writes a long) x3,
// CDataOutputStream::WriteFloat @ 0x00993EE0 x6.
// Layout (Ego_r.pdb, engine/CReactionMatch_AsymmetricPyramid.h): CReactionMatch base {vptr, ReactionType +4},
// XAxisOpinionType +8, YAxisOpinionType +0xc (EOpinion), XCentre +0x10, NegInvXRadius +0x14, PosInvXRadius +0x18,
// YCentre +0x1c, NegInvYRadius +0x20, PosInvYRadius +0x24. The generated header models the vptr as a plain
// member and the enums as byte arrays, so this TU uses a polymorphic retail model with the same names and size.
// Shape: stream kept in edi, `this` in esi, nine writes in member order (the CReactionMatch base's ReactionType
// first, written inline).
// Leaf: TransferBinaryOut@CReactionMatch_AsymmetricPyramid
#include "engine/CReactionMatch_AsymmetricPyramid.h"

class CDataOutputStream { public:
    void WriteFloat(float value);                       // 0x00993EE0
public:
    void WriteSLONG(long);   // 0x00993EB0
};

struct CReactionMatch_AsymmetricPyramidRetail {
    virtual void v00();
    long  ReactionType;                                 // +0x04 EOpinionReactionType
    long  XAxisOpinionType;                             // +0x08 EOpinion
    long  YAxisOpinionType;                             // +0x0c EOpinion
    float XCentre;                                      // +0x10
    float NegInvXRadius;                                // +0x14
    float PosInvXRadius;                                // +0x18
    float YCentre;                                      // +0x1c
    float NegInvYRadius;                                // +0x20
    float PosInvYRadius;                                // +0x24
    void TransferBinaryOut(CDataOutputStream& stream) const;
};
FABLE_STATIC_ASSERT(sizeof(CReactionMatch_AsymmetricPyramidRetail) == sizeof(CReactionMatch_AsymmetricPyramid));

void CReactionMatch_AsymmetricPyramidRetail::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteSLONG(ReactionType);
    stream.WriteSLONG(XAxisOpinionType);
    stream.WriteSLONG(YAxisOpinionType);
    stream.WriteFloat(XCentre);
    stream.WriteFloat(NegInvXRadius);
    stream.WriteFloat(PosInvXRadius);
    stream.WriteFloat(YCentre);
    stream.WriteFloat(NegInvYRadius);
    stream.WriteFloat(PosInvYRadius);
}