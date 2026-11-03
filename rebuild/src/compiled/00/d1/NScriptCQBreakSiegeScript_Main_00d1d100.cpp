// NScript::CQ_BreakSiegeScript::Main() -- original script entry logic.
// Leaf: ?Main@CQ_BreakSiegeScript@NScript@@UAEXXZ
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CQ_BreakSiegeScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFunc.h"

void NScript::CQ_BreakSiegeScript::Main()
{
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BREAK_SIEGE_OBJECTIVE_01", "KnotholeGlade", "KnotholeGlade");
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("ImprisonedBanditLeader", this, &CImprisonedBanditLeader::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("KG_ChiefBreakSiege", this, &CKG_ChiefBreakSiege::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("GuardToHelpDefend", this, &CGuardToHelpDefend::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("BanditAttacker", this, &CBanditAttacker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("BanditSiegeLeader", this, &CBanditSiegeLeader::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("KG_Sheriff", this, &CKG_Sheriff::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("GuardHoldingBandit", this, &CGuardHoldingBandit::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("BanditFirstWave", this, &CBanditFirstWave::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BreakSiegeScript>("DisplacedTrader", this, &CDisplacedTrader::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsRegionLoaded("KnotholeGlade")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    AddSpawnedFunction(new CSpawnedFunc<CQ_BreakSiegeScript>("WatchForOpenGates", 0, this, &CQ_BreakSiegeScript::WatchForOpenGates), "KnotholeGlade");
    AddSpawnedFunction(new CSpawnedFunc<CQ_BreakSiegeScript>("WatchForWinLoseConditions", 0, this, &CQ_BreakSiegeScript::WatchForWinLoseConditions), "KnotholeGlade");
    AddSpawnedFunction(new CSpawnedFunc<CQ_BreakSiegeScript>("WatchForTermination", 0, this, &CQ_BreakSiegeScript::WatchForTermination), CCharString(""));
    if (!BanditSiegeStart) {
        if (IsActiveThreadTerminating()) return;
        CScriptThing Gate = Game->GetThingWithScriptName("KG_Gate");
        Game->CloseDoor(Gate);
        Game->SetThingPersistent(Gate, true);
    }
}