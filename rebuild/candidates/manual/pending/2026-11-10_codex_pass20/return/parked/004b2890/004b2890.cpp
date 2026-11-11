// CQuestManager::PostBoastTransfer -- accept transferred boasts and refresh the active quest screen
// Grade: DIFFER(569v569) -- 569/569,189/189 instructions;4hunks: quest/list arguments use ecx/eax instead of edx/ecx, interface key address uses edx instead of eax. All20external operands correct;141byte GetAcceptedBoastsForScript and72byte list clear helpers exact.
// Retail: 004b2890,569 bytes; thiscall,ret.
// Leaf: ?PostBoastTransfer@CQuestManager@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_BOASTMADE
#define FABLE_RETAIL_CQUESTMANAGER_POSTBOAST
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_POSTBOAST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/C3DVector.h"
#include "engine/CQuestManager.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CMessageEventManager.h"
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/ThingAlive.h"
#include "engine/CWorld.h"

void CQuestManager::GetAcceptedBoastsForScript(const CCharString& script_name,std::list<CBoast>& boasts)const
{
 boasts.clear();
 for(std::list<CBoast>::const_iterator it=AcceptedBoasts.begin();it!=AcceptedBoasts.end();++it)
  if(it->GetScriptName()==script_name)boasts.push_back(*it);
}

void CQuestManager::PostBoastTransfer()
{
 for(std::list<CBoast>::iterator it=TempBoasts.begin();it!=TempBoasts.end();++it) {
  CBoast* pboast=GetBoast(it->GetID(),it->GetScriptName());
  if(pboast) {
   if(it->IsFailed())pboast->SetAsFailed();
   if(it->IsCompleted())pboast->SetAsCompleted();
   AcceptedBoasts.push_back(*pboast);
   RemoveBoast(pboast->GetID(),pboast->GetScriptName());
   if(pboast->IsCompleted())continue;
   if(pboast->IsFailed())continue;
   {
    long extra_data=(long)&AcceptedBoasts.back();
    World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_BOAST_MADE,5000,C3DVector(0,0,0),0,0,0,0,extra_data);
   }
  }
 }
 CThingPlayerCreature* pplayer=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
 if(IS_THING_ALIVE(pplayer)) {
 for(std::list<CCountedPointer<CActiveQuest> >::iterator qit=ActiveQuests.begin();qit!=ActiveQuests.end();++qit) {
  std::vector<CCharString>::iterator last=BetweenStartAndEndScreenQuests.end();
  std::vector<CCharString>::iterator fit=std::find(BetweenStartAndEndScreenQuests.begin(),last,(*qit)->GetName());
  if(fit==last)continue;
  {
   std::list<CBoast> boasts;
   GetAcceptedBoastsForScript((*qit)->GetName(),boasts);
   if(!boasts.empty()) {
    CTCQuestCompletionUI* ptc_ui;
    if(pplayer->GetTC(&ptc_ui)) {
     ptc_ui->SetScriptName((*qit)->GetName());
     World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_LEAVING_QUEST_START_SCREEN,5000,C3DVector(0,0,0),pplayer,0,0,0,0);
    }
    break;
   }
  }
 }
}
}
