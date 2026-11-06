// NScript::CQ_AmbushTradersScript::Main() -- the Ambush Traders quest main: bind the ten entity scripts, post them, spawn the nine watcher threads, set the first objective, spawn DoQuest, wait for the quest to fail or succeed (fail it with the contact-dead / spy-dead / spy-escaped reason, or complete it and wait for the spy to leave or a region change), spawn WatchForRegionClear, wait for the region to clear or change, deactivate the three phase sub-quests and the quest, and switch the jail / gibbet woods / deserted farm creature generators back on
// Leaf: ?Main@CQ_AmbushTradersScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F024C0, 3503 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00F0326E.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012C6CAC whose slot 3 is the landed CQ_AmbushTradersScript::Init 0x00CF9A50
//   (the "Q_AmbushTraders" Alloc 0x00CFA2F0 calls the ctor 0x00CF9350 that stores it); Ego_r.pdb name set ?Main@CQ_AmbushTradersScript@NScript@@UAEXXZ.
//   Retail compiled it in the second AmbushTraders TU (0x00F024C0..0x00F0FAxx: the entity scripts and threads), not with Init.
//   Every bound Alloc is a landed CQ_AmbushTradersScript::CAT_*::Alloc row (binding-name literal = class name less its `C`); the eleven threads
//   are this class's QAEXXZ methods named by their thread-name literals (Ego_r.pdb name set).
// Callees: operator new 0x00BFEA1A, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CWideString(const unsigned short*) 0x0099B6B0 / ~ 0x0099B510, CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities
//   0x00CB8930 / IsActiveThreadTerminating 0x00CB7940 / AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase 0x00CDD450
//   (CSpawnedFunc<CQ_AmbushTradersScript> vtable 0x012C6CC0); CGameScriptInterfaceBase slots 7, 22 MsgOnRegionLoaded, 64, 281 DeactivateQuestLater,
//   288 SetQuestAsCompleted, 289 SetQuestAsFailed, 296 SetQuestCardObjective, 558 SetCreatureGeneratorsEnabled, 655 GetActiveQuestName.
// Layout: engine/CQ_AmbushTradersScript.h (QuestFailed +0x48, QuestSucceeded +0x4a, ContactDead +0x55, SpyDead +0x58, SpyEscaped +0x60,
//   SpyExited +0x61, RegionClear +0x64) with the opt-in FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN declarations (threads, nested classes).
// Shape: the landed quest-Main binding model (CQ_TraderConflictGoodScript::Main 0x00DFA450) with WAIT_UNTIL loops; the region local is the
//   landed DoQuest 0x00F0B130 MsgOnRegionLoaded(region) shape.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

namespace NScript {
#define FABLE_AT_ENTITY(Name) \
    class CQ_AmbushTradersScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_AmbushTradersScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_AT_ENTITY(CAT_BanditAlly)              // Alloc 0x00F037A0
FABLE_AT_ENTITY(CAT_BanditContact)           // Alloc 0x00F057E0
FABLE_AT_ENTITY(CAT_BanditContactEnd)        // Alloc 0x00F056A0
FABLE_AT_ENTITY(CAT_Phase1Guard)             // Alloc 0x00F038A0
FABLE_AT_ENTITY(CAT_Phase1Archer)            // Alloc 0x00F03F20
FABLE_AT_ENTITY(CAT_Phase3Guard)             // Alloc 0x00F050E0
FABLE_AT_ENTITY(CAT_Phase3Archer)            // Alloc 0x00F04BD0
FABLE_AT_ENTITY(CAT_ConvoyMember)            // Alloc 0x00F04280
FABLE_AT_ENTITY(CAT_ConvoySpy)               // Alloc 0x00F04360
FABLE_AT_ENTITY(CAT_Sentry)                  // Alloc 0x00F04510
#undef FABLE_AT_ENTITY
}

void NScript::CQ_AmbushTradersScript::Main()
{
    CCharString region;
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AmbushTradersScript>("AT_BanditAlly", this, &CAT_BanditAlly::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AmbushTradersScript>("AT_BanditContact", this, &CAT_BanditContact::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AmbushTradersScript>("AT_BanditContactEnd", this, &CAT_BanditContactEnd::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AmbushTradersScript>("AT_Phase1Guard", this, &CAT_Phase1Guard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AmbushTradersScript>("AT_Phase1Archer", this, &CAT_Phase1Archer::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AmbushTradersScript>("AT_Phase3Guard", this, &CAT_Phase3Guard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AmbushTradersScript>("AT_Phase3Archer", this, &CAT_Phase3Archer::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AmbushTradersScript>("AT_ConvoyMember", this, &CAT_ConvoyMember::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AmbushTradersScript>("AT_ConvoySpy", this, &CAT_ConvoySpy::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_AmbushTradersScript>("AT_Sentry", this, &CAT_Sentry::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForQuestFailure", 0, this, &CQ_AmbushTradersScript::WatchForQuestFailure), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForQuestSuccess", 0, this, &CQ_AmbushTradersScript::WatchForQuestSuccess), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForBoastSuccess", 0, this, &CQ_AmbushTradersScript::WatchForBoastSuccess), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForHeroDeath", 0, this, &CQ_AmbushTradersScript::WatchForHeroDeath), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForAllGuardsDead", 0, this, &CQ_AmbushTradersScript::WatchForAllGuardsDead), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForAllTradersDead", 0, this, &CQ_AmbushTradersScript::WatchForAllTradersDead), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForBanditSpeechTimer", 0, this, &CQ_AmbushTradersScript::WatchForBanditSpeechTimer), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForSpySpeechTimer", 0, this, &CQ_AmbushTradersScript::WatchForSpySpeechTimer), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForRegionChange", 0, this, &CQ_AmbushTradersScript::WatchForRegionChange), CCharString(""));
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_AMBUSH_TRADERS_OBJECTIVE_01", "BowerstonePosh", "BowerstoneSlums");
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("DoQuest", 0, this, &CQ_AmbushTradersScript::DoQuest), CCharString(""));
    while (!QuestFailed && !QuestSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (QuestFailed) {
        if (IsActiveThreadTerminating())
            return;
        if (ContactDead) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_003_QUEST_FAILED_CONTACT_DEAD", true);
        } else if (SpyDead) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_003_QUEST_FAILED_SPY_DEAD", true);
        } else if (SpyEscaped) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_003_QUEST_FAILED_SPY_ESCAPED", true);
        } else if (IsActiveThreadTerminating()) {
            return;
        }
    } else if (QuestSucceeded) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
        while (!SpyExited && !Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForRegionClear", 0, this, &CQ_AmbushTradersScript::WatchForRegionClear), CCharString(""));
    while (!RegionClear && !Game->MsgOnRegionLoaded(region)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater("Q_AmbushTraders_ActivatePhase1", 0);
    Game->DeactivateQuestLater("Q_AmbushTraders_ActivatePhase2", 0);
    Game->DeactivateQuestLater("Q_AmbushTraders_ActivatePhase3", 0);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->SetCreatureGeneratorsEnabled("BowerstoneJail", true);
    Game->SetCreatureGeneratorsEnabled("GibbetWoods", true);
    Game->SetCreatureGeneratorsEnabled("DesertedFarm", true);
}