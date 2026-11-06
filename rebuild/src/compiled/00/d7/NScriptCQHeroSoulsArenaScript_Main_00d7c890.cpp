// NScript::CQ_HeroSoulsArenaScript::Main() -- the Hero Souls (arena) quest main: bind the ten entity scripts, post them, on a fresh start wait for the arena exterior and make the hall-of-heroes door usable and persistent, spawn WatchForAudioInArena, play the arena guards cut-scene once in the Hall of Heroes, spawn WatchArenaCellsToTurnOffVillage, have Jack talk to the hero once, open both cell exits in the arena cells, then until Thunder is killed: on first entering the arena play the start cut-scene, hand over the quest card, start screen, objective and Witchwood theme, teleport the one or two bodyguards to their markers, fade in and spawn WatchForRoundsInArena; once the soul is collected play the success cut-scene, reset the theme, log the story entry and complete the quest
// Leaf: ?Main@CQ_HeroSoulsArenaScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D7C890, 3736 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00D7D727.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012D3994 that the "Q_HeroSoulsArena" Alloc 0x00D80BA0 stores; Ego_r.pdb
//   name set ?Main@CQ_HeroSoulsArenaScript@NScript@@UAEXXZ. Every bound Alloc is a landed CQ_HeroSoulsArenaScript::C*::Alloc row (binding-name
//   literal = class name less its `C`); the threads are this class's WatchForAudioInArena 0x00D7D730, WatchArenaCellsToTurnOffVillage
//   0x00D7D8D0 and WatchForRoundsInArena 0x00D7FE90 (Ego_r.pdb QAEXXZ methods named by their thread-name literals).
// Callees: operator new 0x00BFEA1A, free 0x00BFEA14, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   ~CScriptThing 0x004AA840, std::vector<CScriptThing>::~vector 0x008AC970 (expanded on the main path), CScriptBase::AddEntityScriptBinding
//   0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / IsActiveThreadTerminating 0x00CB7940 / AddSpawnedFunction 0x00CB7E50,
//   CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_HeroSoulsArenaScript> vtable 0x012D39A8), CQ_HeroSoulsArenaScript::PlayCutscene 0x00D7FC70,
//   AddLogbookStoryEntryString 0x00CBE960 (__fastcall); CGameScriptInterfaceBase slots 7, 12, 64, 70 GetHero, 72 GetThingWithScriptName,
//   75 GetAllThingsWithScriptName, 288 SetQuestAsCompleted, 295 GiveHeroQuestCardDirectly, 296 SetQuestCardObjective,
//   309 KickOffQuestStartScreen, 364 AddNewConversation, 366 AddLineToConversation, 374 FadeScreenIn, 376 Pause, 447 SetThingAsUsable,
//   472 EntityTeleportToThing, 477 SetThingPersistent, 655 GetActiveQuestName, 656 TransitionToTheme, 657 ResetToDefaultTheme.
// Layout: engine/CQ_HeroSoulsArenaScript.h (repo, unchanged): PlayedArenaCS +0x4c, JackTauntNearHOH +0x50, FirstWaveAttack +0x51,
//   PlayedHoHCS +0x52, ArenaLevelReached +0x54; master data KilledThunder +0x63, CollectedSoulFromArena +0x64.
// Shape: the landed quest-Main binding model (CQ_TraderConflictGoodScript::Main 0x00DFA450) with WAIT_UNTIL loops (loop, then a
//   trailing terminate check).
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
#include "engine/CQ_HeroSoulsArenaScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
void __fastcall AddLogbookStoryEntryString(const CCharString& text);   // 0x00CBE960

