// CAIStateGroup_VillageTaskTorch::Update -- advance the village torch task and complete it when appropriate.
// Retail: 0x0093EA90, 452 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?Update@CAIStateGroup_VillageTaskTorch@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_USEOBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TORCHTASK
#define FABLE_RETAIL_CVILLAGETASKTORCH
#define FABLE_RETAIL_CVILLAGETASKTORCH_GETTORCH
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CVillageTaskTorch.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_VillageTaskTorch.h"
#include "engine/CCreatureAction_UseObject.h"
void CAIStateGroup_VillageTaskTorch::Update()
{
 if(PTask==0) { Terminate(); return; }
 CTCVillage* pvillage=PTCVillageMember->GetPVillage();
 if(pvillage->SetTorchTaskAsAcceptedByThing(*PTask,GetCreature())) {
  CThing* torch=PTask->GetTorch();
  if(!torch) { Terminate(); return; }
  C3DVector action_pos(0.0f,0.0f,0.0f);
  CRightHandedSet rhset;
  torch->GetTCGraphicAppearance()->GetDummyObject(ACTION_POINT_01,0,0,action_pos,rhset,1.0f);
  EAIActionReturn ret=AI_ACTION_FAILED;
  do {
   if(PTask==0 || !PTask->IsCreatureAssignedToTask(GetCreature())) { Terminate(); return; }
   bool nav_route_calculated=false;
   ret=AIAction_MoveToPos(ACTION_REPEAT_CALLBACK,action_pos,nav_route_calculated,AI_MOVEMENT_WALKING,true,true,0,true,0.0f);
  }while(ret==AI_ACTION_IN_PROGRESS);
  if(ret!=AI_ACTION_FINISHED || PTask==0 || !PTask->IsCreatureAssignedToTask(GetCreature())) { Terminate(); return; }
  if(ret!=AI_ACTION_FINISHED || PTask==0 || !PTask->IsCreatureAssignedToTask(GetCreature())) { Terminate(); return; }
  if(AIAction_PerformActionAndWait(CCreatureAction_UseObject(GetCreature(),*torch,true),false)==AI_ACTION_FINISHED) {
   pvillage=PTCVillageMember->GetPVillage();
   if(pvillage && PTask!=0) {
#ifdef _MSC_VER
    // Select the original mutable same-type constructor template on VC71.
    pvillage->SetTorchTaskAsCompleted(CIntelligentPointer<CVillageTaskTorch>::CIntelligentPointer<CVillageTaskTorch>(PTask));
#else
    pvillage->SetTorchTaskAsCompleted(CIntelligentPointer<CVillageTaskTorch>(PTask));
#endif
   }
  }
 }
 Terminate();
}