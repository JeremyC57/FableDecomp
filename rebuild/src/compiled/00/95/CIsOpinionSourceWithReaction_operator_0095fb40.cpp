// 0095fb40 CIsOpinionSourceWithReaction::operator()
// Leaf: ??RCIsOpinionSourceWithReaction@@QBE_NABVCThing@@@Z
// Flags:
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_OPINION_FILTER_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_THINGREACTINGTO
#define FABLE_RETAIL_CTCOPINIONSOURCE
#define FABLE_RETAIL_CTCOPINIONSOURCE_AWARENESS_PEEKS
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_THREATSOURCE
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_DEEDREACTIONS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_NCREATUREAI_AWARE_INDIRECTLY
#define FABLE_RETAIL_STLP_LIST
#include "engine/COpinionReactionBehaviour.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCOpinionSource.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CIsOpinionSourceWithReaction.h"
#include "engine/CAIStateGroup_OpinionReactionRoot.h"
#include "engine/CAIBrain.h"
#include "engine/COpinionReactionManager.h"
#include "engine/NCreatureAI.h"

bool CIsOpinionSourceWithReaction::operator()(const CThing& test_thing) const
{
    const CTCOpinionSource* ptc_source;
    if (test_thing.PeekTC(&ptc_source) &&
        (ptc_source->IsReactToSelfPermitted() || test_thing.GetUniqueID()!=PStateGroup->GetCreature().GetUniqueID())) {
        PStateGroup->ThingReactingTo=const_cast<CThing*>(&test_thing);
        for (unsigned long c1=0;c1<OPINION_REACTION_TYPE_LAST;++c1) {
            PStateGroup->ReactionFlags[c1]=c1>=OPINION_ATTITUDE_TYPE_LAST &&
                (!IsFollowingPlayer || ReactionManager->GetAllowWhileFollowingPlayer((EOpinionReactionType)c1));
            if (PStateGroup->ReactionFlags[c1]) {
                bool (CAIStateGroup_OpinionReactionRoot::*is_runnable_func)()=PStateGroup->Behaviours[c1].IsRunnableFunc;
                PStateGroup->ReactionFlags[c1]=(is_runnable_func && !(PStateGroup->*is_runnable_func)()) ? false : true;
            }
        }
        Reaction=ReactionManager->GetReactionType(PStateGroup->GetCreature(),PStateGroup->ReactionFlags,ptc_source);
        if (Reaction!=OPINION_REACTION_TYPE_NONE) {
            if (ptc_source->IsUnknownThreatSource() && (IsAwareOfPlayerForReactions || !(PTCOpinion &&
                PTCOpinion->PeekUnknownThreatOpinionSource()==&ptc_source->GetParentThing()))) return false;
            if (ptc_source->IsAlwaysAwareOf()) return true;
            if (NCreatureAI::IsAwareOfThingIndirectly(&PStateGroup->GetCreature(),&test_thing,ptc_source->PeekUseNavLineOfSight())) return true;
        }
    }
    return false;
}