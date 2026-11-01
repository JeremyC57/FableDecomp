// Test for ~COpinionReactionManagerDef 0x00678460: the user body deletes every owned CReactionMatch and empties the list;
// the implicit members then free the traits pointers. Stand-ins count destructor calls.
#include <stdio.h>
#include <list>
#include <map>
#include <vector>
static int g_matchesDeleted = 0, g_traitsDeleted = 0;
enum EOpinionReactionType { REACTION_NONE };
enum EOpinionAttitudeType { ATTITUDE_NONE };
enum EOpinionTargetingConditionType { TARGET_NONE };
class CReactionMatch { public: virtual ~CReactionMatch() { ++g_matchesDeleted; } EOpinionReactionType ReactionType; };
class CReactionFrequencyTraits { public: virtual ~CReactionFrequencyTraits() {} };
class CReactionFrequencyTraitsArray { public:
    CReactionFrequencyTraits* LowHigh[2][79];
    ~CReactionFrequencyTraitsArray() { for (unsigned long i = 0; i < 79; ++i) { if (LowHigh[0][i]) ++g_traitsDeleted; delete LowHigh[0][i]; if (LowHigh[1][i]) ++g_traitsDeleted; delete LowHigh[1][i]; } }
};
class CBlurredAttitudesDef { public: virtual ~CBlurredAttitudesDef() {} std::vector<EOpinionAttitudeType> Attitudes; };
class COpinionReactionManagerDef {
public:
    std::vector<CBlurredAttitudesDef> AttitudeBlur;
    std::map<EOpinionReactionType, EOpinionAttitudeType> AttitudeCondition;
    std::map<EOpinionReactionType, EOpinionTargetingConditionType> TargetingCondition;
    std::map<EOpinionReactionType, float> PreReactionDelay;
    std::map<EOpinionAttitudeType, float> ToleranceToBeingHit;
    std::map<EOpinionReactionType, bool> BlockFurtherReactions, AllowSpeechOnNonPureAISpeaker, AllowWhileCarrying, AllowWhileFollowingPlayer;
    std::list<CReactionMatch*> Matches;
    CReactionFrequencyTraitsArray FrequencyTraitsArray;
    CReactionFrequencyTraits** FrequencyTraits;
    CReactionFrequencyTraits** FrequencyTraitsHigh;
    bool* ClearedFlag;
    virtual ~COpinionReactionManagerDef();
};
COpinionReactionManagerDef::~COpinionReactionManagerDef()
{
    std::list<CReactionMatch*>::iterator end = Matches.end();
    for (std::list<CReactionMatch*>::iterator it = Matches.begin(); it != end; ++it)
        delete *it;
    Matches.clear();
    *ClearedFlag = Matches.empty();
}
int main()
{
    bool cleared = false;
    {
        COpinionReactionManagerDef* def = new COpinionReactionManagerDef;
        def->ClearedFlag = &cleared;
        for (int i = 0; i < 3; ++i) def->Matches.push_back(new CReactionMatch);
        for (int i = 0; i < 79; ++i) { def->FrequencyTraitsArray.LowHigh[0][i] = (i % 2) ? new CReactionFrequencyTraits : 0; def->FrequencyTraitsArray.LowHigh[1][i] = new CReactionFrequencyTraits; }
        def->AttitudeBlur.resize(2);
        def->BlockFurtherReactions[REACTION_NONE] = true;
        def->PreReactionDelay[REACTION_NONE] = 1.0f;
        delete def;
    }
    int ok = cleared && g_matchesDeleted == 3 && g_traitsDeleted == 79 + 39;
    printf(ok ? "00678460_TEST PASS\n" : "00678460_TEST FAIL\n");
    return ok ? 0 : 1;
}