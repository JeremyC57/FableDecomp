#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include "engine/EOpinionDeedType.h"
#include "engine/EOpinionAttitudeType.h"

struct COpinionDeedReactionDef
{
    std::vector<EOpinionAttitudeType> Attitudes;
    std::string Animation;
    unsigned long DialogueTag;
};
struct COpinionDeedEffectsDef
{
    bool AllowSimultaneousReactionDialogue;
    std::vector<COpinionDeedReactionDef> Reaction;
};
struct COpinionDeedReaction
{
    std::string Animations[OPINION_ATTITUDE_TYPE_LAST];
    unsigned long DialogueTags[OPINION_ATTITUDE_TYPE_LAST];
    COpinionDeedReaction()
    {
        for (long i = 0; i < OPINION_ATTITUDE_TYPE_LAST; ++i) {
            Animations[i] = "old animation";
            DialogueTags[i] = 443;
        }
    }
};
template<class T> class CDefPointer
{
public:
    CDefPointer() : p(0) {}
    const T* operator->() const { return p; }
    const T* p;
};
struct COpinionReactionManager
{
    long DeedReactionSpeechPlaying[OPINION_DEED_TYPE_LAST];
};
struct CWorld
{
    COpinionReactionManager ReactionManager;
    COpinionReactionManager& GetOpinionReactionManager() { return ReactionManager; }
};
const char* GOpinionDeedDefinitionNames[OPINION_DEED_TYPE_LAST];

struct CGameDefinitionManager
{
    COpinionDeedEffectsDef defs[OPINION_DEED_TYPE_LAST];
    long namesRead;
    long definitionsRead;
    bool valid;
    CGameDefinitionManager() : namesRead(0), definitionsRead(0), valid(true) {}
    long GetDefGlobalIndexFromName(const char* name)
    {
        unsigned long i;
        if (sscanf(name, "TEST_DEED_%lu", &i) != 1 || i != (unsigned long)namesRead)
            valid = false;
        ++namesRead;
        return 300 + i;
    }
    void GetDef(unsigned short i, CDefPointer<const COpinionDeedEffectsDef>* out)
    {
        if (i != 301 + definitionsRead) valid = false;
        ++definitionsRead;
        out->p = &defs[i - 300];
    }
};
struct CThing
{
    CWorld world;
    CGameDefinitionManager definitions;
};
class CTCBase
{
public:
    CTCBase(CThing& thing) : Owner(thing) {}
    CThing& Owner;
    CWorld& GetWorld() { return Owner.world; }
    CGameDefinitionManager& GetDefinitionManager() const { return Owner.definitions; }
};
struct CTCHeroOpinionDeedLog : CTCBase
{
    CTCHeroOpinionDeedLog(CThing& thing);
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType type) const;
    long NextHandle;
    long WeaponOutDeedHandle;
    bool WeaponOutCrimeEnabled;
    bool GuardsIgnoreCrimes;
    unsigned short DeedEffectsDefIndices[OPINION_DEED_TYPE_LAST];
    COpinionDeedReaction DeedReactions[OPINION_DEED_TYPE_LAST];
};
namespace NProgressDisplay
{
    long outer = 0;
    long inner = 0;
    bool valid = true;
    void DisplayProgress(const char* label, float progress, bool first, bool second)
    {
        if (progress != -1.0f || first || second) valid = false;
        if (!strcmp(label, "CTCHeroOpinionDeedLog 1")) ++outer;
        else if (!strcmp(label, "CTCHeroOpinionDeedLog 3")) ++inner;
        else valid = false;
    }
}

CDefPointer<const COpinionDeedEffectsDef> CTCHeroOpinionDeedLog::PeekPDeedEffectsDef(EOpinionDeedType type) const
{
    CDefPointer<const COpinionDeedEffectsDef> pdef;
    GetDefinitionManager().GetDef(DeedEffectsDefIndices[type], &pdef);
    return pdef;
}

CTCHeroOpinionDeedLog::CTCHeroOpinionDeedLog(CThing& thing)
    : CTCBase(thing), NextHandle(0)
{
    COpinionReactionManager& reaction_mgr = GetWorld().GetOpinionReactionManager();
    for (long c1 = 0; c1 < OPINION_DEED_TYPE_LAST; ++c1) {
        NProgressDisplay::DisplayProgress("CTCHeroOpinionDeedLog 1", -1.0f, false, false);
        DeedEffectsDefIndices[c1] = (unsigned short)GetDefinitionManager().GetDefGlobalIndexFromName(GOpinionDeedDefinitionNames[c1]);
        if (c1 == OPINION_DEED_TYPE_NONE)
            continue;
        CDefPointer<const COpinionDeedEffectsDef> pdeed_effects_def;
        pdeed_effects_def = PeekPDeedEffectsDef((EOpinionDeedType)c1);
        if (pdeed_effects_def->AllowSimultaneousReactionDialogue)
            reaction_mgr.DeedReactionSpeechPlaying[c1] = -1;
        for (long c2 = 0; c2 < OPINION_ATTITUDE_TYPE_LAST; ++c2) {
            DeedReactions[c1].Animations[c2] = "";
            DeedReactions[c1].DialogueTags[c2] = 0;
            bool found_current_attitude = false;
            std::vector<COpinionDeedReactionDef>::const_iterator it = pdeed_effects_def->Reaction.begin();
            std::vector<COpinionDeedReactionDef>::const_iterator end_it = pdeed_effects_def->Reaction.end();
            for (; it != end_it; ++it) {
                std::vector<EOpinionAttitudeType>::const_iterator it2 = it->Attitudes.begin();
                std::vector<EOpinionAttitudeType>::const_iterator end_it2 = it->Attitudes.end();
                for (; it2 != end_it2; ++it2) {
                    NProgressDisplay::DisplayProgress("CTCHeroOpinionDeedLog 3", -1.0f, false, false);
                    if (*it2 == c2 && !found_current_attitude) {
                        DeedReactions[c1].Animations[c2] = it->Animation;
                        DeedReactions[c1].DialogueTags[c2] = it->DialogueTag;
                        found_current_attitude = true;
                    }
                }
            }
        }
    }
    WeaponOutDeedHandle = -1;
    WeaponOutCrimeEnabled = true;
    GuardsIgnoreCrimes = false;
}

