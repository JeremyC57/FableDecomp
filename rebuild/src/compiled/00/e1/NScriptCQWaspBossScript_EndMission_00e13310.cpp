// NScript::CQ_WaspBossScript::EndMission(void) -- finish the Wasp Boss mission.
// Leaf: ?EndMission@CQ_WaspBossScript@NScript@@QAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#pragma auto_inline(off)
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()
{
 if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}
 Data=0;Info=0;
}
#pragma auto_inline(on)
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/NScript_CQ_WaspBossScript.h"
#include "stlport/fable_map.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp=other.PImp; return *this; }
inline bool CScriptThing::IsAlive()const{if(PImp.IsNull())return false;return PImp->IsAlive();}
inline bool CScriptThing::MsgIsKilledBy(const CCharString& name)const{if(PImp.IsNull())return false;return PImp->MsgIsKilledBy(name);}
void __fastcall RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
void NScript::CQ_WaspBossScript::EndMission()
{
 CScriptThing Queen;
 Queen=Game->GetThingWithScriptName("QueenHornet");
 Game->DisplayQuestInfo(true);
 while(!QueenHornetAttacks) {
  Game->NewScriptFrame();
  if(IsActiveThreadTerminating())return;
 }
 if(IsActiveThreadTerminating())return;
 Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_WASP_MENACE_OBJECTIVE_02","","HeroGuildComplexInside");
 long health_bar=Game->AddQuestInfoBarHealth(Queen,CRGBColour(255,255,0,255),"HUD_ICON_WASP_HEAD",1.0f);
 bool mission_finished=false;
 while(!mission_finished) {
  Game->NewScriptFrame();
  if(IsActiveThreadTerminating())return;
  bool level_loaded=Game->IsLevelLoaded("PicnicArea");
  bool terminating=IsActiveThreadTerminating();
  if(level_loaded) {
   if(terminating)return;
   if(Queen.MsgIsKilledBy("")) {
    if(IsActiveThreadTerminating())return;
    mission_finished=true;
   }
  } else {
  if(terminating)return;
  while(!Game->IsLevelLoaded("PicnicArea")) {
   Game->NewScriptFrame();
   if(IsActiveThreadTerminating())return;
  }
  if(IsActiveThreadTerminating())return;
  Queen=Game->GetThingWithScriptName("QueenHornet");
  Game->DisplayQuestInfo(true);
  health_bar=Game->AddQuestInfoBarHealth(Queen,CRGBColour(255,255,0,255),"HUD_ICON_WASP_HEAD",1.0f);
  }
 }
 if(IsActiveThreadTerminating())return;
 Game->DisplayQuestInfo(false);
 Game->RemoveQuestInfoElement(health_bar);
 Outro();
 MissionSucceeded=true;
}