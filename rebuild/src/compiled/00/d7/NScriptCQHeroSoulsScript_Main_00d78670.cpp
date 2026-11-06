// NScript::CQ_HeroSoulsScript::Main() -- the Hero Souls quest: bind the NotForODG / EndScythe / EndBriarRose entity scripts, auto-save, play Briar's first message and the opening cutscene, then walk the hero through the four souls (Thunder in the arena, Briar's mother, Briar or Scythe, the Guildmaster at Nostro) with quest-card objectives, the soul-3 cutscenes and Jack's message, mark the quest complete and deactivate it once the dragon boss fight starts
// Leaf: ?Main@CQ_HeroSoulsScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D78670, 5390 bytes, thiscall, `sub esp,0x24` frame, no EH frame, ends in its `ret` at 0x00D79B7D.
// Identity (proven): slot 2 (Main) of the level-script vtable 0x012D2F48 (the only reference to 0x00D78670 is .rdata 0x012D2F50) that
//   the landed CQ_HeroSoulsScript ctor 0x00D784D0 stores; slots 1/3/4 are the landed RegisterMain 0x00D78520 / Init 0x00D78500 /
//   OnPersist 0x00D785E0. Ego_r.pdb / FableWin.pdb name set ?Main@CQ_HeroSoulsScript@NScript@@UAEXXZ.
// Callees: operator new 0x00BFEA1A (three 0x1c-byte CEntityScriptBinding<CQ_HeroSoulsScript>, inline ctor: CCharString copy
//   0x0099EC30), CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / IsActiveThreadTerminating
//   0x00CB7940 (44 sites), CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CQ_HeroSoulsScript::PlayCutscene 0x00D7AA30 (x3,
//   by-value CCharString), AddLogbookStoryEntryString 0x00CBE960 (fastcall, x2), ~CScriptThing 0x004AA840 (the GetThingWithScriptName
//   temporaries for OpenDoor and SetThingPersistent); the nested Alloc thunks 0x00D79B90 / 0x00D79C80 / 0x00D7A290 are stored, not
//   called. CGameScriptInterfaceBase slots: 7 NewScriptFrame, 12 IsRegionLoaded, 25 MsgOnQuestCompleted, 70 GetHero, 72
//   GetThingWithScriptName, 274 HeroReceiveMessageFromGuildMaster, 275 SetGuildMasterMessages, 276 ActivateQuest, 280 DeactivateQuest,
//   284 IsQuestActive, 287 IsQuestFailed, 296 SetQuestCardObjective, 364 AddNewConversation, 366 AddLineToConversation, 374
//   FadeScreenIn, 376 Pause, 425 OpenDoor, 477 SetThingPersistent, 558 SetCreatureGeneratorsEnabled, 655 GetActiveQuestName, 704
//   AutoSaveQuestStart, plus SetGuildSealRecallLocation / KickOffQuestStartScreen / SetQuestAsCompleted / DeactivateQuestLater. The
//   82 string literals were paired with retail by relocation (strchk 82/82); no float constant is loaded from memory (fltchk 0/0).
// Layout: engine/CQ_HeroSoulsScript.h (new this chunk): HeardFirstBriarRoseMessage +0x48 .. AmLoadingGame +0x4d; master data
//   engine/CQ_SunnyvaleMasterData.h (unchanged) through MasterScriptData +0x44: CurrentHeroSoulsPosition, JackBossBattleResult,
//   KilledThunder, CollectedSoulFromArena, KilledBriar, DeliveredSoul and the other hero-souls members by name.
// Shape: generated level-script code: every `if (...)` body opens with `if (IsActiveThreadTerminating()) return;`, every wait is
//   `while (!cond) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }`. Matched on the first grade from the
//   generated-script spelling (ebx holds zero for the whole function, as in retail).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HeroSoulsScript.h"
#include "engine/CEntityScriptBinding.h"
void __fastcall AddLogbookStoryEntryString(const CCharString&);   // 0x00CBE960

namespace NScript {
class CQ_HeroSoulsScript::CNotForODG : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsScript&, CScriptDataBase*, const CScriptThing&);
};
class CQ_HeroSoulsScript::CEndScythe : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsScript&, CScriptDataBase*, const CScriptThing&);
};
class CQ_HeroSoulsScript::CEndBriarRose : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsScript&, CScriptDataBase*, const CScriptThing&);
};
}

