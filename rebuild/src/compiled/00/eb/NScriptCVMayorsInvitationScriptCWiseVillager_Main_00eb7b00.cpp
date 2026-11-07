// NScript::CV_MayorsInvitationScript::CWiseVillager::Main() -- the wise villager of the Mayor's invitation: live while alive, with information flagged and persistent; until the hero has beaten or lost to Thunder, wait to be talked to (the first three hero hits re-take control, a fourth detaches it to the quest script and idles it forever), then give the WhichQuest hint (HINT_5_ALT after the Lady Grey deal) in a paused speech movie; afterwards, when talked to, speak the alternative line and ask the marriage proposal: on YES say ACCEPTED, make it marryable and a hero follower and set ParentClass.MissionFailed; then clear the information flag and reset it as a pure-AI narrator
// Leaf: ?Main@CWiseVillager@CV_MayorsInvitationScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB7B00, 3334 bytes, thiscall, no EH frame, plain `ret` (per-exit epilogue copies), frame 0x80.
// Identity (proven): Ego_r.pdb ?Main@CWiseVillager@CV_MayorsInvitationScript@NScript@@EAEXXZ; slot 1 of the CWiseVillager entity
//   vtable 0x012F0610 (stored by the landed Alloc 0x00EB79F0; slot 0 scalar deleting dtor 0x00EB7AE0, 2 Init 0x00EB7AA0, 3 GetParentScript
//   0x00EB7A90); the label set (ghidra_out/labels_anniversary_2026-11-04_waves23.tsv) agrees.
// Callees: SetPredicate 0x00F35B10 with the CIsThingAlive temporary (vtable 0x012C32F0, ctor inline with the CScriptThing copy 0x004ABE90
//   called, ~CIsThingAlive 0x00E241B0 called), IsActiveThreadTerminating 0x00F35B30, scripted-thing handle (ctor expanded, IsValid
//   0x00CD23B9, Release 0x00CD2770, GetScriptThing 0x007E7490, Speak 0x007E7390, IsPerformingScriptTask 0x007E7450, ~ 0x007E74D0 called
//   except at the two deepest exits), movie ctor 0x006E7B60 / dtor 0x006E7B80 (all called), ~CScriptThing 0x004AA840, CCharString
//   0x0099EBF0 / ~ 0x0099EAE0 / operator const char* 0x0099E4C0, GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&)
//   0x0099F690; CGameScriptInterfaceBase slots 7, 8, 39 MsgIsQuestionAnsweredYesOrNo, 70, 114 GiveHeroYesNoQuestion, 206
//   EntitySetAsMarryable, 235 EntitySetAsAllowedToFollowHero, 264 GetHealth, 360 SetThingHasInformation, 361 ClearThingHasInformation,
//   363 EntityResetAsPureAINarrator, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 465 EntityAttachToScript, 477
//   SetThingPersistent, 655 GetActiveQuestName.
// Data: engine/CV_MayorsInvitationScript.h (opt-in WISEVILLAGER): NumTimesHit +0x1c; ParentClass MissionFailed +0x49,
//   HeroHasDefeatedThunder +0x4e, HeroHasLostToThunder +0x4f, WhichQuest +0x58; PMasterData->StruckDealWithLadyGrey +0xd5.
// Shape: generated entity script (a T-check after every wait loop and at the head of each nested block). The movie ctor is out of line
//   (opt-in MOVIEBASE_OUTOFLINE_CTOR: retail calls 0x006E7B60 at all three sites). The idle-forever loop after EntityAttachToScript is
//   `while (true)`: spelled `for (;;)` VC7.1 calls ~CScriptGameResourceObjectScriptedThingBase at its exit instead of expanding it into
//   the shared pad (DIFFER 3318v3334); with `while (true)` the split matches.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CV_MAYORSINVITATIONSCRIPT_WISEVILLAGER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_MayorsInvitationScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
CCharString FABLE_FASTCALL GFIntToCharString(long value);            // 0x0099F830
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

void NScript::CV_MayorsInvitationScript::CWiseVillager::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->SetThingHasInformation(Me, false, true, false);
    Game->SetThingPersistent(Me, true);
    while (!ParentClass.HeroHasDefeatedThunder && !ParentClass.HeroHasLostToThunder) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) && NumTimesHit < 3) {
                if (IsActiveThreadTerminating()) return;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                ++NumTimesHit;
            }
            else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return;
                Game->ClearThingHasInformation(Me);
                if (seh_me.IsValid()) seh_me.Release();
                Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
                while (true) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
            }
        }
        if (IsActiveThreadTerminating()) return;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (PMasterData->StruckDealWithLadyGrey && ParentClass.WhichQuest == 5) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_044_HELPFUL_VILLAGER_HINT_5_ALT", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
        }
        else {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_044_HELPFUL_VILLAGER_HINT_" + GFIntToCharString(ParentClass.WhichQuest), GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
        }
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    {
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
            seh_me.Speak(Game->GetHero(), "TEXT_QST_044_HELPFUL_VILLAGER_ALTERNATIVE", GROUP_SELECT_NONE, false, true, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        Game->GiveHeroYesNoQuestion("TEXT_QST_044_HELPFUL_VILLAGER_PROPOSAL", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
        long answer;
        while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (answer == 1) {
            if (IsActiveThreadTerminating()) return;
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_044_HELPFUL_VILLAGER_ACCEPTED", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            Game->EntitySetAsMarryable(Me, true);
            Game->EntitySetAsAllowedToFollowHero(Me, true);
            ParentClass.MissionFailed = true;
        }
    }
    Game->ClearThingHasInformation(Me);
    Game->EntityResetAsPureAINarrator(Me);
}