void NScript::CQ_HeroSoulsArenaScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsArenaScript>("NotForODG", this, &CNotForODG::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsArenaScript>("SummonerToKill", this, &CSummonerToKill::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsArenaScript>("ArenaHeroGate", this, &CArenaHeroGate::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsArenaScript>("HOHGuard", this, &CHOHGuard::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsArenaScript>("Summoner1", this, &CSummoner1::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsArenaScript>("Summoner2", this, &CSummoner2::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsArenaScript>("HeroSoulsEnemy", this, &CHeroSoulsEnemy::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsArenaScript>("BlockedEntrance", this, &CBlockedEntrance::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsArenaScript>("DyingVillager", this, &CDyingVillager::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsArenaScript>("HSA_ScreamingVillager", this, &CHSA_ScreamingVillager::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (ArenaLevelReached == ALR_NOWHERE) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("ArenaExterior")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptThing door = Game->GetThingWithScriptName("ArenaExtToHallOfHeroes");
        Game->SetThingAsUsable(door, true);
        Game->SetThingPersistent(door, true);
        ArenaLevelReached = ALR_ARENA_EXTERIOR;
    }
    AddSpawnedFunction(new CSpawnedFunc<CQ_HeroSoulsArenaScript>("WatchForAudioInArena", 0, this, &CQ_HeroSoulsArenaScript::WatchForAudioInArena), CCharString(""));
    if (!PlayedHoHCS) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("ArenaHallOfHeroes")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_HEROSOULS_SOUL1_ARENA_GUARDS");
        PlayedHoHCS = true;
        ArenaLevelReached = ALR_ARENA_HOH;
    }
    AddSpawnedFunction(new CSpawnedFunc<CQ_HeroSoulsArenaScript>("WatchArenaCellsToTurnOffVillage", 0, this, &CQ_HeroSoulsArenaScript::WatchArenaCellsToTurnOffVillage), CCharString(""));
    if (!JackTauntNearHOH) {
        if (IsActiveThreadTerminating())
            return;
        Game->Pause(3.0f);
        long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_PRE_ARENA", false, Game->GetHero(), Game->GetHero());
        JackTauntNearHOH = true;
    }
    if (ArenaLevelReached == ALR_ARENA_HOH) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("ArenaCells")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetThingAsUsable(Game->GetThingWithScriptName("CellsExitToArena"), true);
        Game->SetThingAsUsable(Game->GetThingWithScriptName("CellsExitToHOH"), true);
        Game->SetThingPersistent(Game->GetThingWithScriptName("CellsExitToArena"), true);
        Game->SetThingPersistent(Game->GetThingWithScriptName("CellsExitToHOH"), true);
        ArenaLevelReached = ALR_ARENA_CELLS;
    }
    while (!MasterScriptData->KilledThunder) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!PlayedArenaCS && Game->IsRegionLoaded("Arena")) {
            if (IsActiveThreadTerminating())
                return;
            PlayCutscene("CS_HEROSOULS_SOUL1_ARENA_START");
            PlayedArenaCS = true;
            Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_SOULS_ARENA", Game->GetActiveQuestName(), true);
            Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
            Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_04", "Arena", "KnotholeGlade");
            Game->TransitionToTheme("ENVIRONMENT_WITCHWOOD", 0.5f);
            std::vector<CScriptThing> bodyguards;
            Game->GetAllThingsWithScriptName("BodyGuard", bodyguards);
            if (bodyguards.size() == 1) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntityTeleportToThing(bodyguards[0], Game->GetThingWithScriptName("Bodyguard1"), false);
            }
            if (bodyguards.size() == 2) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntityTeleportToThing(bodyguards[0], Game->GetThingWithScriptName("Bodyguard1"), false);
                Game->EntityTeleportToThing(bodyguards[1], Game->GetThingWithScriptName("Bodyguard2"), false);
            }
            Game->FadeScreenIn();
            Game->Pause(0.5f);
            FirstWaveAttack = true;
            AddSpawnedFunction(new CSpawnedFunc<CQ_HeroSoulsArenaScript>("WatchForRoundsInArena", 0, this, &CQ_HeroSoulsArenaScript::WatchForRoundsInArena), CCharString(""));
        }
        if (MasterScriptData->CollectedSoulFromArena) {
            if (IsActiveThreadTerminating())
                return;
            PlayCutscene("CS_HEROSOULS_SOUL1_ARENA_SUCCESS");
            Game->ResetToDefaultTheme(0.2f);
            break;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (MasterScriptData->CollectedSoulFromArena) {
        if (IsActiveThreadTerminating())
            return;
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_350");
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, true);
    }
}