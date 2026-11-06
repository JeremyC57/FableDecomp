// NScript::CV_ChapelOfEvilScript::Main() -- the Chapel of Evil village script main: bind the acolyte / altar / victim entity scripts, post them, wait for a Darkwood region, spawn the region-entry / termination / weapon / quest-info / mission threads, activate the quest once (guild master message, quest card, first objective), spawn the inside-chapel and expression threads, then each time DarkwoodChapel loads wait until the hero is inside and have the altar voice the intro (first visit: three lines, second objective, initial morality) or the welcome-back / not-welcome line
// Leaf: ?Main@CV_ChapelOfEvilScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E693F0, 2832 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00E69EFF.
// Identity (proven from the vtable): 0x00E693F0 is slot 2 (Main) of the script vtable 0x012E8080, whose slot 1 is the landed
//   CV_ChapelOfEvilScript::RegisterMain 0x00E69030 and slot 3 the landed Init 0x00E68FA0; the "V_ChapelOfEvil" Alloc 0x00E6E230
//   (ctor 0x00E68F70) stores it. Ego_r.pdb name set ?Main@CV_ChapelOfEvilScript@NScript@@UAEXXZ. The three bound Allocs are the landed
//   CV_ChapelOfEvilScript::CCOE_AcolyteEvil / CCOE_AltarEvil / CCOE_Victim::Alloc rows; the seven threads are this class's QAEXXZ
//   methods named by their thread-name literals (CSpawnedFunc<CV_ChapelOfEvilScript> vtable 0x012E8094, its landed SuspendableProcess 0x00E690C0).
// Callees: operator new 0x00BFEA1A, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / IsActiveThreadTerminating 0x00CB7940 /
//   AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase 0x00CDD450, strstr 0x00BFEB3A (inline CCharString::Contains), Compare 0x00411570
//   (inline CCharString == const char*), CScriptThing::IsAlive 0x004AB130 / ~CScriptThing 0x004AA840; CGameScriptInterfaceBase slots 7
//   NewScriptFrame, 22 MsgOnRegionLoaded, 64 PostAddScriptedEntities, 70 GetHero, 72 GetThingWithScriptName, 157 GetHeroMorality,
//   274 HeroReceiveMessageFromGuildMaster, 276 ActivateQuest, 295 GiveHeroQuestCardDirectly, 296 SetQuestCardObjective,
//   364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation, 655 GetActiveQuestName.
// Layout: engine/CV_ChapelOfEvilScript.h (new, chunk 788).
// Shape: the landed quest-Main binding model (CQ_TraderConflictGoodScript::Main 0x00DFA450); both region loops are the landed
//   WatchForRegionEntry 0x00E6B750 loop (terminate test at the loop top, rotated by VC7.1 into a pre-test and a bottom test).
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
#include "engine/CV_ChapelOfEvilScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

namespace NScript {
#define FABLE_COE_ENTITY(Name) \
    class CV_ChapelOfEvilScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_ChapelOfEvilScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_COE_ENTITY(CCOE_AcolyteEvil)           // Alloc 0x00E6A500
FABLE_COE_ENTITY(CCOE_AltarEvil)             // Alloc 0x00E6A6E0
FABLE_COE_ENTITY(CCOE_Victim)                // Alloc 0x00E6A600
#undef FABLE_COE_ENTITY
}

void NScript::CV_ChapelOfEvilScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ChapelOfEvilScript>("COE_AcolyteEvil", this, &CCOE_AcolyteEvil::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ChapelOfEvilScript>("COE_AltarEvil", this, &CCOE_AltarEvil::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ChapelOfEvilScript>("COE_Victim", this, &CCOE_Victim::Alloc, 0));
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
        if (region.Contains("Darkwood"))
            break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CV_ChapelOfEvilScript>("WatchForRegionEntry", 0, this, &CV_ChapelOfEvilScript::WatchForRegionEntry), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_ChapelOfEvilScript>("WatchForTermination", 0, this, &CV_ChapelOfEvilScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_ChapelOfEvilScript>("WatchForWeaponUnsheathed", 0, this, &CV_ChapelOfEvilScript::WatchForWeaponUnsheathed), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_ChapelOfEvilScript>("WatchForQuestInfoDisplayConditions", 0, this, &CV_ChapelOfEvilScript::WatchForQuestInfoDisplayConditions), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_ChapelOfEvilScript>("DoMission", 0, this, &CV_ChapelOfEvilScript::DoMission), CCharString(""));
    if (!QuestActivated) {
        if (IsActiveThreadTerminating())
            return;
        Game->ActivateQuest("V_ChapelOfEvil_Activate");
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_019_GM_INTRO", "", true, true);
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_CHAPEL_OF_EVIL", Game->GetActiveQuestName(), true);
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_CHAPEL_OF_EVIL_OBJECTIVE_01", "", "");
        QuestActivated = true;
    }
    AddSpawnedFunction(new CSpawnedFunc<CV_ChapelOfEvilScript>("WatchForInsideChapel", 0, this, &CV_ChapelOfEvilScript::WatchForInsideChapel), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_ChapelOfEvilScript>("WatchForExpressions", 0, this, &CV_ChapelOfEvilScript::WatchForExpressions), CCharString(""));
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
        if (region == "DarkwoodChapel") {
            if (IsActiveThreadTerminating())
                return;
            while (!InsideChapel) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            CScriptThing altar = Game->GetThingWithScriptName("M_SoundAltar");
            if (!ChapelIntroComplete) {
                if (IsActiveThreadTerminating())
                    return;
                if (altar.IsAlive()) {
                    if (IsActiveThreadTerminating())
                        return;
                    long conversation = Game->AddNewConversation(altar, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_019_COE_INTRO_10", false, altar, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_019_COE_INTRO_20", false, altar, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_019_COE_INTRO_30", false, altar, Game->GetHero());
                }
                ChapelIntroComplete = true;
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_CHAPEL_OF_EVIL_OBJECTIVE_02", "", "");
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
                        Game->AddLineToConversation(conversation, "TEXT_QST_019_COE_NOT_WELCOME", false, altar, Game->GetHero());
                    } else {
                        if (IsActiveThreadTerminating())
                            return;
                        long conversation = Game->AddNewConversation(altar, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_019_COE_WELCOME_BACK", false, altar, Game->GetHero());
                    }
                }
                WeaponWarningComplete = false;
            }
        }
        Game->NewScriptFrame();
    }
}