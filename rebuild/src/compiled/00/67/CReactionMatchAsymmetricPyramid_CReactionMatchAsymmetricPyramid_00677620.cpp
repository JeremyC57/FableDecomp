// CReactionMatch_AsymmetricPyramid::CReactionMatch_AsymmetricPyramid(type, xAxis, yAxis, xMin, xCentre, xMax, yMin, yCentre, yMax) -- store the axes and centres, precompute the four inverse half-widths
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00677620, length 114 bytes (`ret 24h`, nine arguments).
// Vtable 0x0125B718 (rtti-vtables.tsv: CReactionMatch_AsymmetricPyramid, slot 1 Match 0x007002A0, slot 4
// TransferBinaryOut 0x006FEC30). No callees.
// Layout (Ego_r.pdb, engine/CReactionMatch_AsymmetricPyramid.h): CReactionMatch base {vptr, ReactionType +4},
// XAxisOpinionType +8, YAxisOpinionType +0xc, XCentre +0x10, NegInvXRadius +0x14, PosInvXRadius +0x18,
// YCentre +0x1c, NegInvYRadius +0x20, PosInvYRadius +0x24 (polymorphic retail model, same names and size).
// Shape: the abstract CReactionMatch base is `__declspec(novtable)` (only the derived vtable store survives) and
// stores ReactionType from its own initialiser; the radii are `1.0f / (centre - min)` and `1.0f / (max - centre)`
// (`fsub ; fdivr [1.0f]`).
// Leaf: ??0CReactionMatch_AsymmetricPyramid
#include "engine/CReactionMatch_AsymmetricPyramid.h"

struct __declspec(novtable) CReactionMatch {
    virtual void v00();
    long ReactionType;                                  // +0x04 EOpinionReactionType
    CReactionMatch(long type) : ReactionType(type) {}
};

struct CReactionMatch_AsymmetricPyramidRetail : CReactionMatch {
    long  XAxisOpinionType;                             // +0x08 EOpinion
    long  YAxisOpinionType;                             // +0x0c EOpinion
    float XCentre;                                      // +0x10
    float NegInvXRadius;                                // +0x14
    float PosInvXRadius;                                // +0x18
    float YCentre;                                      // +0x1c
    float NegInvYRadius;                                // +0x20
    float PosInvYRadius;                                // +0x24
    virtual void v00();
    CReactionMatch_AsymmetricPyramidRetail(long type, long xAxis, long yAxis, float xMin, float xCentre, float xMax,
                                           float yMin, float yCentre, float yMax);
};
FABLE_STATIC_ASSERT(sizeof(CReactionMatch_AsymmetricPyramidRetail) == sizeof(CReactionMatch_AsymmetricPyramid));

CReactionMatch_AsymmetricPyramidRetail::CReactionMatch_AsymmetricPyramidRetail(long type, long xAxis, long yAxis,
        float xMin, float xCentre, float xMax, float yMin, float yCentre, float yMax)
    : CReactionMatch(type),
      XAxisOpinionType(xAxis), YAxisOpinionType(yAxis),
      XCentre(xCentre), NegInvXRadius(1.0f / (xCentre - xMin)), PosInvXRadius(1.0f / (xMax - xCentre)),
      YCentre(yCentre), NegInvYRadius(1.0f / (yCentre - yMin)), PosInvYRadius(1.0f / (yMax - yCentre))
{
}