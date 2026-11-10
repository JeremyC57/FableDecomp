// 00946190 CAIStateGroup_PrepareMeal::Update
// Leaf: ?Update@CAIStateGroup_PrepareMeal@@MAEXXZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_PREPAREMEAL
#include "engine/fable_ai_action_enums.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAIStateGroup_PrepareMeal.h"
void CAIStateGroup_PrepareMeal::Update()
{
    bool arrived;
    EAIActionReturn result=AI_ACTION_FAILED;
    do {
        result=AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,ActionPointPos,arrived,AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
    } while(result==AI_ACTION_IN_PROGRESS);
    if(result!=AI_ACTION_FINISHED){Terminate();return;}
    C3DVector forward=ActionOrientation.GetForward();
    C3DVector pos=GetCreature().PeekPos()+forward;
    result=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,pos,0x11,0.0027777f);
    if(result!=AI_ACTION_FINISHED){Terminate();return;}
    CTCCreatureModeManager* modes;
    GetCreature().GetTC(&modes);
    modes->AddMode(0xe);
    while(AIAction_DoNothing()==AI_ACTION_IN_PROGRESS){}
    modes->RemoveMode(0xe);
    Terminate();
}