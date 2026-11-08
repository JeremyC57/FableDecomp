// NScript::CQ_HeroSoulsGuildmasterScript::Main() -- the Hero Souls (guildmaster) quest main: bind the seven entity scripts, post them, auto-save the quest start unless Briar is dead, set the guild objective, play the guild arrival cut-scene once (with Jack's taunt), set the graveyard objective and activate the Nostro quest, wait for the guild woods (Jack taunts once when the hero is outside the guild or the 200-tick timer runs out), play the guildmaster start cut-scene once, hand over the quest card and start screen, wait out the woods guards (Jack's message when three remain), then play the choice cut-scene and either complete the quest (guildmaster killed) or fail it and send the hero to the graveyard
// Leaf: ?Main@CQ_HeroSoulsGuildmasterScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D84D60, 3259 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00D85A1A.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012D4804 (the ctor 0x00D84B50 called by the "Q_HeroSoulsGuildmaster" Alloc
//   0x00D87A80); Ego_r.pdb name set ?Main@CQ_HeroSoulsGuildmasterScript@NScript@@UAEXXZ. Every bound Alloc is a landed
//   CQ_HeroSoulsGuildmasterScript::C*::Alloc row (binding-name literal = class name less its `C`).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CWideString(const unsigned
//   short*) 0x0099B6B0 / ~ 0x0099B510, ~CScriptThing 0x004AA840, ~CTimer 0x00CD4470 (inline copies at the loop exits),
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / IsActiveThreadTerminating 0x00CB7940,
//   CQ_HeroSoulsGuildmasterScript::PlayCutscene 0x00D872D0, AddLogbookStoryEntryString 0x00CBE960 (__fastcall),
//   std::map<CCharString,bool>::operator[] 0x008ADF10; pDSTGame 0x0143E8F8 slots 87 RegisterTimer, 88 DeregisterTimer, 89 SetTimer,
//   90 GetTimer; pGlobalScriptDef 0x0143E90C (HS_GoodSoulTaken +0xC9C); CGameScriptInterfaceBase slots 7, 12, 64, 70 GetHero, 72
//   GetThingWithScriptName, 108 RemoveThing, 156 GiveHeroMorality, 267 SetThingAsKilled, 274 HeroReceiveMessageFromGuildMaster,
//   276 ActivateQuest, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 289 SetQuestAsFailed, 294 RemoveQuestCardFromHero,
//   295 GiveHeroQuestCardDirectly, 296 SetQuestCardObjective, 309 KickOffQuestStartScreen, 364 AddNewConversation,
//   366 AddLineToConversation, 368 IsConversationActive, 374 FadeScreenIn, 376 Pause, 472 EntityTeleportToThing,
//   655 GetActiveQuestName, 704 AutoSaveQuestStart.
// Shape: the landed quest-Main binding model (CQ_TraderConflictGoodScript::Main 0x00DFA450) with WAIT_UNTIL loops; the local CTimer's
//   destructor is a plain inline body (VC7.1 expands it at the hot loop exits and calls 0x00CD4470 elsewhere, as retail).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HeroSoulsGuildmasterScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTimer.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
extern CGameScriptInterfaceBase* pDSTGame;               // 0x0143E8F8
void __fastcall AddLogbookStoryEntryString(const CCharString& text);   // 0x00CBE960
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

namespace NScript {
#define FABLE_HSG_ENTITY(Name) \
    class CQ_HeroSoulsGuildmasterScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HeroSoulsGuildmasterScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_HSG_ENTITY(CGuildmaster_HS)            // Alloc 0x00D85A30
FABLE_HSG_ENTITY(CGuardOutsideWoods)         // Alloc 0x00D85FA0
FABLE_HSG_ENTITY(CMuteGuardOutsideWoods)     // Alloc 0x00D86080
FABLE_HSG_ENTITY(CGuardInWoods)              // Alloc 0x00D86390
FABLE_HSG_ENTITY(CWaitingGuardInWoods)       // Alloc 0x00D86560
FABLE_HSG_ENTITY(CHSG_DistanceChecker)       // Alloc 0x00D86980
FABLE_HSG_ENTITY(CSoulInfoAcolyte)           // Alloc 0x00D86B60
#undef FABLE_HSG_ENTITY
}

