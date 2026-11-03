// NScript::CV_HeroDuelScript::DoMission() -- challenge the hero and wait for competing quests to clear before the duel.
// Leaf: ?DoMission@CV_HeroDuelScript@NScript@@QAEXXZ
// Flags: /Gr
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/NScript_CV_HeroDuelScript.h"
#include "engine/CGameScriptInterfaceBase.h"
void NScript::CV_HeroDuelScript::DoMission()
{
 while(!Game->IsLevelLoaded("BowerstoneJail")) {
  Game->NewScriptFrame();
  if(IsActiveThreadTerminating()) return;
 }
 if(IsActiveThreadTerminating()) return;
 DuelChallenge();
 Game->SetQuestCardObjective("V_MayorsInvitation","TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_08","ExecutionTree","");
 while(!Game->IsLevelLoaded("HeroDualArea")) {
  Game->NewScriptFrame();
  if(IsActiveThreadTerminating()) return;
  if(Game->IsQuestActive("Q_AmbushTraders") || Game->IsQuestActive("Q_HangingTreeEvil") ||
     Game->IsQuestActive("Q_HangingTreeGood") || Game->IsQuestActive("Q_EndGameFocalSites") ||
     Game->IsQuestActive("Q_MinionClifftopChase")) {
   if(IsActiveThreadTerminating()) return;
   while(Game->IsQuestActive("Q_AmbushTraders") || Game->IsQuestActive("Q_HangingTreeEvil") ||
         Game->IsQuestActive("Q_HangingTreeGood") || Game->IsQuestActive("Q_EndGameFocalSites") ||
         Game->IsQuestActive("Q_MinionClifftopChase")) {
    Game->NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
   }
   if(IsActiveThreadTerminating()) return;
   while(Game->IsLevelLoaded("HeroDualArea") != false) {
    Game->NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
   }
   if(IsActiveThreadTerminating()) return;
  }
 }
 if(IsActiveThreadTerminating()) return;
 GotToExecutionTree=true;
 Duel();
 DuelStarted=true;
}