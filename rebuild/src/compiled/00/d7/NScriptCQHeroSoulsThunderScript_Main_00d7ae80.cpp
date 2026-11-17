// CQ_HeroSoulsThunderScript::Main() -- Hero Souls (Thunder): bind HeroThunder, wait for Knothole Glade; on first run open the KG gate, pick the start line, play the start cut-scene, switch the arena quests/sound themes/objective and fade in; have Jack speak once, wait for Thunder's defeat, then play the success cut-scene, give morality, log the story and complete the quest
// Leaf: ?Main@CQ_HeroSoulsThunderScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D7AE80, 1392 bytes (int3 padded).
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0 / operator=(const char*) 0x0099EFE0, operator new 0x00BFEA1A,
//   AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012D364C, allocator 0x00D7B400), PostAddScriptedEntities 0x00CB8930,
//   IsActiveThreadTerminating 0x00CB7940, ~CScriptThing 0x004AA840, map<CCharString,CCharString>::operator[] 0x009AC700,
//   PlayCutscene 0x00D7C470 (by-value name), AddLogbookStoryEntryString 0x00CBE960 (__fastcall), pGlobalScriptDef 0x0143E90C;
//   CGameScriptInterface slots 7, 12, 64, 70, 72, 156, 276, 280, 288, 296, 364, 366, 374, 425, 655, 665.
// NF87 (2026-11-17): moved onto the shared engine/CQ_HeroSoulsThunderScript.h (CLevelScriptBase<CQ_SunnyvaleMasterData>: Game +0x40,
//   MasterScriptData +0x44; PlayedThunderCS +0x48, QuestStartScreened +0x49, JackSpoken +0x4a, csargs +0x4c as the STLport
//   map<CCharString, CCharString>, so the three operator[] calls bind 0x009AC700 under its real spelling), engine/CEntityScriptBinding.h
//   and engine/CQ_SunnyvaleMasterData.h (KilledThunder +0x63, CurrentHeroSoulsPosition +0x70, WhisperKilledByHero +0x74,
//   HeroMarriedLadyGrey +0xd7), in the shape of the landed sibling 0x00D84D60 (CQ_HeroSoulsGuildmasterScript::Main).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_map.h"
#include "engine/CQ_HeroSoulsThunderScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
void __fastcall AddLogbookStoryEntryString(const CCharString& text);   // 0x00CBE960

void NScript::CQ_HeroSoulsThunderScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsThunderScript>("HeroThunder", this, &CHeroThunder::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsRegionLoaded("KnotholeGlade")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!PlayedThunderCS && !QuestStartScreened) {
        if (IsActiveThreadTerminating())
            return;
        Game->OpenDoor(Game->GetThingWithScriptName("KG_Gate"));
        if (MasterScriptData->WhisperKilledByHero) {
            if (IsActiveThreadTerminating())
                return;
            csargs["$LINE1"] = "TEXT_CS_B04_SOUL1_THUNDER_START_40_A";
        } else if (MasterScriptData->HeroMarriedLadyGrey) {
            if (IsActiveThreadTerminating())
                return;
            csargs["$LINE1"] = "TEXT_CS_B04_SOUL1_THUNDER_START_40_B";
        } else {
            if (IsActiveThreadTerminating())
                return;
            csargs["$LINE1"] = "TEXT_CS_B04_SOUL1_THUNDER_START_40_C";
        }
        PlayCutscene("CS_HEROSOULS_SOUL1_THUNDER_START");
        PlayedThunderCS = true;
        Game->ActivateQuest("Q_HeroSoulsArena");
        Game->ActivateQuest("Q_HeroSoulsPermanentArena");
        Game->DeactivateQuest("Q_ArenaHoldingScript", 0);
        Game->SetSoundThemesAsEnabledForRegion("ArenaExterior", false);
        Game->SetSoundThemesAsEnabledForRegion("ArenaCells", false);
        Game->SetSoundThemesAsEnabledForRegion("Arena", false);
        MasterScriptData->CurrentHeroSoulsPosition = EPIHS_1ST_SOUL_VISIT_THE_ARENA;
        Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_03", "Arena", "KnotholeGlade");
        Game->FadeScreenIn();
    }
    if (!JackSpoken) {
        if (IsActiveThreadTerminating())
            return;
        long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_PRE_THUNDER_10", false, Game->GetHero(), Game->GetHero());
        if (MasterScriptData->WhisperKilledByHero) {
            if (IsActiveThreadTerminating())
                return;
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_PRE_THUNDER_20", false, Game->GetHero(), Game->GetHero());
        }
        JackSpoken = true;
    }
    while (!MasterScriptData->KilledThunder) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    PlayCutscene("CS_HEROSOULS_SOUL1_THUNDER_SUCCESS");
    Game->GiveHeroMorality(pGlobalScriptDef->HS_GoodSoulTaken);
    AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_355");
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, true);
}
