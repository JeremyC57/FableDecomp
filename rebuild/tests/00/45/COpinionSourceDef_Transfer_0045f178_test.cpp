// Behaviour test for COpinionSourceDef::Transfer @ 0x0045F178: saving transfers every flag and both flattened arrays; loading rebuilds them from the maps.
#include <cstdio>
#include <cstring>
#include <map>

enum EOpinion { OPINION_COUNT = 5 };
enum EOpinionReactionType { REACTION_COUNT = 0x4f };
class CPersistContext { public:
    long Mode;
    int maps, bools, longs, floats;
    CPersistContext(long m) : Mode(m), maps(0), bools(0), longs(0), floats(0) {}
    void Transfer(std::map<EOpinion, float>&) { ++maps; }
    void Transfer(std::map<EOpinionReactionType, bool>&) { ++maps; }
    void Transfer(bool&) { ++bools; }
    void Transfer(long&) { ++longs; }
    void Transfer(float&) { ++floats; }
};
class COpinionSet { public: float* Values; };
class COpinionSourceDef { public:
    std::map<EOpinion, float> ProducedOpinion;
    bool AlwaysAwareOf, UnknownThreatSource, ReactNonstop, ReactToSelfPermitted, ReactionFlagDefault;
    std::map<EOpinionReactionType, bool> ReactionFlag;
    bool UseNavLineOfSight, WitnessesWillNotAttack, WitnessesWillOverrideFactionEnemy, BypassesPersonality, RelativeEffect;
    long RestrictToFactionAndAllies;
    bool RestrictToFactionIsReversed;
    COpinionSet OpinionProduced;
    bool ReactionFlags[0x4f];
    void Transfer(CPersistContext& context);
};

void COpinionSourceDef::Transfer(CPersistContext& context)
{
    context.Transfer(ProducedOpinion);
    context.Transfer(AlwaysAwareOf);
    context.Transfer(UnknownThreatSource);
    context.Transfer(ReactNonstop);
    context.Transfer(ReactToSelfPermitted);
    context.Transfer(ReactionFlagDefault);
    context.Transfer(ReactionFlag);
    context.Transfer(UseNavLineOfSight);
    context.Transfer(WitnessesWillNotAttack);
    context.Transfer(WitnessesWillOverrideFactionEnemy);
    context.Transfer(BypassesPersonality);
    context.Transfer(RelativeEffect);
    context.Transfer(RestrictToFactionAndAllies);
    context.Transfer(RestrictToFactionIsReversed);
    bool saving = context.Mode == 2 || context.Mode == 3;
    if (saving) {
        unsigned long i;
        for (i = 0; i < REACTION_COUNT; ++i)
            context.Transfer(ReactionFlags[i]);
        for (i = 0; i < OPINION_COUNT; ++i)
            context.Transfer(OpinionProduced.Values[i]);
    } else {
        float values[OPINION_COUNT];
        memset(values, 0, sizeof(values));
        bool opinionSet[OPINION_COUNT];
        std::map<EOpinion, float>::iterator end = ProducedOpinion.end();
        for (std::map<EOpinion, float>::iterator it = ProducedOpinion.begin(); it != end; ++it) {
            values[it->first] = it->second;
            opinionSet[it->first] = true;
        }
        unsigned long i;
        for (i = 0; i < OPINION_COUNT; ++i)
            OpinionProduced.Values[i] = values[i];
        for (i = 0; i < REACTION_COUNT; ++i)
            ReactionFlags[i] = i < 0x12 ? false : ReactionFlagDefault;
        bool reactionSet[REACTION_COUNT];
        std::map<EOpinionReactionType, bool>::iterator rend = ReactionFlag.end();
        for (std::map<EOpinionReactionType, bool>::iterator r = ReactionFlag.begin(); r != rend; ++r) {
            reactionSet[r->first] = true;
            ReactionFlags[r->first] = r->second;
        }
        (void)opinionSet; (void)reactionSet;
    }
}

int main()
{
    int ok = 1;
    float store[5] = { 9, 9, 9, 9, 9 };
    COpinionSourceDef def;
    def.OpinionProduced.Values = store;
    def.ReactionFlagDefault = true;
    def.ProducedOpinion[(EOpinion)1] = 2.5f;
    def.ProducedOpinion[(EOpinion)4] = -1.0f;
    def.ReactionFlag[(EOpinionReactionType)3] = true;
    def.ReactionFlag[(EOpinionReactionType)0x20] = false;
    CPersistContext save(2);
    def.Transfer(save);
    ok &= save.maps == 2 && save.bools == 11 + 0x4f && save.longs == 1 && save.floats == 5;
    ok &= store[0] == 9.0f;                         // saving leaves the arrays alone
    CPersistContext load(0);
    def.Transfer(load);
    ok &= load.bools == 11 && load.floats == 0;
    ok &= store[0] == 0.0f && store[1] == 2.5f && store[4] == -1.0f && store[2] == 0.0f;
    ok &= def.ReactionFlags[0] == false && def.ReactionFlags[0x11] == false && def.ReactionFlags[0x12] == true;
    ok &= def.ReactionFlags[3] == true && def.ReactionFlags[0x20] == false && def.ReactionFlags[0x4e] == true;
    printf("0045f178_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}