// NScript::CQ_HangingTreeGoodScript::Main() -- the good Hanging Tree quest's main: wait for BowerstoneJail, bind the ten entity scripts, spawn DoMission, wait for success, failure or the party's defeat, then complete (prisoner-kill rewards, morality, drop the evil quest's card) or fail the quest, clear the party, re-enable the creature generators and deactivate the quest
// Grade: DIFFER(2106v2106) -- 13 masked lines, all in the failure branch: the same eax<->edx rotation as CQ_HangingTreeEvilScript::Main 0x00D69480 (FadeScreenOut, SetQuestAsFailed); next idea: as 0x00D69480
// Leaf: ?Main@CQ_HangingTreeGoodScript@NScript@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HangingTreeGoodScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
void NScript::CQ_HangingTreeGoodScript::Main()
{
    while (!Game->IsLevelLoaded("BowerstoneJail")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_Prisoner", this, &CHTG_Prisoner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_Guard", this, &CHTG_Guard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_AmbushPoint", this, &CHTG_AmbushPoint::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_AmbushPointRear", this, &CHTG_AmbushPointRear::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_AmbushPointCond", this, &CHTG_AmbushPointCond::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_AmbushPointBackup", this, &CHTG_AmbushPointBackup::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_Executioner", this, &CHTG_Executioner::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_Audience", this, &CHTG_Audience::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_Boss", this, &CHTG_Boss::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HangingTreeGoodScript>("HTG_FreedomFighter", this, &CHTG_FreedomFighter::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeGoodScript>("DoMission", 0, this, &CQ_HangingTreeGoodScript::DoMission), CCharString(""));
    while (!MissionSucceeded && !MissionFailed && !PartyDefeated) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating()) return;
        Game->FadeScreenOut(2.0f, 0.5f, CRGBColour(0, 0, 0, 255));
        if (HeroKilledPrisoner) {
            if (IsActiveThreadTerminating()) return;
            Game->SetQuestCardGoldReward(Game->GetActiveQuestName(), pGlobalScriptDef->HTGoodHeroKilledPrisonerGoldReward);
            Game->SetQuestCardRenownReward(Game->GetActiveQuestName(), pGlobalScriptDef->HTGoodHeroKilledPrisonerRenownReward);
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        Game->GiveHeroMorality(pGlobalScriptDef->HTGoodCompletionMorality);
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
        Game->RemoveQuestCardFromGuild("Q_HangingTreeEvil");
        RemoveParty(false);
        CleanUp();
    } else {
        if (IsActiveThreadTerminating()) return;
        Game->FadeScreenOut(2.0f, 0.5f, CRGBColour(0, 0, 0, 255));
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, ReasonForFailing.ToWideString(), true);
        RemoveParty(false);
        RemoveDregs();
    }
    Game->FadeScreenIn();
    Game->SetCreatureGeneratorsEnabled("BowerstoneJail", true);
    Game->SetCreatureGeneratorsEnabled("GibbetWoods", true);
    Game->SetCreatureGeneratorsEnabled("DesertedFarm", true);
    Game->SetCreatureGeneratorsEnabled("ExecutionTree", true);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
