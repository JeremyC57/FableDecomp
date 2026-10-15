// Behaviour test for the CReactionMatch_SecondaryAxisCentredMRShifted constructor @ 0x006775B0.
#include <cstdio>

enum EOpinionReactionType { OPINION_REACTION_TYPE_NONE = 0, OPINION_REACTION_TYPE_FEAR = 3 };
enum EOpinion { OPINION_NONE = 0, OPINION_MORALITY = 2 };

struct CReactionMatch {
    EOpinionReactionType ReactionType;
    CReactionMatch(EOpinionReactionType type) : ReactionType(type) {}
    virtual long GetMatchType() const { return 0; }
    virtual ~CReactionMatch() {}
};

struct CReactionMatch_SecondaryAxisCentredMRShifted : CReactionMatch {
    EOpinion Axis; float CentreOnAxis, RadiusOnAxis, MShiftZero, MShiftWeight, RShiftZero, RShiftWeight;
    CReactionMatch_SecondaryAxisCentredMRShifted(EOpinionReactionType type, EOpinion axis, float centre, float radius,
                                                 float mShiftZero, float mShiftWeight, float rShiftZero, float rShiftWeight)
        : CReactionMatch(type), Axis(axis), CentreOnAxis(centre), RadiusOnAxis(radius),
          MShiftZero(mShiftZero), MShiftWeight(mShiftWeight), RShiftZero(rShiftZero), RShiftWeight(rShiftWeight) {}
    virtual long GetMatchType() const { return 7; }
};

int main()
{
    CReactionMatch_SecondaryAxisCentredMRShifted m(OPINION_REACTION_TYPE_FEAR, OPINION_MORALITY, 0.5f, 0.25f, 1.0f, 2.0f, 3.0f, 4.0f);
    const CReactionMatch& base = m;
    if (m.ReactionType != OPINION_REACTION_TYPE_FEAR || m.Axis != OPINION_MORALITY || m.CentreOnAxis != 0.5f || m.RadiusOnAxis != 0.25f ||
        m.MShiftZero != 1.0f || m.MShiftWeight != 2.0f || m.RShiftZero != 3.0f || m.RShiftWeight != 4.0f || base.GetMatchType() != 7) {
        std::printf("006775b0_TEST FAIL\n"); return 1;
    }
    std::printf("006775b0_TEST PASS\n");
    return 0;
}