void NScript::CQ_HeroSoulsGuildmasterScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsGuildmasterScript>("Guildmaster_HS", this, &CGuildmaster_HS::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsGuildmasterScript>("GuardOutsideWoods", this, &CGuardOutsideWoods::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsGuildmasterScript>("MuteGuardOutsideWoods", this, &CMuteGuardOutsideWoods::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsGuildmasterScript>("GuardInWoods", this, &CGuardInWoods::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsGuildmasterScript>("WaitingGuardInWoods", this, &CWaitingGuardInWoods::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsGuildmasterScript>("HSG_DistanceChecker", this, &CHSG_DistanceChecker::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HeroSoulsGuildmasterScript>("SoulInfoAcolyte", this, &CSoulInfoAcolyte::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (!MasterScriptData->KilledBriar) {
        if (IsActiveThreadTerminating())
            return;
        Game->AutoSaveQuestStart();
    }
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_11", "HeroGuildComplexInside", "HeroGuildComplexInside");
    if (!PlayedGuildCS) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("HeroGuildComplexInside")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_HEROSOULS_SOUL3_GUILD_ARRIVAL");
        PlayedGuildCS = true;
        Game->Pause(0.5f);
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_B04_JACK_GUILD_TAUNT_10", "", true, true);
    }
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_12", "GraveyardCircle", "Graveyard");
    Game->ActivateQuest("Q_HeroSoulsNostro");
    bool tauntGiven = false;
    CTimer tauntTimer;
    tauntTimer = 200;
    while (!Game->IsRegionLoaded("GuildWoods")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!tauntGiven && (!Game->IsRegionLoaded("HeroGuildComplexInside") || tauntTimer.GetTime() == 0)) {
            if (IsActiveThreadTerminating())
                return;
            Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_B04_JACK_GUILD_TAUNT_20", "", true, true);
            tauntGiven = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (!PlayedGuildMasterCS) {
        if (IsActiveThreadTerminating())
            return;
        PlayCutscene("CS_HEROSOULS_SOUL3_GM_START");
        PlayedGuildMasterCS = true;
    }
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_SOULS_GUILDMASTER", Game->GetActiveQuestName(), true);
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_14", "GuildWoods", "HeroGuildComplexInside");
    Game->FadeScreenIn();
    bool messageGiven = false;
    while (GuardInWoodsCount > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!messageGiven && GuardInWoodsCount == 3) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Game->GetHero(), false, false);
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_JACK_MSG_GUILDMASTER_TAUNT", false, Game->GetHero(), Game->GetHero());
            while (Game->IsConversationActive(conversation)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            messageGiven = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing guildmaster = Game->GetThingWithScriptName("Guildmaster_HS");
    PlayCutscene("CS_HEROSOULS_SOUL3_GM_CHOICE");
    if (cs_flags["GMKILLED"]) {
        if (IsActiveThreadTerminating())
            return;
        MasterScriptData->KilledGM = true;
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_375");
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, true);
        Game->SetThingAsKilled(guildmaster);
        Game->GiveHeroMorality(pGlobalScriptDef->HS_GoodSoulTaken);
        Game->RemoveThing(guildmaster, false, true);
        Game->FadeScreenIn();
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), false, L"", true);
        Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("GraveYardHSP"), false);
        Game->RemoveQuestCardFromHero(Game->GetActiveQuestName());
        Game->SetQuestCardObjective("Q_HeroSouls", "TEXT_QUEST_HERO_SOULS_OBJECTIVE_12A", "GraveyardCircle", "Graveyard");
        while (!Game->IsRegionLoaded("Graveyard")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
        Game->FadeScreenIn();
    }
}