static COpinionDeedReactionDef Reaction(const char* animation, unsigned long tag)
{
    COpinionDeedReactionDef reaction;
    reaction.Animation = animation;
    reaction.DialogueTag = tag;
    return reaction;
}
#define CHECK(expr) do { if (!(expr)) { printf("006c5a80_TEST failure at %d\n", __LINE__); return 1; } } while (0)
int main()
{
    CThing thing;
    char names[OPINION_DEED_TYPE_LAST][32];
    for (long i = 0; i < OPINION_DEED_TYPE_LAST; ++i) {
        sprintf(names[i], "TEST_DEED_%ld", i);
        GOpinionDeedDefinitionNames[i] = names[i];
        thing.world.ReactionManager.DeedReactionSpeechPlaying[i] = i + 5;
        thing.definitions.defs[i].AllowSimultaneousReactionDialogue = (i % 3 == 0);
    }
    COpinionDeedReactionDef none = Reaction("none must be skipped", 999);
    none.Attitudes.push_back((EOpinionAttitudeType)0);
    thing.definitions.defs[0].Reaction.push_back(none);
    COpinionDeedReactionDef first = Reaction("first", 20);
    first.Attitudes.push_back((EOpinionAttitudeType)0);
    first.Attitudes.push_back((EOpinionAttitudeType)2);
    first.Attitudes.push_back((EOpinionAttitudeType)0);
    COpinionDeedReactionDef second = Reaction("second", 22);
    second.Attitudes.push_back((EOpinionAttitudeType)0);
    second.Attitudes.push_back((EOpinionAttitudeType)1);
    thing.definitions.defs[1].Reaction.push_back(Reaction("no attitudes", 33));
    thing.definitions.defs[1].Reaction.push_back(first);
    thing.definitions.defs[1].Reaction.push_back(second);
    COpinionDeedReactionDef last = Reaction("last", 1);
    last.Attitudes.push_back((EOpinionAttitudeType)(OPINION_ATTITUDE_TYPE_LAST - 1));
    thing.definitions.defs[OPINION_DEED_TYPE_LAST - 1].Reaction.push_back(last);
    CTCHeroOpinionDeedLog log(thing);
    CHECK(&log.Owner == &thing);
    CHECK(thing.definitions.valid);
    CHECK(thing.definitions.namesRead == OPINION_DEED_TYPE_LAST);
    CHECK(thing.definitions.definitionsRead == OPINION_DEED_TYPE_LAST - 1);
    CHECK(NProgressDisplay::valid);
    CHECK(NProgressDisplay::outer == OPINION_DEED_TYPE_LAST);
    CHECK(NProgressDisplay::inner == OPINION_ATTITUDE_TYPE_LAST * 6);
    for (long i = 0; i < OPINION_DEED_TYPE_LAST; ++i) {
        CHECK(log.DeedEffectsDefIndices[i] == 300 + i);
        CHECK(thing.world.ReactionManager.DeedReactionSpeechPlaying[i] ==
              (i != OPINION_DEED_TYPE_NONE && i % 3 == 0 ? -1 : i + 5));
    }
    CHECK(log.DeedReactions[0].Animations[0] == "old animation");
    CHECK(log.DeedReactions[0].DialogueTags[0] == 443);
    CHECK(log.DeedReactions[1].Animations[0] == "first" && log.DeedReactions[1].DialogueTags[0] == 20);
    CHECK(log.DeedReactions[1].Animations[1] == "second" && log.DeedReactions[1].DialogueTags[1] == 22);
    CHECK(log.DeedReactions[1].Animations[2] == "first" && log.DeedReactions[1].DialogueTags[2] == 20);
    CHECK(log.DeedReactions[1].Animations[3].empty() && log.DeedReactions[1].DialogueTags[3] == 0);
    CHECK(log.DeedReactions[2].Animations[0].empty() && log.DeedReactions[2].DialogueTags[0] == 0);
    CHECK(log.DeedReactions[OPINION_DEED_TYPE_LAST - 1].Animations[OPINION_ATTITUDE_TYPE_LAST - 1] == "last");
    CHECK(log.DeedReactions[OPINION_DEED_TYPE_LAST - 1].DialogueTags[OPINION_ATTITUDE_TYPE_LAST - 1] == 1);
    CHECK(log.NextHandle == 0);
    CHECK(log.WeaponOutDeedHandle == -1);
    CHECK(log.WeaponOutCrimeEnabled);
    CHECK(!log.GuardsIgnoreCrimes);
    puts("006c5a80_TEST PASS");
    return 0;
}