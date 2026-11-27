// 009101c0 CAIStateGroup_MinionListenToStory::Update
// Leaf: ?Update@CAIStateGroup_MinionListenToStory@@MAEXXZ
// Flags: none
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_MINION_LISTEN_STORY_PASS29
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_DECL_PASS26
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_SCHOOLSTORYTIME
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#include "engine/C3DVector.h"
class CThing;
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/CAIStateGroup_MinionListenToStory.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/GFFloatRandom.h"
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
#pragma intrinsic(sin,cos)
void CAIStateGroup_MinionListenToStory::Update()
{
    long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("STORY_PLACE_MARKER");
    CThing* story_place = 0;
    PeekThingSearchTools().GetPBestThing(CThingFilter_HasDefGlobalIndex(def_index), CThingCompare_Nearest(GetCreature().GetPos()), &story_place);
    if (!story_place) {
        Terminate();
        return;
    }
    C3DVector teacher_pos = story_place->GetPos();
    CRightHandedSet teacher_orientation = story_place->GetPhysicsTC()->GetRHSet();
    static float dist = 2.0f;
    static float max_angle = 0.25f;
    float radius = GFFloatRandom(1.5f, GetPWorldSeed());
    radius += 3.5;
    float angle = GFFloatRandom(max_angle*6.2831855f,GetPWorldSeed()) - max_angle*3.1415927f;
    C3DVector sit_pos = teacher_pos + teacher_orientation.GetForward() * (radius * (float)cos(angle))
                                    + teacher_orientation.GetRight() * (radius * (float)sin(angle));
    EAIActionReturn ret = AI_ACTION_IN_PROGRESS;
    do {
        bool nav_route_calculated;
        ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK,sit_pos,dist,nav_route_calculated,AI_MOVEMENT_RUNNING,true,false,true,0,true,0);
    } while (ret==AI_ACTION_IN_PROGRESS);
    if (ret!=AI_ACTION_FINISHED) { Terminate(); return; }
    do {
        ret = AIAction_LookAtPos(ACTION_REPEAT_CALLBACK,teacher_pos,0x11,0.0027777f);
    } while (ret==AI_ACTION_IN_PROGRESS);
    if (!SitDown()) { Terminate(); return; }
    while (AIAction_DoNothing()==AI_ACTION_IN_PROGRESS) {
        const CTCVillageMember* ptc_village_mem;
        GetCreature().GetTC(&ptc_village_mem);
        if (!ptc_village_mem->GetPVillage()->IsStoryTime()) break;
    }
    Terminate();
}