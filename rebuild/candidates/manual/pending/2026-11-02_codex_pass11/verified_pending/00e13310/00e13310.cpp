// NScript::CQ_WaspBossScript::EndMission(void) -- finish the Wasp Boss mission.
// Leaf: ?EndMission@CQ_WaspBossScript@NScript@@QAEXXZ
#include "engine/fable_wasp.h"
void NScript::CQ_WaspBossScript::EndMission()
{
 CScriptThing Queen;
 Queen=Game.GetThingWithScriptName("QueenHornet");
 Game.DisplayQuestInfo(true);
 while(!QueenHornetAttacks) {
  Game.NewScriptFrame();
  if(IsActiveThreadTerminating())return;
 }
 if(IsActiveThreadTerminating())return;
 Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_WASP_MENACE_OBJECTIVE_02","","HeroGuildComplexInside");
 long health_bar=Game.AddQuestInfoBarHealth(Queen,CRGBColour(255,255,0,255),"HUD_ICON_WASP_HEAD",1.0f);
 bool mission_finished=false;
 while(!mission_finished) {
  Game.NewScriptFrame();
  if(IsActiveThreadTerminating())return;
  bool level_loaded=Game.IsLevelLoaded("PicnicArea");
  bool terminating=IsActiveThreadTerminating();
  if(level_loaded) {
   if(terminating)return;
   if(Queen.MsgIsKilledBy("")) {
    if(IsActiveThreadTerminating())return;
    mission_finished=true;
   }
  } else {
  if(terminating)return;
  while(!Game.IsLevelLoaded("PicnicArea")) {
   Game.NewScriptFrame();
   if(IsActiveThreadTerminating())return;
  }
  if(IsActiveThreadTerminating())return;
  Queen=Game.GetThingWithScriptName("QueenHornet");
  Game.DisplayQuestInfo(true);
  health_bar=Game.AddQuestInfoBarHealth(Queen,CRGBColour(255,255,0,255),"HUD_ICON_WASP_HEAD",1.0f);
  }
 }
 if(IsActiveThreadTerminating())return;
 Game.DisplayQuestInfo(false);
 Game.RemoveQuestInfoElement(health_bar);
 Outro();
 MissionSucceeded=true;
}
