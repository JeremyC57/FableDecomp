// NScript::CQ_TraderEscortScript::Main() -- the trader escort quest main: bind the fourteen entity scripts (DarkwoodTrader .. EndTrader), post the scripted entities, set the first Darkwood escort objective, spawn WatchForSurprisingBalverines and WatchForMissionRules, then until the quest start screen has been shown watch for the hero pickpocketing and add the pickpocket-traders boast
// Leaf: ?Main@CQ_TraderEscortScript@NScript@@UAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E00DB0, 2088 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 2 (Main) of the RTTI-less quest-script vtable 0x012DF16C (0x012DF174; slots 0 ??_G 0x00E0AFB0,
//   1 RegisterMain 0x00E00C20, 3 Init 0x00E006D0, 4 OnPersist 0x00E00CE0 -- all landed as CQ_TraderEscortScript); Ego_r.pdb /
//   FableWin.pdb name set ?Main@CQ_TraderEscortScript@NScript@@UAEXXZ; every bound name is an entity script of this class
//   (the fourteen Alloc targets are the landed CQ_TraderEscortScript::C*::Alloc rows) and the two spawned threads are this
//   class's WatchForSurprisingBalverines 0x00E06050 / WatchForMissionRules 0x00E06440. The ledger name
//   "CV_TravellingHeroesScript::Main" was a propagated guess.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012DF40C), CScriptBase::PostAddScriptedEntities
//   0x00CB8930, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_TraderEscortScript> vtable 0x012DF224),
//   CScriptBase::AddSpawnedFunction 0x00CB7E50, CScriptBase::IsActiveThreadTerminating 0x00CB7940;
//   CGameScriptInterfaceBase slots 7 NewScriptFrame (+0x1c), 64 PostAddScriptedEntities (+0x100), 226 HeroHasExpression
//   (+0x388), 296 SetQuestCardObjective (+0x4a0), 303 AddBoast (+0x4bc), 655 GetActiveQuestName (+0xa3c, by value);
//   pGlobalScriptDef 0x0143E90C (TE_AllTradersAliveBoastCost +0xe04 / Reward +0xe08 -- retail reuses the all-traders-alive
//   cost and reward for the pickpocket boast).
// Layout (retail = Ego_r.pdb - 0x14, engine/CQ_TraderEscortScript.h): QuestStartScreened +0x6e.
// Shape: the landed quest-Main model (CQ_SecretPassageScript::Main 0x00DECAD0): one flag word (ebx, memory home zeroed at
//   entry) guards each name temporary that exists only when its operator new succeeded; the binding flag 1 lives in ebp.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_TraderEscortScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/fable_script_def_global.h"
void NScript::CQ_TraderEscortScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("DarkwoodTrader", this, &CDarkwoodTrader::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("FleshEatingBalverine", this, &CFleshEatingBalverine::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("InfectedBalverine", this, &CInfectedBalverine::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("TraderComment", this, &CTraderComment::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("DarkwoodAssassinSpawn", this, &CDarkwoodAssassinSpawn::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("DarkwoodBalverineTrader", this, &CDarkwoodBalverineTrader::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("RockTrollTrigger", this, &CRockTrollTrigger::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("DarkwoodRockTroll", this, &CDarkwoodRockTroll::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("MagicBarrier", this, &CMagicBarrier::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("TE_BanditLeader", this, &CTE_BanditLeader::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("BanditCronies", this, &CBanditCronies::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("DW5_Balv", this, &CDW5_Balv::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("DW5_Bandit", this, &CDW5_Bandit::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderEscortScript>("EndTrader", this, &CEndTrader::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_01", "Darkwood1", "Greatwood");
    AddSpawnedFunction(new CSpawnedFunc<CQ_TraderEscortScript>("WatchForSurprisingBalverines", 0, this, &CQ_TraderEscortScript::WatchForSurprisingBalverines), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_TraderEscortScript>("WatchForMissionRules", 0, this, &CQ_TraderEscortScript::WatchForMissionRules), CCharString(""));
    while (!QuestStartScreened) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Game->HeroHasExpression("EXPRESSION_PICKPOCKET")) {
            if (IsActiveThreadTerminating()) return;
            Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_PICKPOCKETTRADERS", 10, pGlobalScriptDef->TE_AllTradersAliveBoastCost, pGlobalScriptDef->TE_AllTradersAliveBoastReward, true, Game->GetActiveQuestName(), 0);
            break;
        }
    }
    if (IsActiveThreadTerminating()) return;
}