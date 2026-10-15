// behaviour test for 0x0045EE9B COpinionSourceDef::operator=
#include <cstdio>
static int g_copies = 0;
struct CParentDefClassBase {
    int baseTag;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { baseTag = o.baseTag; return *this; }
};
struct CBox { int v; CBox& operator=(const CBox& o) { v = o.v; ++g_copies; return *this; } };
struct COpinionSourceDef : CParentDefClassBase {
    CBox ProducedOpinion;
    bool AlwaysAwareOf, UnknownThreatSource, ReactNonstop, ReactToSelfPermitted, ReactionFlagDefault;
    CBox ReactionFlag;
    bool UseNavLineOfSight, WitnessesWillNotAttack, WitnessesWillOverrideFactionEnemy, BypassesPersonality, RelativeEffect;
    long RestrictToFactionAndAllies; bool RestrictToFactionIsReversed;
    CBox OpinionProduced;
    bool ReactionFlags[79];
    COpinionSourceDef& operator=(const COpinionSourceDef& other)
    {
        CParentDefClassBase::operator=(other);
        ProducedOpinion = other.ProducedOpinion;
        AlwaysAwareOf = other.AlwaysAwareOf;
        UnknownThreatSource = other.UnknownThreatSource;
        ReactNonstop = other.ReactNonstop;
        ReactToSelfPermitted = other.ReactToSelfPermitted;
        ReactionFlagDefault = other.ReactionFlagDefault;
        ReactionFlag = other.ReactionFlag;
        UseNavLineOfSight = other.UseNavLineOfSight;
        WitnessesWillNotAttack = other.WitnessesWillNotAttack;
        WitnessesWillOverrideFactionEnemy = other.WitnessesWillOverrideFactionEnemy;
        BypassesPersonality = other.BypassesPersonality;
        RelativeEffect = other.RelativeEffect;
        RestrictToFactionAndAllies = other.RestrictToFactionAndAllies;
        RestrictToFactionIsReversed = other.RestrictToFactionIsReversed;
        OpinionProduced = other.OpinionProduced;
        for (long i = 0; i < 79; ++i)
            ReactionFlags[i] = other.ReactionFlags[i];
        return *this;
    }
};
int main()
{
    static COpinionSourceDef a, b;
    for (int i = 0; i < 79; ++i) { b.ReactionFlags[i] = (i % 3) == 0; a.ReactionFlags[i] = false; }
    b.baseTag = 6; b.ReactNonstop = true; b.RelativeEffect = true; b.RestrictToFactionAndAllies = 12; b.OpinionProduced.v = 77;
    COpinionSourceDef* r = &(a = b);
    int bad = 0;
    for (int j = 0; j < 79; ++j) if (a.ReactionFlags[j] != ((j % 3) == 0)) bad = 1;
    if (r != &a || bad || a.baseTag != 6 || !a.ReactNonstop || !a.RelativeEffect || a.RestrictToFactionAndAllies != 12
        || a.OpinionProduced.v != 77 || g_copies != 3)
    { std::printf("0045ee9b_TEST FAIL\n"); return 1; }
    std::printf("0045ee9b_TEST PASS\n");
    return 0;
}