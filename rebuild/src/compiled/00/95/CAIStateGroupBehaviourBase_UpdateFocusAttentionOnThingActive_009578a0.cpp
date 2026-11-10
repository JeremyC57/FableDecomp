// CAIStateGroupBehaviourBase::UpdateFocusAttentionOnThingActive() -- maintain active attention on the target.
// Retail: 0x009578a0,148 bytes,thiscall.
// Leaf: ?UpdateFocusAttentionOnThingActive@CAIStateGroupBehaviourBase@@IAE_NXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBEHAVIOURBASE
#include "engine/fable_ai_action_enums.h"
#include "engine/CThing.h"
#include "engine/CAIStateGroupBehaviourBase.h"
bool CAIStateGroupBehaviourBase::UpdateFocusAttentionOnThingActive()
{
    EAIActionReturn ret=AI_ACTION_FAILED;
    do {
        if(!IsAwareOfThing())
            return false;
        bool nav;
        ret=AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,2.0f,nav,AI_MOVEMENT_WALKING,true,false,true,true,0.0f);
    }while(ret==AI_ACTION_IN_PROGRESS);
    if(ret!=AI_ACTION_FINISHED)
        return false;
    return UpdateFocusAttentionOnThingPassive();
}