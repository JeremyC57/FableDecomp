// CAIStateGroup_VillageTaskPickCrops::Update() -- Accept a village crop assignment and take the crop when the approach succeeds.
// Retail: 00940310, 494 bytes, thiscall.
// Leaf: ?Update@CAIStateGroup_VillageTaskPickCrops@@UAEXXZ
#define FABLE_RETAIL_CROP_TASK_UPDATE_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CVILLAGETASKPICKCROPS
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PICKCROP
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CVillageTaskPickCrops.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CCreatureAction_PickCrop.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_VillageTaskPickCrops.h"

void CAIStateGroup_VillageTaskPickCrops::Update()
{
    CTCVillageMember* ptc_village_mem;
    if (!PTask.Get()) {
        Terminate();
        return;
    }
    if (!GetCreature().GetTC(TCI_VILLAGE_MEMBER, &ptc_village_mem)) {
        Terminate();
        return;
    }
    CTCVillage* pvillage = ptc_village_mem->GetPVillage();
    if (!pvillage || !pvillage->SetCropTaskAsAcceptedByThing(*PTask, GetCreature())) {
        PTask = CIntelligentPointer<CVillageTaskPickCrops>(0);
        Terminate();
        return;
    }
    CIntelligentPointer<CThingObject> pcrop(PTask->GetCrop());
    bool nav_route_calculated = false;
    EAIActionReturn ret;
    do {
        if (!PTask.Get() || !PTask->IsCreatureAssignedToTask(GetCreature()) || !IsTimeForWork()) {
            Terminate();
            return;
        }
        ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, pcrop, 0.2f,
            nav_route_calculated, AI_MOVEMENT_WALKING, true, false, true, true, 0.0f);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret == AI_ACTION_FINISHED && PTask.Get() && PTask->IsCreatureAssignedToTask(GetCreature())) {
        if (AIAction_PerformActionAndWait(CCreatureAction_PickCrop(GetCreature(), *pcrop), false) != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
    }
    Terminate();
    return;
}
