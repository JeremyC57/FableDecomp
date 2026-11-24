// Select the opinion reaction and install its update member.
// Leaf: ?IsRunnable@CAIStateGroup_OpinionReactionRoot@@UAE_NXZ
// Flags:
// Inferred: the awareness accessor's emitted copy uses the original size setting.
// Retail 0x00643B4D encodes true as xor eax,eax / inc eax, while a speed build
// emits mov eax,1. The scoped size setting preserves the expanded leaf.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_THINGREACTINGTO
#define FABLE_RETAIL_CAIBRAIN
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCFOLLOWED
#define FABLE_RETAIL_CTCREGIONFOLLOWER
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_THINGFLAGS
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_DEEDREACTIONS
#define FABLE_RETAIL_OPINION_STRENGTH_PASS26
#define FABLE_RETAIL_OPINIONREACTIONROOT_ISRUNNABLE_PASS27
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCFollowed.h"
#include "engine/CTCRegionFollower.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/EOpinionTargetingConditionType.h"
#include "engine/CIsOpinionSourceWithReaction.h"
#include "engine/COpinionReactionBehaviour.h"
class COpinionSet;
#include "engine/COpinionReactionManager.h"
#include "engine/CAIStateGroup_OpinionReactionRoot.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"

template<class T> bool FABLE_FASTCALL operator==(const CBaseObject*, const CIntelligentPointer<T>&);

bool CAIStateGroup_OpinionReactionRoot::IsRunnable()
{
    ExtraDataFloat = 0;
    ExtraDataSLONG = 0;
    ExtraDataPThing = 0;
    if (GetCreature().GetProperty() == 3)
        return false;
    const COpinionReactionManager& reaction_mgr = PeekWorld().PeekOpinionReactionManager();
    const CThing* pthing_opinion_source = reaction_mgr.PeekThingOpinionSource();
    CIsOpinionSourceWithReaction is_candidate(this);
    if (!pthing_opinion_source) {
        const std::list<CIntelligentPointer<const CThing> >& sources_list = PeekWorld().PeekOpinionReactionManager().PeekOpinionSourcesList();
        float best_sq_dist = 0.0f;
        for (std::list<CIntelligentPointer<const CThing> >::const_iterator it = sources_list.begin(); it != sources_list.end(); ++it) {
            CIntelligentPointer<const CThing> psource(*it);
            if (IS_THING_ALIVE(psource)) {
                float sq_dist = (GetCreature().GetPos() - psource->GetPos()).GetSquaredMagnitude();
                if ((!pthing_opinion_source || sq_dist < best_sq_dist) && is_candidate(*psource.Get()))
                    pthing_opinion_source = psource.Get();
            }
        }
    }
    if (pthing_opinion_source && is_candidate(*pthing_opinion_source) && is_candidate.PeekReaction() != 0) {
        CurrentReactionType = is_candidate.PeekReaction();
        CurrentUpdateFunc = Behaviours[CurrentReactionType].UpdateFunc;
        return true;
    }
    ThingReactingTo.Set(0);
    if (reaction_mgr.PeekThingOpinionSource())
        return false;
    {
        const CTCPerceiveThing* ptc_perception;
        GetCreature().PeekTC(&ptc_perception);
        const CThing* pperceived_thing = ptc_perception->GetPerceivedThing();
        if (IS_THING_ALIVE(pperceived_thing) && ptc_perception->IsAwareOfThing()) {
            if (pperceived_thing->HasTC(TCI_OPINION_SOURCE))
                return false;
            ThingReactingTo = const_cast<CThing*>(pperceived_thing);
        }
    }
    if (!IS_THING_ALIVE(ThingReactingTo))
        return false;
    // CThing class type 2 identifies a player creature.
    ReactingToPlayer = ThingReactingTo->GetType() == 2 && ThingReactingTo->HasTC(TCI_HERO_OPINION_DEED_LOG);
    const CTCFollowed* ptc_followed;
    const CTCRegionFollower* ptc_region_follow;
    bool is_following = (ThingReactingTo->PeekTC(&ptc_followed) && ptc_followed->IsFollower(&GetCreature())) ||
                        (ThingReactingTo->PeekTC(&ptc_region_follow) && ptc_region_follow->IsRegionFollower(&GetCreature()));
    if (ReactingToPlayer && is_following)
        return false;
    const CTCTargetingBase* ptc_targeting;
    bool targeted = false;
    bool z_targeted = false;
    if (ThingReactingTo->PeekTC(&ptc_targeting) && ptc_targeting->GetPTargetedThing() == &GetCreature()) {
        targeted = true;
        const CPlayer* pplayer = PeekWorld().PeekMyPlayer();
        if (pplayer && pplayer->GetPControlledCreature() == ThingReactingTo && pplayer->IsZTargeting())
            z_targeted = true;
    }
    const CTCCreatureModeManager* ptc_mode_manager;
    unsigned long mode_type = NCreatureMode::MODE_NULL;
    if (GetCreature().PeekTC(&ptc_mode_manager))
        mode_type = ptc_mode_manager->GetCurrentMode().GetType();
    for (long c1 = 0; c1 < 79; ++c1) {
        if (c1 < 18) {
            ReactionFlags[c1] = false;
            continue;
        }
        EOpinionReactionType r = (EOpinionReactionType)c1;
        switch (reaction_mgr.GetTargetingCondition(r)) {
        case OPINION_TARGETING_CONDITION_TYPE_NONE: ReactionFlags[c1] = true; break;
        case 1: ReactionFlags[c1] = !targeted; break;
        case 2: ReactionFlags[c1] = targeted && !z_targeted; break;
        case 3: ReactionFlags[c1] = !z_targeted; break;
        case 4: ReactionFlags[c1] = z_targeted; break;
        case 5: ReactionFlags[c1] = targeted; break;
        }
        if (!ReactionFlags[c1])
            continue;
        if (!reaction_mgr.GetAllowWhileCarrying(r) &&
            (mode_type == NCreatureMode::MODE_CARRYING_GOODS_STAND ||
             mode_type == NCreatureMode::MODE_HOLD_JUG ||
             mode_type == NCreatureMode::MODE_FILL_JUG ||
             mode_type == NCreatureMode::MODE_SERVE_CUSTOMER ||
             mode_type == NCreatureMode::MODE_ARMS_OFF_TABLE ||
             mode_type == NCreatureMode::MODE_SIT_TAVERN_STOOL)) {
            ReactionFlags[c1] = false;
            continue;
        }
        if (ReactionFlags[c1])
            ReactionFlags[c1] = (Behaviours[c1].IsRunnableFunc == 0 ||
                                 (this->*Behaviours[c1].IsRunnableFunc)()) ? true : false;
    }
    EOpinionReactionType reaction = reaction_mgr.GetReactionType(GetCreature(), ReactionFlags, 0);
    if (reaction != (EOpinionReactionType)0) {
        const CTCPerceiveThing* ptc_perception;
        GetCreature().PeekTC(&ptc_perception);
        if (ptc_perception->IsThingIgnoreFOVLineOfSight()) {
            CurrentReactionType = reaction;
            CurrentUpdateFunc = Behaviours[CurrentReactionType].UpdateFunc;
            return true;
        }
    }
    return false;
}

#if defined(_MSC_VER)
#pragma optimize("s", on)
#endif
inline bool CTCPerceiveThing::IsAwareOfThing() const
{
    return AwarenessTimer != 0.0f;
}
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif