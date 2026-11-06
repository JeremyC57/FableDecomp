// NScript::CV_TempleOfLightScript::Main() -- the Temple of Light village script main: bind the acolyte / altar entity scripts, post them, wait for a Witchwood region, spawn the acolyte-exit / termination / weapon / quest-info / mission threads, activate the quest once (guild master message), spawn the inside-temple and donation threads, then each time Witchwood3 loads wait until the hero is inside, mark the altar on the minimap and have it voice the intro (first visit: three lines, wait for the conversation, quest card, first objective, initial morality) or the welcome-back / not-welcome line
// Leaf: ?Main@CV_TempleOfLightScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDB190, 2711 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00EDBC26.
// Identity (proven from the vtable): 0x00EDB190 is slot 2 (Main) of the script vtable 0x012F3C68, whose slot 1 is the landed
//   CV_TempleOfLightScript::RegisterMain 0x00EDAE70 and slot 3 the landed Init 0x00EDAE20; the "V_TempleOfLight" Alloc 0x00EDEB40 stores it.
//   Ego_r.pdb name set ?Main@CV_TempleOfLightScript@NScript@@UAEXXZ. The two bound Allocs are the landed CV_TempleOfLightScript::
//   CTOL_AcolyteGood / CTOL_AltarGood::Alloc rows; the seven threads are this class's QAEXXZ methods named by their thread-name literals
//   (CSpawnedFunc<CV_TempleOfLightScript> vtable 0x012F3C7C, its landed SuspendableProcess 0x00EDAF00).
// Callees: operator new 0x00BFEA1A, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / IsActiveThreadTerminating 0x00CB7940 /
//   AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase 0x00CDD450, strstr 0x00BFEB3A (inline CCharString::Contains), Compare 0x00411570
//   (inline CCharString == const char*), CScriptThing::IsAlive 0x004AB130 / ~CScriptThing 0x004AA840; CGameScriptInterfaceBase slots 7
//   NewScriptFrame, 22 MsgOnRegionLoaded, 64 PostAddScriptedEntities, 70 GetHero, 72 GetThingWithScriptName, 157 GetHeroMorality,
//   274 HeroReceiveMessageFromGuildMaster, 276 ActivateQuest, 295 GiveHeroQuestCardDirectly, 296 SetQuestCardObjective,
//   350 MiniMapAddMarker, 364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation, 368 IsConversationActive,
//   655 GetActiveQuestName.
// Layout: engine/CV_TempleOfLightScript.h (new, chunk 788).
// Shape: the sibling CV_ChapelOfEvilScript::Main 0x00E693F0 (chunk 788) with the Temple literals, the minimap marker, the
//   conversation wait, and the quest card / first objective moved into the first-visit block.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING_CONTAINS
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#include <string.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CV_TempleOfLightScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

namespace NScript {
#define FABLE_TOL_ENTITY(Name) \
    class CV_TempleOfLightScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_TempleOfLightScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_TOL_ENTITY(CTOL_AcolyteGood)           // Alloc 0x00EDC1D0
FABLE_TOL_ENTITY(CTOL_AltarGood)             // Alloc 0x00EDC2D0
#undef FABLE_TOL_ENTITY
}

void NScript::CV_TempleOfLightScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TempleOfLightScript>("TOL_AcolyteGood", this, &CTOL_AcolyteGood::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TempleOfLightScript>("TOL_AltarGood", this, &CTOL_AltarGood::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CCharString region;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (region.Contains("Witchwood"))
            break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CV_TempleOfLightScript>("WatchForAcolyteExit", 0, this, &CV_TempleOfLightScript::WatchForAcolyteExit), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_TempleOfLightScript>("WatchForTermination", 0, this, &CV_TempleOfLightScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_TempleOfLightScript>("WatchForWeaponUnsheathed", 0, this, &CV_TempleOfLightScript::WatchForWeaponUnsheathed), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_TempleOfLightScript>("WatchForQuestInfoDisplayConditions", 0, this, &CV_TempleOfLightScript::WatchForQuestInfoDisplayConditions), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_TempleOfLightScript>("DoMission", 0, this, &CV_TempleOfLightScript::DoMission), CCharString(""));
    if (!QuestActivated) {
        if (IsActiveThreadTerminating())
            return;
        Game->ActivateQuest("V_TempleOfLight_Activate");
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_064_GM_INTRO", "", true, true);
        QuestActivated = true;
    }
    AddSpawnedFunction(new CSpawnedFunc<CV_TempleOfLightScript>("WatchForInsideTemple", 0, this, &CV_TempleOfLightScript::WatchForInsideTemple), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_TempleOfLightScript>("WatchForDonationConditions", 0, this, &CV_TempleOfLightScript::WatchForDonationConditions), CCharString(""));
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (region == "Witchwood3") {
            if (IsActiveThreadTerminating())
                return;
            while (!InsideTemple) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            CScriptThing altar = Game->GetThingWithScriptName("M_SoundAltar");
            Game->MiniMapAddMarker(altar, "HUD_ORB_QUEST_VIGNETTE");
            if (!TempleIntroComplete) {
                if (IsActiveThreadTerminating())
                    return;
                if (altar.IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return;
                    long conversation = Game->AddNewConversation(altar, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_064_TOL_INTRO_10", false, altar, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_064_TOL_INTRO_20", false, altar, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_064_TOL_INTRO_30", false, altar, Game->GetHero());
                    while (Game->IsConversationActive(conversation)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                TempleIntroComplete = true;
                Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TEMPLE_OF_LIGHT", Game->GetActiveQuestName(), true);
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_TEMPLE_OF_LIGHT_OBJECTIVE_01", "", "");
                MoralityInitial = Game->GetHeroMorality();
            } else {
                if (IsActiveThreadTerminating())
                    return;
                if (altar.IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (AttackedByHero) {
                        if (IsActiveThreadTerminating())
                            return;
                        long conversation = Game->AddNewConversation(altar, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_064_TOL_NOT_WELCOME", false, altar, Game->GetHero());
                    } else {
                        if (IsActiveThreadTerminating())
                            return;
                        long conversation = Game->AddNewConversation(altar, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_064_TOL_WELCOME_BACK", false, altar, Game->GetHero());
                    }
                }
                WeaponWarningComplete = false;
            }
        }
        Game->NewScriptFrame();
    }
}