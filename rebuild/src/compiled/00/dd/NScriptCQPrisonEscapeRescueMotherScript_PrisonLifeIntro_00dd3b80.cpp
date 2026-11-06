// NScript::CQ_PrisonEscapeRescueMotherScript::PrisonLifeIntro() -- the prison-life intro cut-scene: wait for PrisonCells_1, start the quest, confiscate the hero's gear, the guard (peeing) and the accomplice talk to the hero, then the accomplice's welcome speech; flag PrisonCutsceneFinished
// Leaf: ?PrisonLifeIntro@CQ_PrisonEscapeRescueMotherScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD3B80, 2126 bytes, thiscall, plain `ret`.
// Identity: the only reference is the CScriptThread spawn at 0x00DD34EF (`mov [edi+34h],0DD3B80h`) inside
//   CQ_PrisonEscapeRescueMotherScript::Main 0x00DD2CE0; the TU order Main, WatchHeroEscapeStatus (0x00DD35D0, landed), this,
//   matches the Ego_r order WatchHeroEscapeStatus -> PrisonLifeIntro; the literals are "TEXT_QST_055_PRISON_INTRO_01" ..
//   "_15" (0x012DBE48 .. 0x012DBC38). Ego_r ?PrisonLifeIntro@CQ_PrisonEscapeRescueMotherScript@NScript@@QAEXXZ.
//   The catalog label `CQ_GuildTrainingScript::RunCeremonyCutscene` is wrong (that one is landed at 0x00D49D50).
// Callees: IsActiveThreadTerminating 0x00CB7940, CCharString 0x0099EBF0 / 0x0099EAE0, CBaseObject ctor 0x0099A380, scripted
//   handle IsValid 0x00CD23B9 / Release 0x00CD2770 / PlayAnimation 0x007E73D0 / MoveToPosition 0x007E72F0 / dtor 0x007E74D0,
//   ~CScriptThing 0x004AA840, GetHeroTitleAsLHTSTag 0x00CBEBA7 (__fastcall), operator+(const char*, const CCharString&)
//   0x0099F690 (__fastcall); CGameScriptInterfaceBase slots 7, 8, 11, 70, 72, 131, 137, 138, 192, 309, 364-366, 368, 374,
//   376, 475, 703; CScriptThing slot 6 GetPos.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_PrisonEscapeRescueMotherScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/fable_script_animation_defaults.h"

CCharString __fastcall GetHeroTitleAsLHTSTag();   // 0x00CBEBA7

void NScript::CQ_PrisonEscapeRescueMotherScript::PrisonLifeIntro()
{
    if (MotherRescuedFromPrisonAutoSaveCheckPoint) {
        if (IsActiveThreadTerminating()) return;
        return;
    }
    while (!Game->IsLevelLoaded("PrisonCells_1")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->KickOffQuestStartScreen("Q_PrisonEscapeRescueMother", false, true);
    Game->AutoSaveCheckPoint();
    Game->ConfiscateAllHeroItems();
    Game->ConfiscateAllHeroWeapons();
    Game->SetHeroWillAsUsable(false);
    Game->RemoveHeroHairstyle();
    Game->FadeScreenIn();
    CScriptGameResourceObjectScriptedThingBase guard_res;
    CScriptGameResourceObjectScriptedThingBase accomplice_res;
    CScriptThing guard = Game->GetThingWithScriptName("PrisonGuard");
    CScriptThing accomplice = Game->GetThingWithScriptName("PrisonerAccomplice");
    Game->EntitySetFacingAngleTowardsThing(accomplice, Game->GetHero(), false);
    if (guard_res.IsValid()) guard_res.Release();
    while (!Game->StartScriptingEntity(guard, guard_res, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (accomplice_res.IsValid()) accomplice_res.Release();
    while (!Game->StartScriptingEntity(accomplice, accomplice_res, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    guard_res.PlayAnimation("ST_PISSING", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
    long conversation = Game->AddNewConversation(guard, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    Game->AddPersonToConversation(conversation, accomplice);
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_01", false, guard, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_02", false, guard, accomplice);
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_03", false, accomplice, guard);
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_04", false, guard, accomplice);
    if (YearsPassedInCell == 0) {
        if (IsActiveThreadTerminating()) return;
        Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_04_B", false, guard, accomplice);
    }
    while (Game->IsConversationActive(conversation)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    guard_res.MoveToPosition(Game->GetThingWithScriptName("GuardPatrolExit").GetPos(), 2.0f, (EScriptEntityMoveType)0, false, true);
    Game->Pause(5.0f);
    PrisonerConversationIndex = conversation = Game->AddNewConversation(accomplice, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_05", false, accomplice, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_06", false, accomplice, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_07", false, accomplice, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_08" + GetHeroTitleAsLHTSTag(), false, accomplice, Game->GetHero());
    if (MasterScriptData->WhisperKilledByHero) {
        if (IsActiveThreadTerminating()) return;
        Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_09_A", false, accomplice, Game->GetHero());
    } else {
        if (IsActiveThreadTerminating()) return;
        Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_09_B", false, accomplice, Game->GetHero());
    }
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_10", false, accomplice, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_11", false, accomplice, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_12", false, accomplice, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_13", false, accomplice, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_14", false, accomplice, Game->GetHero());
    Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_INTRO_15", false, accomplice, Game->GetHero());
    while (Game->IsConversationActive(conversation)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    PrisonCutsceneFinished = true;
}