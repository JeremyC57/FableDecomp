// NScript::CQ_DragonBossFightScript::DoMission() -- the dragon boss fight mission thread: set the first objective, wait for DragonCliff2, fade out, put the hero's followers in limbo, play the intro cut-scene with the boss music, set the second objective and the quest start screen, fade in, show the dragon's health bar, spawn the enemy-spawning and Jack-taunt threads, wait for the dragon to die, then play the death and mask-choice cut-scenes and, by the PutMaskOn cut-scene flag, the evil (mask worn) or good outro: logbook entry, HeroWoreMask, followers out of limbo, quest completed, four frames, the morality-dependent ending movie, the morality change, then stop the music and deactivate the quest
// Leaf: ?DoMission@CQ_DragonBossFightScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D26190, 2235 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00D26A4A.
// Identity (proven): Ego_r.pdb name set ?DoMission@CQ_DragonBossFightScript@NScript@@QAEXXZ; the landed CQ_DragonBossFightScript::Main
//   0x00D255E0 spawns it under the "DoMission" thread literal; it calls the landed PlayCutscene 0x00D27050 five times (that row's five
//   proven call sites 0x00D26348 / 0x00D2662A / 0x00D2666B / 0x00D266ED / 0x00D26887 all lie in this body) and reads the class's
//   DragonState +0x48 and cs_flags +0x84.
// Callees: operator new 0x00BFEA1A, operator delete 0x00BFE9BC (inline ~CScriptThing PImp release), CCharString(const char*, long)
//   0x0099EBF0 / ~ 0x0099EAE0, ~CBaseClass 0x0099A2E0, CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddSpawnedFunction
//   0x00CB7E50, CSpawnedFuncBase 0x00CDD450, CQ_DragonBossFightScript::PlayCutscene 0x00D27050, AddLogbookStoryEntryString 0x00CBE960
//   (__fastcall), std::map<CCharString,bool>::operator[] 0x008ADF10, std::vector<CScriptThing>::~vector 0x008AC970 (out of line, the
//   one shared exit); the threads RunEnemySpawning 0x00D26BA0 ("RunEnemySpawning", Ego_r.pdb ?RunEnemySpawning@CQ_DragonBossFight
//   Script@NScript@@QAEXXZ; no catalog row yet) and JackTaunts 0x00D26A50 ("JackTaunts", landed); CGameScriptInterfaceBase slots
//   7 NewScriptFrame, 12 IsRegionLoaded, 72 GetThingWithScriptName, 84 GetAllCreaturesExcludingHero, 156 GiveHeroMorality,
//   157 GetHeroMorality, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 296 SetQuestCardObjective, 309 KickOffQuestStartScreen,
//   321 DisplayQuestInfo, 325 AddQuestInfoBarHealth, 369 PlayAVIMovie, 372 FadeScreenOutUntilNextCallToFadeScreenIn, 374 FadeScreenIn,
//   389 SetCutsceneActionMode, 453 EntitySetInLimbo, 565 SetTeleportingAsActive, 655 GetActiveQuestName, 696 OverrideMusic,
//   697 StopOverrideMusic (ghidra_out/cgsi_slots_aligned.tsv). The 28 string literals were paired with retail by relocation (strchk).
// Layout: engine/CQ_DragonBossFightScript.h (new, chunk 790); CQ_SunnyvaleMasterData BodyGuardsInLimbo +0xe4, HeroWoreMask +0x133.
// Shape: the quest-thread model (the WAIT_UNTIL loops of the landed quest Mains). The dragon health bar's GetThingWithScriptName
//   temporary is destroyed with an inline ~CScriptThing (vtable store, PImp release, ~CBaseClass). The local vector's destructor is
//   declared out of line (GOTCHAS chunk 789): with the inline STLport body VC7.1 expands it at the shared exit (2292 bytes). The
//   terminate test of each morality arm sits in both arms (VC7.1 hoists the `mov ecx,esi` above the branch, GOTCHAS chunk 781).
//   Retail differs from Aeon's FSE port here: no dragon-alive polling (it waits on DragonState == 4), no RemoveThing sweep, the mask
//   choice runs through PlayCutscene("CS_DRAGON_OUTRO_CHOICE"), and the ending is GetHeroMorality() < 0.5 (evil movie) / else (good).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/EMusicSetType.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_DragonBossFightScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
void __fastcall AddLogbookStoryEntryString(const CCharString& text);   // 0x00CBE960
inline CScriptThing::~CScriptThing() { }
template <> std::vector<CScriptThing, std::allocator<CScriptThing> >::~vector();   // 0x008AC970 (out of line in retail)

void NScript::CQ_DragonBossFightScript::DoMission()
{
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_DRAGON_BOSS_OBJECTIVE_01", "DragonCliff2", "NorthernWastes3");
    while (!Game->IsRegionLoaded("DragonCliff2")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.0f);
    MasterScriptData->BodyGuardsInLimbo = true;
    std::vector<CScriptThing> creatures;
    Game->GetAllCreaturesExcludingHero(creatures);
    for (unsigned long i = 0; i < creatures.size(); ++i)
        Game->EntitySetInLimbo(creatures[i], true, true);
    PlayCutscene("CS_DRAGON_INTRO");
    Game->OverrideMusic(MUSIC_SET_BOSS, false, false);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_DRAGON_BOSS_OBJECTIVE_02", "DragonCliff2", "NorthernWastes3");
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    Game->FadeScreenIn();
    Game->SetTeleportingAsActive(false);
    Game->DisplayQuestInfo(true);
    Game->AddQuestInfoBarHealth(Game->GetThingWithScriptName("Dragon"), CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_DRAGON", 1.0f);
    AddSpawnedFunction(new CSpawnedFunc<CQ_DragonBossFightScript>("RunEnemySpawning", 0, this, &CQ_DragonBossFightScript::RunEnemySpawning), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_DragonBossFightScript>("JackTaunts", 0, this, &CQ_DragonBossFightScript::JackTaunts), CCharString(""));
    while (DragonState != 4) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetTeleportingAsActive(true);
    PlayCutscene("CS_DRAGON_DEATH");
    Game->SetCutsceneActionMode(true, "TEXT_QST_B05_MASK_CHOICE");
    PlayCutscene("CS_DRAGON_OUTRO_CHOICE");
    Game->SetCutsceneActionMode(false, "");
    bool putMaskOn = cs_flags["PutMaskOn"];
    if (putMaskOn) {
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_DRAGON_OUTRO_EVIL");
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_385");
        MasterScriptData->HeroWoreMask = true;
        for (unsigned long i = 0; i < creatures.size(); ++i)
            Game->EntitySetInLimbo(creatures[i], false, true);
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHeroMorality() < 0.5f) {
            if (IsActiveThreadTerminating())
                return;
            Game->PlayAVIMovie("Data\\Video\\dragon_evil_mask.xmv");
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->PlayAVIMovie("Data\\Video\\dragon_good_mask.xmv");
        }
        Game->GiveHeroMorality(-1.0f);
    } else {
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_DRAGON_OUTRO_GOOD");
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_380");
        MasterScriptData->HeroWoreMask = false;
        for (unsigned long i = 0; i < creatures.size(); ++i)
            Game->EntitySetInLimbo(creatures[i], false, true);
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHeroMorality() < 0.5f) {
            if (IsActiveThreadTerminating())
                return;
            Game->PlayAVIMovie("Data\\Video\\dragon_evil_no_mask.xmv");
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->PlayAVIMovie("Data\\Video\\dragon_good_no_mask.xmv");
        }
        Game->GiveHeroMorality(0.5f);
    }
    Game->StopOverrideMusic(false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}