// 00915350 CAIStateGroup_KidHurtSelfRunToMother::Update
// Flags: none
// Leaf: ?Update@CAIStateGroup_KidHurtSelfRunToMother@@UAEXXZ
#include "fable_compiler.h"
#define FABLE_RETAIL_KID_HURT_SELF_PASS23
#define FABLE_RETAIL_KID_MOTHER_REFERENCE_PASS23
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_KidHurtSelfRunToMother.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CSoundPair.h"
#include "engine/CThingFilter_CanSeeEachOther.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "engine/GFWorldDistance.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"

void CAIStateGroup_KidHurtSelfRunToMother::Update()
{
    EAIActionReturn ret=AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),"BOY_TRIP_FALL_OVER_01",CSoundPair(0),true,true,false,0,0),false);
    if(ret==AI_ACTION_TERMINATE_PROCESS || ret==AI_ACTION_FAILED){Terminate();return;}
    CTCCreatureModeManager* ptc_cmm;
    GetCreature().GetTC(&ptc_cmm);
    ptc_cmm->AddMode(20);
    AddedSadMode=true;
    if(AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),"BOY_TRIP_GET_UP_01",CSoundPair(0),false,true,false,0,0),false)!=AI_ACTION_FINISHED){Terminate();return;}
    if(GFRandom(100,GetPWorldSeed())<75){Terminate();return;}
    if(IS_THING_ALIVE(GetCreature().PeekMotherCreature()) && GFWorld2DDistanceSmallerThan(GetCreature().GetPos(),GetCreature().PeekMotherCreature()->GetPos(),80.0f,false)){
        bool finished_nav2=false;
        CThingFilter_CanSeeEachOther can_see_each_other(GetCreature());
        bool sent_message=false;
        while((ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,GetCreature().PeekMotherCreature(),1.0f,finished_nav2,AI_MOVEMENT_RUNNING,true,false,true,true,0.0f))==AI_ACTION_IN_PROGRESS){
            if(!sent_message && can_see_each_other(*GetCreature().PeekMotherCreature())){SendMessageToMother();sent_message=true;}
        }
        if(ret!=AI_ACTION_FINISHED){Terminate();return;}
        if(!sent_message){SendMessageToMother();sent_message=true;}
        for(long c1=0;c1<5 && ret!=AI_ACTION_TERMINATE_PROCESS;++c1){
            long frame_to_stop_looking=CWorld::GetFrame()+2*CMainGameComponent::GetConstantFPS()+GFRandom(30,GetPWorldSeed());
            while(frame_to_stop_looking>CWorld::GetFrame() && ret!=AI_ACTION_TERMINATE_PROCESS)
                ret=AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,GetCreature().PeekMotherCreature(),1,0.0027777f);
            ret=PerformRandomLengthCrying();
        }
    }else ret=PerformRandomLengthCrying();
    Terminate();
}