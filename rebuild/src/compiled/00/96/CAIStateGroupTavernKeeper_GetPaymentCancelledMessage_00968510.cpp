// CAIStateGroup_TavernKeeper::GetPaymentCancelledMessage -- find a cancelled-payment message matching the customer.
// Retail: 0x00968510, 172 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?GetPaymentCancelledMessage@CAIStateGroup_TavernKeeper@@QAE_NXZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_WAITMESSAGE
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNKEEPER
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER_CONST
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/EAIAction.h"
#include "engine/EMessageEventType.h"
class CMessageEvent;
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_TavernKeeper.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CMessageEventManager.h"
#include "engine/CFilter_Bind3.h"
#include "engine/ThingAlive.h"
// Original donor02bc5110 and native0091a670: the real out-of-line callback loop.
template<class Filter>
#ifdef _MSC_VER
__declspec(noinline)
#endif
EAIActionReturn CAIStateGroupBase::AIAction_WaitForMessageCallback(EAIActionRepeat repeat,const Filter& filter,const CMessageEvent** ppevent)
{
 if(IsStopping())return AI_ACTION_TERMINATE_PROCESS;
 EAIActionReturn result=AI_ACTION_IN_PROGRESS;
 do{
  *ppevent=PeekWorld().GetMessageEventManager().FindMostRecentMessage(filter);
  if(*ppevent){result=AI_ACTION_FINISHED;break;}
  PBrain->SuspendSuspendableProcess();
  if(IsStopping()){result=AI_ACTION_TERMINATE_PROCESS;break;}
  result=AI_ACTION_IN_PROGRESS;
 }while(repeat==ACTION_REPEAT_UNTIL_FINISHED && result==AI_ACTION_IN_PROGRESS);
 return result;
}
bool CAIStateGroup_TavernKeeper::GetPaymentCancelledMessage()
{
 const CMessageEvent* pevent=0;
 if(IS_THING_ALIVE(PPayee)){
  if(AIAction_WaitForMessageCallback(ACTION_REPEAT_CALLBACK,
    GFFilter_Bind3(CMessageEventFilter_IsOfType(MESSAGE_EVENT_PAYMENT_CANCELLED),
      CMessageEventFilter_IsOriginatedByThing(*PPayee),CMessageEventFilter_IsTargettedAtThing(GetCreature())),&pevent)==AI_ACTION_FINISHED){
   GetWorld().GetMessageEventManager().RemoveMessage(pevent);
   return true;
  }
 }
 return false;
}