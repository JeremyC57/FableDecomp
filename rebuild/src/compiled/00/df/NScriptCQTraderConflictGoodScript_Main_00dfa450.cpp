// NScript::CQ_TraderConflictGoodScript::Main() -- the trader conflict (good) quest main: bind the seven entity scripts, post them, wait for the bandit camp entrance, open Gate1 / Gate2Outer, play the intro cut-scene and quest start screen, spawn WatchTimeLimit, rebuild the enemy list and set the objective, put every enemy into the bandit faction awake with a 10 m break-off range, spawn the four watcher threads, wait for the camp centre (noting when the first region is cleared), open Gate2Inner, wait for the outro, complete the quest and once the hero leaves the entrance deactivate the extras and the quest
// Leaf: ?Main@CQ_TraderConflictGoodScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DFA450, 2640 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00DFAE9F.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012DEAC8; Ego_r.pdb name set ?Main@CQ_TraderConflictGoodScript@NScript@@UAEXXZ.
//   Every bound Alloc is a landed CQ_TraderConflictGoodScript::C*::Alloc row (binding-name literal = class name less its `C`); the threads
//   are this class's WatchTimeLimit 0x00DFAEB0, WatchForRegionTransitions 0x00DFC920, WatchForHittingEnemies 0x00DFC630, WatchForTradersFreed
//   0x00DFCC10 and WatchForKilledPeople 0x00DFC290 (Ego_r.pdb QAEXXZ methods named by their thread-name literals); PlayCutscene 0x00DFDED0 and
//   UpdateLiveEnemies 0x00DFC320 are landed rows.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_TraderConflictGoodScript> vtable 0x012DEB2C), CScriptBase::AddSpawnedFunction
//   0x00CB7E50; CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities, 72 GetThingWithScriptName,
//   280 DeactivateQuest, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 296 SetQuestCardObjective, 309 KickOffQuestStartScreen,
//   374 FadeScreenIn, 425 OpenDoor, 593 EntitySetSleepEnabled, 601 EntitySetInFaction, 655 GetActiveQuestName, 817 SetCombatNearbyBreakOffRange
//   (CScriptThing by value: the copy ctor is expanded in place, as in the landed WatchForRegionTransitions 0x00DFC920).
// Shape: the landed quest-Main binding model (CQ_PrisonEscapeRescueMotherScript::Main 0x00DD2CE0) with WAIT_UNTIL loops (loop, then a
//   trailing terminate check); the zero in ebp doubles as the enemy loop's counter start.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_TraderConflictGoodScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
// Retail expands the CScriptThing copy at the by-value SetCombatNearbyBreakOffRange site (implicit copy shape: CBaseClass vtable, then
//   CScriptThing vtable, PImp copy with the reference-count increment).
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }

namespace NScript {
#define FABLE_TCG_ENTITY(Name) \
    class CQ_TraderConflictGoodScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_TraderConflictGoodScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_TCG_ENTITY(CTraderToRescue)            // Alloc 0x00DFB050
FABLE_TCG_ENTITY(CTC_BanditGuard)            // Alloc 0x00DFB250
FABLE_TCG_ENTITY(CTC_BanditHostageKeeper)    // Alloc 0x00DFB990
FABLE_TCG_ENTITY(CBCMTrader)                 // Alloc 0x00DFBCF0
FABLE_TCG_ENTITY(CBCGameMaster)              // Alloc 0x00DFBDE0
FABLE_TCG_ENTITY(CBanditExtra)               // Alloc 0x00DFBED0
FABLE_TCG_ENTITY(CCampHostageDoor)           // Alloc 0x00DFBFA0
#undef FABLE_TCG_ENTITY
}

void NScript::CQ_TraderConflictGoodScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictGoodScript>("TraderToRescue", this, &CTraderToRescue::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictGoodScript>("TC_BanditGuard", this, &CTC_BanditGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictGoodScript>("TC_BanditHostageKeeper", this, &CTC_BanditHostageKeeper::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictGoodScript>("BCMTrader", this, &CBCMTrader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictGoodScript>("BCGameMaster", this, &CBCGameMaster::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictGoodScript>("BanditExtra", this, &CBanditExtra::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TraderConflictGoodScript>("CampHostageDoor", this, &CCampHostageDoor::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsRegionLoaded("BanditCampEntrance")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->OpenDoor(Game->GetThingWithScriptName("Gate1"));
    Game->OpenDoor(Game->GetThingWithScriptName("Gate2Outer"));
    PlayCutscene("CS_TRADERCON_GOOD_INTRO");
    IntroDone = true;
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    Game->FadeScreenIn();
    AddSpawnedFunction(new CSpawnedFunc<CQ_TraderConflictGoodScript>("WatchTimeLimit", 0, this, &CQ_TraderConflictGoodScript::WatchTimeLimit), CCharString(""));
    UpdateLiveEnemies();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_TRADER_CONFLICT_GOOD_OBJECTIVE_01", "BanditCampCentre", "BanditCampMain");
    for (unsigned int i = 0; i < AllCreatures.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetCombatNearbyBreakOffRange(AllCreatures[i], 10.0f);
        Game->EntitySetSleepEnabled(AllCreatures[i], false);
        Game->EntitySetInFaction(AllCreatures[i], "FACTION_BANDITS");
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CQ_TraderConflictGoodScript>("WatchForRegionTransitions", 0, this, &CQ_TraderConflictGoodScript::WatchForRegionTransitions), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_TraderConflictGoodScript>("WatchForHittingEnemies", 0, this, &CQ_TraderConflictGoodScript::WatchForHittingEnemies), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_TraderConflictGoodScript>("WatchForTradersFreed", 0, this, &CQ_TraderConflictGoodScript::WatchForTradersFreed), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_TraderConflictGoodScript>("WatchForKilledPeople", 0, this, &CQ_TraderConflictGoodScript::WatchForKilledPeople), CCharString(""));
    while (!Game->IsRegionLoaded("BanditCampCentre")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!FirstRegionDead && AllCreatures.size() == 0) {
            if (IsActiveThreadTerminating())
                return;
            FirstRegionDead = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->OpenDoor(Game->GetThingWithScriptName("Gate2Inner"));
    while (!OutroDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
    while (!(!Game->IsRegionLoaded("BanditCampEntrance"))) {   // WAIT_UNTIL(!IsRegionLoaded): the double negation is the retail test + setne
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuest("Q_TraderConflictGood_Extras", 0);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}