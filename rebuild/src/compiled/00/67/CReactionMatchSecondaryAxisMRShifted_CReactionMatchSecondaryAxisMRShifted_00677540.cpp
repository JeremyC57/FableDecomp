// CReactionMatch_SecondaryAxisMRShifted::CReactionMatch_SecondaryAxisMRShifted(EOpinionReactionType type, EOpinion axis, float lowerBound, float upperBound, float mShiftZero, float mShiftWeight, float rShiftZero, float rShiftWeight) -- store the reaction type, the axis, the lower bound and the inverse interval, and the four M/R shift terms
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00677540, length 77 bytes (`ret 20h`: eight by-value arguments).
// Vtable: CReactionMatch_SecondaryAxisMRShifted 0x0125B6E8. Constants: 1.0f @ 0x0122DED8. No callees.
// Layout: PDB names (CReactionMatch::ReactionType +0x04, Axis +0x08, LowerBoundOnAxis +0x0c, InvIntervalOnAxis
// +0x10, MShiftZero +0x14, MShiftWeight +0x18, RShiftZero +0x1c, RShiftWeight +0x20) -- the offsets agree with
// engine/CReactionMatch_SecondaryAxisMRShifted.h, but a constructor needs the real vtable, which the generated
// header (a `void* __vftable` layout struct) cannot carry, so the class is modelled locally.
// Shape: the upper bound is never stored -- only `1.0f / (upper - lower)` (`fld upper ; fsub lower ; fdivr 1.0`,
// stored last); every other argument is copied with plain moves, the inline CReactionMatch base constructor's
// vtable store is dead and the derived vtable store is scheduled among the member stores.
// Leaf: ??0CReactionMatch_SecondaryAxisMRShifted
enum EOpinionReactionType { OPINION_REACTION_TYPE_NONE = 0 };
enum EOpinion { OPINION_NONE = 0 };

class CReactionMatch { public:
    EOpinionReactionType ReactionType;                  // +0x04
    CReactionMatch(EOpinionReactionType type) : ReactionType(type) {}
    virtual long GetMatchType() const;
};

struct CReactionMatch_SecondaryAxisMRShifted : CReactionMatch {
    EOpinion Axis;                                      // +0x08
    float    LowerBoundOnAxis;                          // +0x0c
    float    InvIntervalOnAxis;                         // +0x10
    float    MShiftZero;                                // +0x14
    float    MShiftWeight;                              // +0x18
    float    RShiftZero;                                // +0x1c
    float    RShiftWeight;                              // +0x20
    CReactionMatch_SecondaryAxisMRShifted(EOpinionReactionType type, EOpinion axis, float lowerBound, float upperBound,
                                          float mShiftZero, float mShiftWeight, float rShiftZero, float rShiftWeight);
    virtual long GetMatchType() const;
};

CReactionMatch_SecondaryAxisMRShifted::CReactionMatch_SecondaryAxisMRShifted(
        EOpinionReactionType type, EOpinion axis, float lowerBound, float upperBound,
        float mShiftZero, float mShiftWeight, float rShiftZero, float rShiftWeight)
    : CReactionMatch(type), Axis(axis), LowerBoundOnAxis(lowerBound), InvIntervalOnAxis(1.0f / (upperBound - lowerBound)),
      MShiftZero(mShiftZero), MShiftWeight(mShiftWeight), RShiftZero(rShiftZero), RShiftWeight(rShiftWeight)
{
}