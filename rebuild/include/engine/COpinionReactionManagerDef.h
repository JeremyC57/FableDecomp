#ifndef FABLE_ENGINE_COPINIONREACTIONMANAGERDEF_H
#define FABLE_ENGINE_COPINIONREACTIONMANAGERDEF_H
#ifdef FABLE_RETAIL_OPINION_STRENGTH_PASS26
#include "engine/CParentDefClassBase.h"
#include "engine/EOpinionAttitudeType.h"
#include "engine/EOpinionReactionType.h"
#include "engine/EOpinionTargetingConditionType.h"
#include "engine/CReactionFrequencyTraitsArray.h"
#include "stlport/fable_map.h"
class CReactionMatch;
class CBlurredAttitudesDef;
// Both original type records supply these members. Retail addresses Matches and FrequencyTraits at +A8 and +324.
class COpinionReactionManagerDef : public CParentDefClassBase {
public:
    std::vector<CBlurredAttitudesDef> AttitudeBlur;
    std::map<EOpinionReactionType,EOpinionAttitudeType> AttitudeCondition;
    std::map<EOpinionReactionType,EOpinionTargetingConditionType> TargetingCondition;
    std::map<EOpinionReactionType,float> PreReactionDelay;
    std::map<EOpinionAttitudeType,float> ToleranceToBeingHit;
    std::map<EOpinionReactionType,bool> BlockFurtherReactions, AllowSpeechOnNonPureAISpeaker, AllowWhileCarrying, AllowWhileFollowingPlayer;
    std::list<CReactionMatch*> Matches;
    CReactionFrequencyTraitsArray FrequencyTraitsArray;
    CReactionFrequencyTraits** FrequencyTraits;
    CReactionFrequencyTraits** FrequencyTraitsHigh;
};
FABLE_STATIC_ASSERT_AT(905,offsetof(COpinionReactionManagerDef,Matches)==0xa8);
FABLE_STATIC_ASSERT_AT(906,offsetof(COpinionReactionManagerDef,FrequencyTraits)==0x324);
#endif
#endif
