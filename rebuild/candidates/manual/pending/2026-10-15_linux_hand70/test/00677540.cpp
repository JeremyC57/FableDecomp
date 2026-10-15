// Behaviour test for the CReactionMatch_SecondaryAxisMRShifted constructor @ 0x00677540: the inverse interval is derived.
#include <cstdio>

enum EOpinionReactionType { OPINION_REACTION_TYPE_NONE = 0, OPINION_REACTION_TYPE_FEAR = 3 };
enum EOpinion { OPINION_NONE = 0, OPINION_MORALITY = 2 };

struct CReactionMatch {
    EOpinionReactionType ReactionType;
    CReactionMatch(EOpinionReactionType type) : ReactionType(type) {}
    virtual long GetMatchType() const { return 0; }
    virtual ~CReactionMatch() {}
};

struct CReactionMatch_SecondaryAxisMRShifted : CReactionMatch {
    EOpinion Axis; float LowerBoundOnAxis, InvIntervalOnAxis, MShiftZero, MShiftWeight, RShiftZero, RShiftWeight;
    CReactionMatch_SecondaryAxisMRShifted(EOpinionReactionType type, EOpinion axis, float lowerBound, float upperBound,
                                          float mShiftZero, float mShiftWeight, float rShiftZero, float rShiftWeight)
        : CReactionMatch(type), Axis(axis), LowerBoundOnAxis(lowerBound), InvIntervalOnAxis(1.0f / (upperBound - lowerBound)),
          MShiftZero(mShiftZero), MShiftWeight(mShiftWeight), RShiftZero(rShiftZero), RShiftWeight(rShiftWeight) {}
    virtual long GetMatchType() const { return 6; }
};

int main()
{
    CReactionMatch_SecondaryAxisMRShifted m(OPINION_REACTION_TYPE_FEAR, OPINION_MORALITY, -1.0f, 3.0f, 1.0f, 2.0f, 3.0f, 4.0f);
    if (m.ReactionType != OPINION_REACTION_TYPE_FEAR || m.Axis != OPINION_MORALITY || m.LowerBoundOnAxis != -1.0f || m.InvIntervalOnAxis != 0.25f ||
        m.MShiftZero != 1.0f || m.MShiftWeight != 2.0f || m.RShiftZero != 3.0f || m.RShiftWeight != 4.0f) {
        std::printf("00677540_TEST FAIL\n"); return 1;
    }
    std::printf("00677540_TEST PASS\n");
    return 0;
}
