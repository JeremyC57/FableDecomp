// CQuestManager::SetBoastAsMade(index,script_name) -- accept a boast and notify the world.
// Leaf: ?SetBoastAsMade@CQuestManager@@QAEXJVCCharString@@@Z
// Retail:004b1a80,277 bytes,thiscall
// Flags: none
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_BOASTMADE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BOASTS
#define FABLE_RETAIL_CTCHEROSTATS_BOASTTAKEN
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/CQuestManager.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCHeroStats.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CWorld.h"
#include "engine/CMessageEventManager.h"
#include "engine/ThingAlive.h"
#include "fable_game.h"
void CQuestManager::SetBoastAsMade(long index,CCharString script_name)
{
 CBoast* pboast=GetBoast(index,script_name);
 if(pboast){
  CThingPlayerCreature* phero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
  if(IS_THING_ALIVE(phero)){
   CTCHeroStats* ptc_hero_stats;
   if(phero->GetTC(&ptc_hero_stats))ptc_hero_stats->SetNumBoastsTaken(ptc_hero_stats->GetNumBoastsTaken()+1);
  }
  AcceptedBoasts.push_back(*pboast);
  long extra_data=(long)&AcceptedBoasts.back();
  World->GetMessageEventManager().AddMessage(MESSAGE_EVENT_BOAST_MADE,50,C3DVector(0.0f,0.0f,0.0f),0,0,0.0f,0.0f,extra_data);
  RemoveBoast(index,script_name);
 }
}