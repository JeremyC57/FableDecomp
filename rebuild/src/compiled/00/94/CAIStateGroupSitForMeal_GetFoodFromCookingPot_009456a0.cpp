// 009456a0 CAIStateGroup_SitForMeal::GetFoodFromCookingPot
// Leaf: ?GetFoodFromCookingPot@CAIStateGroup_SitForMeal@@AAE_NXZ
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_COOKING_POT_MEAL_PASS29
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_BUILDING_GETTERS_PASS26
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER_PROTECTED
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
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
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_SitForMeal.h"
#include "engine/CTCUserActionPointBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/ThingAlive.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CCreatureAction_GetItemOutOfPocket.h"

#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
inline bool CAIStateGroup_SitForMeal::IsMealTime() const
{
    const CTCVillageMember* member;
    if (GetCreature().PeekTC(&member) && member->GetPVillage() && member->GetPVillage()->IsEatTime())
        return true;
    return false;
}

namespace { extern CCharString NAME_OF_BITCHIN_BOWL; }
bool CAIStateGroup_SitForMeal::GetFoodFromCookingPot()
{
    CIntelligentPointer<const CThingBuilding> p_home_building(GetCreature().GetHomeBuilding());
    if (IS_THING_DEAD(PPot) || IS_THING_DEAD(p_home_building)) return false;
    if (!p_home_building->IsPointInside(PPot->GetPos())) p_home_building=0;
    ActionPointIndex=-1;
    bool ready_to_eat=false;
    bool has_nearby_wait_pos=false;
    C3DVector wait_pos=PPot->GetPos();
    C3DVector action_pos,look_pos;
    float proximity=GFRandom(100,GetPWorldSeed())*0.05f+2.0f;
    EAIActionReturn ret;
    do {
        CTCSingleUserActionPoints* ptc_pot=0;
        if (!(IS_THING_ALIVE(PPot) && PPot->GetTC(&ptc_pot))) return false;
        CRightHandedSet rhs;
        if (ptc_pot->GetFreeActionPoint(GetCreature(),action_pos,rhs,ActionPointIndex)) {
            ready_to_eat=true;
            look_pos=action_pos+rhs.GetForward();
        }
        if (!ready_to_eat) {
            bool nav;
            ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PPot,proximity,nav,AI_MOVEMENT_WALKING,true,true,true,true,0.0f);
            if (ret!=AI_ACTION_FINISHED && ret!=AI_ACTION_IN_PROGRESS) return false;
        }
    } while (!ready_to_eat && IsMealTime());
    if (!ready_to_eat) return false;
    bool nav;
    ret=AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED,action_pos,nav,AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
    if (ret!=AI_ACTION_FINISHED) return false;
    ret=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,look_pos,17,0.0027777f);
    if (ret!=AI_ACTION_FINISHED) return false;
    long bowl_def_index=PeekDefinitionManager().GetDefGlobalIndexFromName(NAME_OF_BITCHIN_BOWL);
    ret=AIAction_PerformActionAndWait(CCreatureAction_GetItemOutOfPocket(GetCreature(),bowl_def_index),false);
    if (ret!=AI_ACTION_FINISHED) return false;
    HoldingBowl=true;
    ret=AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),"ST_FILL_ITEM",CSoundPair(0),false,true,false,0,0),false);
    if (ret!=AI_ACTION_FINISHED) return false;
    return true;
}