// CQuestManager::SetQuestAsFailed -- report failure, finish its card, and refresh available quests
// Retail:004b2280,527bytes,thiscall,ret16.
// Leaf: ?SetQuestAsFailed@CQuestManager@@QAE_NABVCCharString@@_NABVCWideString@@1@Z
// Flags: none
#define FABLE_RETAIL_CQUESTMANAGER_FAILUREBOOKKEEP
#define FABLE_RETAIL_CQUESTMANAGER_FAILQUEST
#define FABLE_RETAIL_CQUESTMANAGER_COMPLETIONUI
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_QUESTNAME
#define FABLE_RETAIL_CTCQUESTCARD_FAILURE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_FAILEDQUEST
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_BOASTMADE
#define FABLE_RETAIL_CQUESTMANAGER_POSTBOAST
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
#include "fable_string.h"
#include "stlport/fable_containers.h"
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

#include "engine/CTCQuestCard.h"
#include "engine/CTCHeroStats.h"
#include "engine/CDefString.h"
#include "engine/CIsQuestCardWithDefName.h"

void CQuestManager::RemoveAvailableQuestCardType(const CCharString& def_name)
{
 AvailableQuestCardTypes.remove_if(CIsQuestCardWithDefName(def_name));
 SetQuestCardsNeedUpdating(true);
}
bool CQuestManager::SetQuestAsFailed(const CCharString& script_name,bool display_screen,const CWideString& reason,bool offer_teleport_to_guild)
{
 long index=GetIndexFromRegisteredQuest(script_name);
 if(index>-1) {
  World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_FAILED_BEFORE_SCREEN_SHOWN,50,C3DVector(0,0,0),0,0,0,0,index);
  if(!display_screen)
   World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_FAILED,50,C3DVector(0,0,0),0,0,0,0,index);
  CThingPlayerCreature* phero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
  if(IS_THING_ALIVE(phero)) {
   if(display_screen) {
    CTCQuestCompletionUI* ptc_ui;
    phero->GetTC(&ptc_ui);
    ptc_ui->Activate((CTCQuestCompletionUI::CompletionType)2,script_name,false,reason,offer_teleport_to_guild);
   } else OnQuestFinishScreenDisplayed(script_name);
   CThing* pquest_card=GetActiveQuestCardFromScriptName(script_name);
   if(pquest_card) {
    CTCQuestCard* ptc_quest_card;
    pquest_card->GetTC(&ptc_quest_card);
    if(ptc_quest_card)ptc_quest_card->SetAsFinished(true);
    CTCHeroStats* ptc_hero_stats;
    phero->GetTC(&ptc_hero_stats);
    ptc_hero_stats->SetNumFailedAQuest(ptc_hero_stats->GetNumFailedAQuest()+1);
    RemoveAvailableQuestCardType(pquest_card->GetDefName());
   }
   RemoveActiveQuestCardAndMakeAvailableAgain(script_name,false);
  }
  return true;
 }
 return false;
}