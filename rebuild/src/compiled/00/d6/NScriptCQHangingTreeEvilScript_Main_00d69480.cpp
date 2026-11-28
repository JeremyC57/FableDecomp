// Leaf: ?Main@CQ_HangingTreeEvilScript@NScript@@UAEXXZ
// Flags: /O2 /Oy /W3
// Inline-budget: the linked CSpawnedFuncBase destructor calls the predicate counted-pointer destructor at 00CDD4F0.
#define FABLE_RETAIL_HANGING_TIMER_INLINES_PASS30
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/TRetVoid.h"
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

void NScript::CQ_HangingTreeEvilScript::Main()
{
    TRetVoid _Fail_Val;
    Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_HANGING_TREE_EVIL_OBJECTIVE_01", "ExecutionTree", "BowerstoneSlums");
    while (!Game.IsLevelLoaded("BowerstoneJail")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    MissionTimer = 1000;
    Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_HANGING_TREE_EVIL_OBJECTIVE_01", "ExecutionTree", "BowerstoneSlums");
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_AmbushPoint", this, &CHTE_AmbushPoint::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_AssaultGuard", this, &CHTE_AssaultGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_FreedomFighter", this, &CHTE_FreedomFighter::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_Audience", this, &CHTE_Audience::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_Prisoner", this, &CHTE_Prisoner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("HTE_Executioner", this, &CHTE_Executioner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("GUARD1", this, &CGUARD1::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeEvilScript>("GUARD2", this, &CGUARD2::Alloc, 1));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("DoMission", 0, this, &CQ_HangingTreeEvilScript::DoMission), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("BanditYells", 0, this, &CQ_HangingTreeEvilScript::BanditYells), CCharString(""));
    while (!MissionSucceeded && !MissionFailed) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        CleanUp();
        Game.Pause(1.0f);
        Game.SetQuestAsCompleted(Game.GetActiveQuestName(), true, true, false);
        Game.GiveHeroMorality(pGlobalScriptDef->HTEvilCompletionMorality);
        Game.RemoveQuestCardFromGuild("Q_HangingTreeGood");
        Game.FadeScreenIn();
    } else {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game.FadeScreenOut(2.0f, 0.5f, CRGBColour(0, 0, 0));
        CleanUp();
        Game.Pause(2.0f);
        Game.SetQuestAsFailed(Game.GetActiveQuestName(), true, ReasonForFailing.ToWideString(), true);
        Game.FadeScreenIn();
    }
    Game.SetCreatureGeneratorsEnabled("BowerstoneJail", true);
    Game.SetCreatureGeneratorsEnabled("GibbetWoods", true);
    Game.SetCreatureGeneratorsEnabled("DesertedFarm", true);
    Game.SetCreatureGeneratorsEnabled("ExecutionTree", true);
    Game.DeactivateQuestLater(Game.GetActiveQuestName(), 0);
}