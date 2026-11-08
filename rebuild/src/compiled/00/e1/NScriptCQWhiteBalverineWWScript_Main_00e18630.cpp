// NScript::CQ_WhiteBalverineWWScript::Main() -- the Witchwood White Balverine quest main: bind KG_Chief / the white and soldier balverines, post them, switch the Witchwood4 generators off, wait for the WitchWood_7 level, set objective 07, fetch the balverine, keep it persistent, spawn MonitorBalverine / SpawnBalverines, play the CS_WBW_DRINK cut-scene with the balverine cast as BALV, take it under scripted control and wait until the hero is within 8 units or hits it, start the boss music and aim it at the hero, then wait for the mission to fail or succeed (failure fails the quest, success marks WhiteBalverineFinished) and deactivate the quest
// Leaf: ?Main@CQ_WhiteBalverineWWScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E18630, 2150 bytes, thiscall, plain `ret`, no EH frame; ends in its `ret` at 0x00E18E95.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012E0CE4 (slot 1 the landed RegisterMain 0x00E18570, slot 3 the landed
//   Init 0x00E18480); Ego_r.pdb name set ?Main@CQ_WhiteBalverineWWScript@NScript@@UAEXXZ. The bound Allocs are the landed
//   CKG_Chief / CWBWW_WhiteBalverine / CWBWW_SoldierBalverine::Alloc rows (binding literal = class name less its `C`); the threads are
//   MonitorBalverine 0x00E18EB0 (landed) and SpawnBalverines 0x00E19560 (Ego_r ?SpawnBalverines@CQ_WhiteBalverineWWScript@NScript@@QAEXXZ,
//   paired with the "SpawnBalverines" literal; unlabelled before).
// Callees: operator new 0x00BFEA1A, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CWideString 0x0099B6B0 / ~ 0x0099B510,
//   ~CScriptThing 0x004AA840, CScriptBase AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 /
//   IsActiveThreadTerminating 0x00CB7940 / AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_WhiteBalverineWWScript>
//   vtable 0x012E0CF8), CBaseObject 0x0099A380 / ~ 0x0099A430 / operator= 0x0099A3B0, malloc 0x00BFEA0E (the cast map's header node),
//   map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[] 0x00CD3D2E / ~map 0x00CDBFB0, ~ScriptedThingBase 0x007E74D0,
//   handle IsValid 0x00CD23B9 / Release 0x00CD2770, ~CCountedPointer<movie> 0x006E7AB0, RunCutsceneMacro 0x00CBFB7D and
//   IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); CGameScriptInterfaceBase slots 7 NewScriptFrame, 8 StartScriptingEntity,
//   11 IsLevelLoaded, 64 PostAddScriptedEntities, 70 GetHero, 72 GetThingWithScriptName, 281 DeactivateQuestLater, 289 SetQuestAsFailed,
//   296 SetQuestCardObjective, 321 DisplayQuestInfo, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 451 GiveThingBestEnemyTarget,
//   477 SetThingPersistent, 558 SetCreatureGeneratorsEnabled, 655 GetActiveQuestName, 696 OverrideMusic; CScriptThing slots 21 MsgIsHitBy,
//   41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom.
// Layout: engine/CQ_WhiteBalverineWWScript.h (new, chunk 797); MasterScriptData->WhiteBalverineFinished +0xe1 (engine/CQ_SunnyvaleMasterData.h).
// Shape: the landed quest-Main binding model (CQ_TraderConflictGoodScript::Main 0x00DFA450) plus the cut-scene block of the landed
//   CMazeAtTavern::Main 0x00E26E30 (cast map with the inline ctor and the out-of-line ~map, inline movie ctor / dtor with the PImp dtor
//   call, inline CWideScreenMagicPauseEntities). The unused "MK_WBW_FIRSTSPAWNB" thing is a named local that lives to the end (retail
//   destroys it at the last exit); the member assignment is the implicit CScriptThing operator= (o.Data / o.Info loaded first).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_WhiteBalverineWWScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
#include "engine/EMusicSetType.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
FABLE_FORCEINLINE CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp = other.PImp; return *this; }
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0

namespace NScript {
#define FABLE_WBWW_ENTITY(Name) \
    class CQ_WhiteBalverineWWScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_WhiteBalverineWWScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_WBWW_ENTITY(CKG_Chief)                 // Alloc 0x00E192E0
FABLE_WBWW_ENTITY(CWBWW_WhiteBalverine)      // Alloc 0x00E190A0
FABLE_WBWW_ENTITY(CWBWW_SoldierBalverine)    // Alloc 0x00E191E0
#undef FABLE_WBWW_ENTITY
}

void NScript::CQ_WhiteBalverineWWScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WhiteBalverineWWScript>("KG_Chief", this, &CKG_Chief::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WhiteBalverineWWScript>("WBWW_WhiteBalverine", this, &CWBWW_WhiteBalverine::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WhiteBalverineWWScript>("WBWW_SoldierBalverine", this, &CWBWW_SoldierBalverine::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetCreatureGeneratorsEnabled("Witchwood4", false);
    while (!Game->IsLevelLoaded("WitchWood_7")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective("Q_WhiteBalverineKnotholeGlade", "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_07", "Witchwood4", "KnotholeGlade");
    CScriptThing first_spawn = Game->GetThingWithScriptName("MK_WBW_FIRSTSPAWNB");
    WhiteBalverine = Game->GetThingWithScriptName("WBWW_WhiteBalverine");
    Game->SetThingPersistent(WhiteBalverine, true);
    AddSpawnedFunction(new CSpawnedFunc<CQ_WhiteBalverineWWScript>("MonitorBalverine", 0, this, &CQ_WhiteBalverineWWScript::MonitorBalverine), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_WhiteBalverineWWScript>("SpawnBalverines", 0, this, &CQ_WhiteBalverineWWScript::SpawnBalverines), CCharString(""));
    {
        CScriptGameResourceObjectScriptedThingBase seh_balverine;
        Game->StartScriptingEntity(WhiteBalverine, seh_balverine, SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["BALV"] = seh_balverine;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        RunCutsceneMacro("CS_WBW_DRINK", mymap, 0, 0, false, true);
    }
    CScriptGameResourceObjectScriptedThingBase seh_balverine;
    Game->StartScriptingEntity(WhiteBalverine, seh_balverine, SCRIPT_AI_PRIORITY_HIGHEST);
    while (!(IsDistanceBetweenThingsUnder(Game->GetHero(), WhiteBalverine, 8.0f) || WhiteBalverine.MsgIsHitBy("SCRIPT_NAME_HERO") ||
             (WhiteBalverine.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !WhiteBalverine.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (seh_balverine.IsValid())
        seh_balverine.Release();
    Game->OverrideMusic(MUSIC_SET_BOSS, false, false);
    Game->GiveThingBestEnemyTarget(WhiteBalverine, Game->GetHero());
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating())
            return;
        MasterScriptData->WhiteBalverineFinished = true;
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"", true);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->DisplayQuestInfo(false);
}