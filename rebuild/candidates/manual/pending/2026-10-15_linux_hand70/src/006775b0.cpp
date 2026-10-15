// CReactionMatch_SecondaryAxisCentredMRShifted::CReactionMatch_SecondaryAxisCentredMRShifted(EOpinionReactionType type, EOpinion axis, float centre, float radius, float mShiftZero, float mShiftWeight, float rShiftZero, float rShiftWeight) -- store the reaction type, the axis, its centre/radius and the four M/R shift terms
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006775B0, length 67 bytes (`ret 20h`: eight by-value arguments).
// Vtable: CReactionMatch_SecondaryAxisCentredMRShifted 0x0125B700. No callees.
// Layout: PDB names (CReactionMatch::ReactionType +0x04, Axis +0x08, CentreOnAxis +0x0c, RadiusOnAxis +0x10,
// MShiftZero +0x14, MShiftWeight +0x18, RShiftZero +0x1c, RShiftWeight +0x20) -- the offsets agree with
// engine/CReactionMatch_SecondaryAxisCentredMRShifted.h, but a constructor needs the real vtable, which the
// generated header (a `void* __vftable` layout struct) cannot carry, so the class is modelled locally.
// Shape: the CReactionMatch base constructor is inline (its own vtable store is dead and dropped), every argument
// is copied through eax/ecx/edx with plain moves (no FPU: the floats are only copied), and the derived vtable store
// is scheduled among the member stores.
// Leaf: ??0CReactionMatch_SecondaryAxisCentredMRShifted
enum EOpinionReactionType { OPINION_REACTION_TYPE_NONE = 0 };
enum EOpinion { OPINION_NONE = 0 };

struct CReactionMatch {
    EOpinionReactionType ReactionType;                  // +0x04
    CReactionMatch(EOpinionReactionType type) : ReactionType(type) {}
    virtual long GetMatchType() const;
};

struct CReactionMatch_SecondaryAxisCentredMRShifted : CReactionMatch {
    EOpinion Axis;                                      // +0x08
    float    CentreOnAxis;                              // +0x0c
    float    RadiusOnAxis;                              // +0x10
    float    MShiftZero;                                // +0x14
    float    MShiftWeight;                              // +0x18
    float    RShiftZero;                                // +0x1c
    float    RShiftWeight;                              // +0x20
    CReactionMatch_SecondaryAxisCentredMRShifted(EOpinionReactionType type, EOpinion axis, float centre, float radius,
                                                 float mShiftZero, float mShiftWeight, float rShiftZero, float rShiftWeight);
    virtual long GetMatchType() const;
};

CReactionMatch_SecondaryAxisCentredMRShifted::CReactionMatch_SecondaryAxisCentredMRShifted(
        EOpinionReactionType type, EOpinion axis, float centre, float radius,
        float mShiftZero, float mShiftWeight, float rShiftZero, float rShiftWeight)
    : CReactionMatch(type), Axis(axis), CentreOnAxis(centre), RadiusOnAxis(radius),
      MShiftZero(mShiftZero), MShiftWeight(mShiftWeight), RShiftZero(rShiftZero), RShiftWeight(rShiftWeight)
{
}
