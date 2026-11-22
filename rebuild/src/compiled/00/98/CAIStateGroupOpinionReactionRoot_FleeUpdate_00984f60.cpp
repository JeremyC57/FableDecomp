// CAIStateGroup_OpinionReactionRoot::FleeUpdate()
// Leaf: ?FleeUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_THINGREACTINGTO
#define FABLE_RETAIL_OPINIONREACTIONROOT_FLEEUPDATE_PASS26
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NEARBYDIRECTED
#define FABLE_RETAIL_MINION_CRIMINAL_UPDATE_PASS24
#define FABLE_RETAIL_KID_EVADE_PASS23
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE
#define FABLE_RETAIL_NSPEECHGAINMANAGER_ACTIVEDIALOGUEINFO
#define FABLE_RETAIL_CWORLD
#include "fable_string.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_OpinionReactionRoot.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsAlliedWithFaction.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingSearchTools.h"
#include "engine/NSpeechGainManager.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"

// Inline-budget: the native search helper calls the actual comparator at 0x00986C30.
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
const CThingAICreature* CThingCompare_Nearest::operator()(const CThingAICreature* first, const CThingAICreature* second) const
{
    if (!first) return second;
    if (!second) return first;
    return (first->GetPos() - Pos).GetSquaredMagnitude() <
           (second->GetPos() - Pos).GetSquaredMagnitude() ? first : second;
}

const float FLEE_DISTANCE = 15.0f;
const float FLEE_MAX_DURATION_SECS = 10.0f;
const float FLEE_MIN_DURATION_SECS = 4.0f;
const float FLEE_TOWARDS_ALLIES_ANGLE_ACCURACY_DEGREES = 30.0f;

bool CAIStateGroup_OpinionReactionRoot::FleeUpdate()
{
    const CTCCreatureNavigation* ptc_nav;
    GetCreature().GetTC(&ptc_nav);
    C3DVector ret_pos;
    bool ret_bool = false;
    if (ThingReactingTo->GetUniqueID() == GetCreature().GetUniqueID()) {
        const CTCEnemy* ptc_enemy;
        GetCreature().PeekTC(&ptc_enemy);
        const CThingAICreature* pcreature;
        if (PeekThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_IsNot(GetCreature()),
                CThingFilter_IsAlliedWithFaction(ptc_enemy->GetPFaction())), CThingCompare_Nearest(GetCreature().GetPos()),
                &pcreature) && ptc_nav->IsPosNavigable(pcreature->GetPos())) {
            ret_bool = ptc_nav->GetNearbyDirectedPosition(GetCreature().GetPos(), FLEE_DISTANCE,
                pcreature->GetPos() - GetCreature().GetPos(), (float)(FLEE_TOWARDS_ALLIES_ANGLE_ACCURACY_DEGREES / 360.0), ret_pos);
        }
    }
    if (!ret_bool)
        ret_bool = ptc_nav->GetNearestFleePosition(GetCreature().GetPos(), FLEE_DISTANCE, ret_pos);
    if (ret_bool) {
        CTCCreatureModeManager* ptc_cmm;
        GetCreature().GetTC(&ptc_cmm);
        bool pushed_mode = ptc_cmm->AddMode(15);

        unsigned long frame_to_stop_fleeing = (unsigned long)(CWorld::GetFrame() +
            (GFFloatRandom(FLEE_MAX_DURATION_SECS - FLEE_MIN_DURATION_SECS, GetPWorldSeed()) +
                FLEE_MIN_DURATION_SECS) * CMainGameComponent::GetConstantFPS());
        EAIActionReturn ret = AI_ACTION_FAILED;
        bool just_found_route = false;
        do {
            ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, ret_pos, 1.0f, ret_bool,
                AI_MOVEMENT_RUNNING, true, false, true, 0, true, 0.0f);
            if (!just_found_route && ret_bool) {
                just_found_route = true;
                if (WaitForTextToLoadThenSpeak("TEXT_AI_OPINION_ACT_SCREAM_TO_SELF",
                        NSpeechGainManager::DIALOGUE_LAYER_BACKGROUND) != AI_ACTION_FINISHED)
                    return false;
            }
        } while (ret == AI_ACTION_IN_PROGRESS && (unsigned long)CWorld::GetFrame() < frame_to_stop_fleeing);

        if (pushed_mode) {
            GetCreature().GetTC(&ptc_cmm);
            ptc_cmm->RemoveMode(15);
        }
        return ret == AI_ACTION_FINISHED || ret == AI_ACTION_IN_PROGRESS;
    }
    return false;
}