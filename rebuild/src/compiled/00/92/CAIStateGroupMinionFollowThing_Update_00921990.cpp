// 00921990 CAIStateGroup_MinionFollowThing::Update
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
// Leaf: ?Update@CAIStateGroup_MinionFollowThing@@MAEXXZ
// Flags: /Gr
#define FABLE_RETAIL_MINION_FOLLOW_UPDATE_PASS29
#define FABLE_RETAIL_CAISTATEGROUP_MINIONFOLLOWTHING
#define FABLE_RETAIL_CTCREGIONFOLLOWER
#define FABLE_RETAIL_CTCREGIONFOLLOWER_REGIONLOAD
#define FABLE_RETAIL_CTHINGAICREATURE_REGIONLOAD
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_GFGET2DDISTANCESQUARED
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_DISTANCE_PASS26
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/GFRandom.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

#include "engine/CTCFollowed.h"
#include "engine/CTCRegionFollower.h"
#include "engine/C2DVector.h"
#include "engine/GFSqr.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_MinionFollowThing.h"

void CAIStateGroup_MinionFollowThing::Update()
{
    if (IS_THING_ALIVE(PFollowee)) {
        SetThingToNotAvoid(PFollowee.Get());
        EAIActionReturn return_value;
        do {
            if (ExpressionUsed) {
                if (IsExpressionPerfomedTo("EXPRESSION_WAIT")) {
                    ExpressionUsed=false;
                    if (FollowerAdded && PFollowee!=0) {
                        CTCFollowed* ptc_followed;
                        if (PFollowee->GetTC(&ptc_followed))
                            ptc_followed->RemoveFollower(GetCreature());
                        if (!GetCreature().IsRegionFollowingOverriddenFromScript()) {
                            CTCRegionFollower* ptc_region_follower;
                            if (PFollowee->GetTC(&ptc_region_follower))
                                ptc_region_follower->RemoveFollower(GetCreature());
                        }
                    }
                    Terminate();
                    return;
                }
                CTCFollowed* ptc_followed;
                if (IS_THING_ALIVE(PFollowee) && PFollowee->GetTC(&ptc_followed) && !ptc_followed->IsFollower(&GetCreature())) {
                    ExpressionUsed=false;
                    Terminate();
                    return;
                }
            }
            if (IsExpressionPerfomedTo("EXPRESSION_FOLLOW"))
                ExpressionUsed=true;
            if (!IS_THING_ALIVE(PFollowee)) { Terminate(); return; }
            if (IsStopping()) { Terminate(); return; }
            if (GFGet2DDistanceSquared(GetCreature().PeekPos(),PFollowee->PeekPos()) < GFSqr(2.0f)) {
                CurrentMovementType=AI_MOVEMENT_NULL;
                return_value=AIAction_DoNothing();
            } else if (CurrentMovementType==AI_MOVEMENT_NULL && GFGet2DDistanceSquared(GetCreature().PeekPos(),PFollowee->PeekPos()) < GFSqr(3.5f)) {
                return_value=AIAction_DoNothing();
            } else {
                if (CurrentMovementType==AI_MOVEMENT_NULL) CurrentMovementType=AI_MOVEMENT_WALKING;
                if (CurrentMovementType==AI_MOVEMENT_WALKING) {
                    if (GFGet2DDistanceSquared(GetCreature().PeekPos(),PFollowee->PeekPos()) > GFSqr(7.5f)) CurrentMovementType=AI_MOVEMENT_RUNNING;
                } else if (GFGet2DDistanceSquared(GetCreature().PeekPos(),PFollowee->PeekPos()) < GFSqr(4.5f)) CurrentMovementType=AI_MOVEMENT_WALKING;
                bool finished_calculating_route;
                return_value=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PFollowee,FollowDistance,finished_calculating_route,CurrentMovementType,true,false,AvoidDynamicObstacles,true,0.0f);
            }
        } while (return_value==AI_ACTION_IN_PROGRESS);
    } else {
        CTCAIScratchpad* ai_scratchpad;
        GetCreature().GetTC(&ai_scratchpad);
        CThing* thing_to_follow=0;
        PeekThingSearchTools().GetPBestThing(CThingFilter_HasScriptName(ai_scratchpad->GetTargetName()), CThingCompare_Nearest(GetCreature().PeekPos()), &thing_to_follow);
        if (ai_scratchpad->GetScriptingStateGroup()==ESSG_FOLLOW_NEAREST) {
            PeekThingSearchTools().GetPBestThing(CThingFilter_HasScriptName(ai_scratchpad->GetTargetName()), CThingCompare_Nearest(GetCreature().PeekPos()), &thing_to_follow);
        } else {
            std::vector<CThing*> things;
            PeekThingSearchTools().GetAllThings<CThing>(CThingFilter_HasScriptName(ai_scratchpad->GetTargetName()),things);
            if (things.empty()) { Terminate(); return; }
            thing_to_follow=things[GFRandom(things.size(),GetPWorldSeed())];
        }
        while (ai_scratchpad->GetScriptingStateGroup()==ESSG_FOLLOW_RANDOM || ai_scratchpad->GetScriptingStateGroup()==ESSG_FOLLOW_NEAREST) {
            bool finished_calculating_route;
            AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,thing_to_follow,ai_scratchpad->FollowDistance,finished_calculating_route,AI_MOVEMENT_WALKING,true,false,AvoidDynamicObstacles,true,0.0f);
            if (IsStopping()) break;
        }
        Terminate();
    }
}