// NScript::CQ_HangingTreeEvilScript::Main() -- the evil Hanging Tree quest's main: set the quest card, wait for BowerstoneJail, start the mission timer, bind the eight entity scripts, spawn DoMission and BanditYells, wait for success or failure, then complete (morality, drop the good quest's card) or fail the quest, re-enable the creature generators and deactivate the quest
// Grade: DIFFER(2118v2118) -- 15 masked lines, all in the failure branch: FadeScreenOut/Pause/SetQuestAsFailed take eax<->edx (and ecx) rotated; next idea: find the retail source shape that resets VC7.1's scratch-register rotation at the else block (compile-only sweep of ~20 forms in NOTES.md left it unchanged)
// Leaf: ?Main@CQ_HangingTreeEvilScript@NScript@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CQ_HangingTreeEvilScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
void NScript::CQ_HangingTreeEvilScript::Main()
{
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HANGING_TREE_EVIL_OBJECTIVE_01", "ExecutionTree", "BowerstoneSlums");
    while (!Game->IsLevelLoaded("BowerstoneJail")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    MissionTimer = 1000;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HANGING_TREE_EVIL_OBJECTIVE_01", "ExecutionTree", "BowerstoneSlums");
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_AmbushPoint", this, &CHTE_AmbushPoint::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_AssaultGuard", this, &CHTE_AssaultGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_FreedomFighter", this, &CHTE_FreedomFighter::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_Audience", this, &CHTE_Audience::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_Prisoner", this, &CHTE_Prisoner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_Executioner", this, &CHTE_Executioner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("GUARD1", this, &CGUARD1::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("GUARD2", this, &CGUARD2::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("DoMission", 0, this, &CQ_HangingTreeEvilScript::DoMission), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("BanditYells", 0, this, &CQ_HangingTreeEvilScript::BanditYells), CCharString(""));
    while (!MissionSucceeded && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating()) return;
        CleanUp();
        Game->Pause(1.0f);
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
        Game->GiveHeroMorality(pGlobalScriptDef->HTEvilCompletionMorality);
        Game->RemoveQuestCardFromGuild("Q_HangingTreeGood");
    } else {
        if (IsActiveThreadTerminating()) return;
        Game->FadeScreenOut(2.0f, 0.5f, CRGBColour(0, 0, 0, 255));
        CleanUp();
        Game->Pause(2.0f);
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, ReasonForFailing.ToWideString(), true);
    }
    Game->FadeScreenIn();
    Game->SetCreatureGeneratorsEnabled("BowerstoneJail", true);
    Game->SetCreatureGeneratorsEnabled("GibbetWoods", true);
    Game->SetCreatureGeneratorsEnabled("DesertedFarm", true);
    Game->SetCreatureGeneratorsEnabled("ExecutionTree", true);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