void NScript::CQ_HeroSoulsScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsScript>("NotForODG", this, &CNotForODG::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsScript>("EndScythe", this, &CEndScythe::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsScript>("EndBriarRose", this, &CEndBriarRose::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (!AmLoadingGame) {
        if (IsActiveThreadTerminating()) return;
        AmLoadingGame = true;
        Game->AutoSaveQuestStart();
    }
    if (!HeardFirstBriarRoseMessage) {
        if (IsActiveThreadTerminating()) return;
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_B04_BRIAR_MSG1_10", "", true, true);
    }
    Game->SetCreatureGeneratorsEnabled("NorthernWastes3", false);
    if (!PlayedOpeningCS) {
        if (IsActiveThreadTerminating()) return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HERO_SOULS_OBJECTIVE_01", "NorthernWastes3", "NorthernWastes3");
        while (!Game->IsRegionLoaded("NorthernWastes3")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        HeardFirstBriarRoseMessage = true;
        PlayCutscene("CS_HEROSOULS_SOUL1_INTRO");
        PlayedOpeningCS = true;
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), false, false);
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_340");
        Game->AutoSaveQuestStart();
        Game->FadeScreenIn();
        Game->Pause(1.0f);
        long Conversation = Game->AddNewConversation(Game->GetHero(), false, false);
        Game->AddLineToConversation(Conversation, "TEXT_QST_B04_JACK_MSG_FIRST_SOUL_10", false, Game->GetHero(), Game->GetHero());
        if (MasterScriptData->JackBossBattleResult == 1) {
            if (IsActiveThreadTerminating()) return;
            Game->AddLineToConversation(Conversation, "TEXT_QST_B04_JACK_MSG_FIRST_SOUL_20_A", false, Game->GetHero(), Game->GetHero());
        } else {
            if (IsActiveThreadTerminating()) return;
            Game->AddLineToConversation(Conversation, "TEXT_QST_B04_JACK_MSG_FIRST_SOUL_20_B", false, Game->GetHero(), Game->GetHero());
        }
        Game->AddLineToConversation(Conversation, "TEXT_QST_B04_JACK_MSG_FIRST_SOUL_30", false, Game->GetHero(), Game->GetHero());
    }
    MasterScriptData->CurrentHeroSoulsPosition = EPIHS_1ST_SOUL_TALK_TO_THUNDER;
    if (!Game->IsQuestActive("Q_HeroSoulsArena") && !MasterScriptData->KilledThunder && !MasterScriptData->CollectedSoulFromArena) {
        if (IsActiveThreadTerminating()) return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HERO_SOULS_OBJECTIVE_02", "KnotholeGlade", "KnotholeGlade");
    }
    if (!MasterScriptData->KilledThunder && !MasterScriptData->CollectedSoulFromArena) {
        if (IsActiveThreadTerminating()) return;
        Game->ActivateQuest("Q_HeroSoulsThunder");
        while (!Game->MsgOnQuestCompleted("Q_HeroSoulsThunder") && !Game->MsgOnQuestCompleted("Q_HeroSoulsArena")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->DeactivateQuest("Q_HeroSoulsArena", 0);
        Game->DeactivateQuest("Q_HeroSoulsThunder", 0);
        Game->FadeScreenIn();
        if (Game->IsRegionLoaded("KnotholeGlade")) {
            if (IsActiveThreadTerminating()) return;
            Game->OpenDoor(Game->GetThingWithScriptName("KG_Gate"));
        }
    }
    if (MasterScriptData->KilledThunder) {
        if (IsActiveThreadTerminating()) return;
        MasterScriptData->CurrentHeroSoulsPosition = EPIHS_2ND_SOUL_TALK_TO_BRIAR_KILLED_THUNDER;
    } else {
        if (IsActiveThreadTerminating()) return;
        MasterScriptData->CurrentHeroSoulsPosition = EPIHS_2ND_SOUL_TALK_TO_BRIAR_GOT_ARENA_SOUL;
    }
    if (!HeardSecondBriarRoseMessage) {
        if (IsActiveThreadTerminating()) return;
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_B04_BRIAR_MSG2_10", "", true, true);
    }
    HeardSecondBriarRoseMessage = true;
    if (!MasterScriptData->KilledBriar && !MasterScriptData->CollectedSoulFromMother) {
        if (IsActiveThreadTerminating()) return;
        if (!Game->IsQuestActive("Q_HeroSoulsMother")) {
            if (IsActiveThreadTerminating()) return;
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HERO_SOULS_OBJECTIVE_06", "NorthernWastes3", "NorthernWastes3");
        } else {
            if (IsActiveThreadTerminating()) return;
            Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_07", "OakvaleMemorialGarden", "OakBay");
        }
        Game->ActivateQuest("Q_HeroSoulsBriar");
        while (!Game->MsgOnQuestCompleted("Q_HeroSoulsBriar") && !Game->MsgOnQuestCompleted("Q_HeroSoulsMother")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->DeactivateQuest("Q_HeroSoulsBriar", 0);
        Game->DeactivateQuest("Q_HeroSoulsMother", 0);
    }
    if (!MasterScriptData->KilledBriar) {
        if (IsActiveThreadTerminating()) return;
        if (!HeardGMMessage) {
            if (IsActiveThreadTerminating()) return;
            Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_B04_BRIAR_MSG3_10", "", true, true);
            HeardGMMessage = true;
        }
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HERO_SOULS_OBJECTIVE_10", "NorthernWastes3", "NorthernWastes3");
        if (MasterScriptData->DeliveredSoul == 1) {
            if (IsActiveThreadTerminating()) return;
            while (!Game->IsRegionLoaded("NorthernWastes3")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            Game->SetGuildSealRecallLocation(C3DVector(0.0f, 0.0f, 0.0f), 0.0f);
            PlayCutscene("CS_HEROSOULS_SOUL3_INTRO_BRIAR");
            MasterScriptData->DeliveredSoul++;
            Game->FadeScreenIn();
        }
    } else {
        if (IsActiveThreadTerminating()) return;
        if (!HeardGMMessage) {
            if (IsActiveThreadTerminating()) return;
            Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_B04_SCYTHE_MSG_BRIAR_DEAD", "", true, true);
            HeardGMMessage = true;
        }
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HERO_SOULS_OBJECTIVE_11", "HeroGuildComplexInside", "HeroGuildComplexInside");
    }
    if (!MasterScriptData->KilledGM && !MasterScriptData->CollectedSoulFromNostro) {
        if (IsActiveThreadTerminating()) return;
        if (!Game->IsQuestFailed("Q_HeroSoulsGuildmaster")) {
            if (IsActiveThreadTerminating()) return;
            Game->ActivateQuest("Q_HeroSoulsGuildmaster");
            Game->DeactivateQuest("V_GuildMaster", 0);
            Game->SetGuildMasterMessages(false);
        }
        while (!Game->MsgOnQuestCompleted("Q_HeroSoulsGuildmaster") && !Game->MsgOnQuestCompleted("Q_HeroSoulsNostro")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->DeactivateQuest("Q_HeroSoulsGuildmaster", 0);
        Game->DeactivateQuest("Q_HeroSoulsNostro", 0);
        if (!MasterScriptData->KilledGM) {
            if (IsActiveThreadTerminating()) return;
            Game->ActivateQuest("V_GuildMaster");
            Game->SetGuildMasterMessages(true);
            if (MasterScriptData->KilledBriar) {
                if (IsActiveThreadTerminating()) return;
                MasterScriptData->CurrentHeroSoulsPosition = EPIHS_3RD_SOUL_TALK_TO_SCYTHE;
            } else {
                if (IsActiveThreadTerminating()) return;
                MasterScriptData->CurrentHeroSoulsPosition = EPIHS_3RD_SOUL_TALK_TO_BRIAR;
            }
        }
    }
    if (MasterScriptData->KilledGM && !HeardJackMessage) {
        if (IsActiveThreadTerminating()) return;
        long Conversation = Game->AddNewConversation(Game->GetHero(), false, false);
        Game->AddLineToConversation(Conversation, "TEXT_QST_B04_GM_DEAD_SCYTHE_MSG", false, Game->GetHero(), Game->GetHero());
    }
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HERO_SOULS_OBJECTIVE_15", "NorthernWastes3", "NorthernWastes3");
    while (!Game->IsRegionLoaded("NorthernWastes3")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (MasterScriptData->KilledBriar) {
        if (IsActiveThreadTerminating()) return;
        PlayCutscene("CS_HEROSOULS_SOUL3_OUTRO_SCYTHE");
    } else {
        if (IsActiveThreadTerminating()) return;
        PlayCutscene("CS_HEROSOULS_SOUL3_OUTRO_BRIAR");
    }
    if (!HeardJackMessage) {
        if (IsActiveThreadTerminating()) return;
        Game->Pause(1.0f);
        long Conversation = Game->AddNewConversation(Game->GetHero(), false, false);
        Game->AddLineToConversation(Conversation, "TEXT_QST_B04_JACK_MSG1_10", false, Game->GetHero(), Game->GetHero());
    }
    HeardJackMessage = true;
    MasterScriptData->DeliveredSoul++;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HERO_SOULS_OBJECTIVE_16", "NorthernWastes3", "NorthernWastes3");
    Game->SetCreatureGeneratorsEnabled("NorthernWastes3", true);
    if (MasterScriptData->KilledGM) {
        if (IsActiveThreadTerminating()) return;
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_375");
    } else {
        if (IsActiveThreadTerminating()) return;
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_370");
    }
    Game->SetThingPersistent(Game->GetThingWithScriptName("DragonGate"), true);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    while (!Game->MsgOnQuestCompleted("Q_DragonBossFight")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}