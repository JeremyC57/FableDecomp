// CQuestManager::PostBoastTransfer() -- move each transferred (temporary) boast into the accepted list with its failed / completed state, announcing the still-open ones; then, for the first active quest that sits between its start and end screens and has accepted boasts, hand its name to the hero's quest-completion UI and announce leaving the quest start screen
// Leaf: ?PostBoastTransfer@CQuestManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x004B2890, 569 bytes, thiscall `ret`.
// Inline-budget: the existing shared opt-in FABLE_RETAIL_STLP_FIND_NOINLINE_BODY makes __find visible but __declspec(noinline) (retail
//   calls 0x004B8E40 out of line); CBasicString::Compare (FABLE_RETAIL_CBASICSTRING_COMPARE_BODY, plain in-class) stays called by itself.
// Names: Ego_r.pdb / Ego_d.pdb ?PostBoastTransfer@CQuestManager@@QAEXXZ; Ego_d quest_manager.obj: function-scope `it` and pplayer, the
//   pboast block with its nested extra_data block, the qit block holding fit and boasts with the nested ptc_ui block -- as written here
//   (no `last` local: Ego_d lists none).
// Callees: CCharString copy 0x0099EC30 (GetBoast's by-value name), CQuestManager::GetBoast 0x004AF7A0 / RemoveBoast 0x004AFE20,
//   operator new 0x00BFEA0E + CBoast copy ctor 0x004B73A0 (list push_back), CMessageEventManager::AddMessage 0x00687540, the main
//   player's controlled creature 0x00449970 / 0x00487DC0, std::__find<CCharString*,CCharString> 0x004B8E40, list clear 0x004B8C00,
//   free 0x00BFEA14, CVectorMap::LowerBound 0x0040F020 (GetTC<CTCQuestCompletionUI>), CCharString::operator= 0x0099EFB0.
// Same-TU bodies: CQuestManager::GetAcceptedBoastsForScript (retail 0x004B1960, RELOCATION_MATCH from this file) after the row in Ego_d
//   order; the shared visible noinline __find (FABLE_RETAIL_STLP_FIND_NOINLINE_BODY; 0x004B8E40 RELOCATION_MATCH from this file) AND
//   CBasicString<char>::Compare (FABLE_RETAIL_CBASICSTRING_COMPARE_BODY; 0x00411570 MATCH from this file): with both bodies visible VC7.1
//   knows the find call writes no memory, keeps BetweenStartAndEndScreenQuests.end() in edi across it (`cmp eax,edi`, 0x004B29DF) without
//   a named `last`, and the GetAcceptedBoastsForScript argument / GetTC key registers come out as retail (GOTCHAS wf10, 0x006E3580).
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
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
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

void CQuestManager::PostBoastTransfer()
{
 std::list<CBoast>::iterator it;
 for(it=TempBoasts.begin();it!=TempBoasts.end();++it) {
  CBoast* pboast=GetBoast(it->GetID(),it->GetScriptName());
  if(pboast) {
   if(it->IsFailed())pboast->SetAsFailed();
   if(it->IsCompleted())pboast->SetAsCompleted();
   AcceptedBoasts.push_back(*pboast);
   RemoveBoast(pboast->GetID(),pboast->GetScriptName());
   if(!pboast->IsCompleted() && !pboast->IsFailed()) {
    long extra_data=(long)&AcceptedBoasts.back();
    World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_BOAST_MADE,5000,C3DVector(0,0,0),0,0,0,0,extra_data);
   }
  }
 }
 CThingPlayerCreature* pplayer=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
 if(IS_THING_ALIVE(pplayer)) {
  for(std::list<CCountedPointer<CActiveQuest> >::iterator qit=ActiveQuests.begin();qit!=ActiveQuests.end();++qit) {
   std::vector<CCharString>::iterator fit=std::find(BetweenStartAndEndScreenQuests.begin(),BetweenStartAndEndScreenQuests.end(),(*qit)->GetName());
   if(fit==BetweenStartAndEndScreenQuests.end())continue;
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

void CQuestManager::GetAcceptedBoastsForScript(const CCharString& script_name,std::list<CBoast>& boasts)const
{
 boasts.clear();
 for(std::list<CBoast>::const_iterator it=AcceptedBoasts.begin();it!=AcceptedBoasts.end();++it)
  if(it->GetScriptName()==script_name)boasts.push_back(*it);
}
