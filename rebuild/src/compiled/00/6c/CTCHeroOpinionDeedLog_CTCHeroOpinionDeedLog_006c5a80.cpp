// Cache each deed's definitions and reaction data for the world.
// Leaf: ??0CTCHeroOpinionDeedLog@@QAE@AAVCThing@@@Z
// Flags:
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDREACTIONS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_VSLOTS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_ONSERIALISE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_CTOR_PASS27
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_FORWARDER
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "stlport/fable_map.h"
#include "engine/EOpinionAttitudeType.h"
#include "engine/COpinionDeedReaction.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionReactionManager.h"
#include "engine/COpinionDeedReactionDef.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NProgressDisplay.h"
// Descriptive: GOpinionDeedDefinitionNames, the 92-entry deed definition table at 0x0137FBC0.
extern const char* GOpinionDeedDefinitionNames[OPINION_DEED_TYPE_LAST];

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