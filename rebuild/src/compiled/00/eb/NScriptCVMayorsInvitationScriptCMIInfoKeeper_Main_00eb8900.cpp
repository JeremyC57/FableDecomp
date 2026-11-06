// NScript::CV_MayorsInvitationScript::CMI_InfoKeeper::Main() -- the Mayor's Invitation gossip villager: unless the secret is already learned (or he has spoken, then he just idles), give the gossip intro whenever talked to until the quest is WhichQuest 3, then read his chain number, offer information and, when talked to, tell the next of the four gossip lines by SecretsLearned (the fourth sets HeroLearnedSecret and objective 04), count it and idle
// Inline-split: one terminate check wrapped individually (the StartScriptingEntity wait in the first loop); retail's construct is unproven
// Leaf: ?Main@CMI_InfoKeeper@CV_MayorsInvitationScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB8900, 2964 bytes, thiscall, plain `ret`, no EH frame; ends in its `ret` at 0x00EB9493.
// Identity (proven): Ego_r.pdb / FableWin.pdb name set ?Main@CMI_InfoKeeper@CV_MayorsInvitationScript@NScript@@EAEXXZ; the address
//   appears once in .rdata, at slot 1 of the entity vtable stored by the landed CMI_InfoKeeper::Alloc 0x00EB8810 (its Init 0x00EB88C0,
//   GetParentScript 0x00EB88B0, OnPredicateFail 0x00EB88D0, OnPersist 0x00EB9950 are landed); ChainNumber +0x1c / HaveSpoken +0x20 are
//   the PDB members, and the literals are the TEXT_QST_044_GOSSIP* lines and TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_04.
// Callees: CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CScriptThing copy 0x004ABE90 (in the inline
//   CIsThingAlive ctor) / ~CIsThingAlive 0x00E241B0 / ~CScriptThing 0x004AA840, handle IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / dtor 0x007E74D0 (out of line, the opt-in),
//   CBaseObject 0x0099A380 / ~ 0x0099A430, movie ~CCountedPointer 0x006E7AB0 / ctor 0x006E7B60 / dtor 0x006E7B80, CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0, GFCharStringToInt 0x0099E7F0 (__fastcall); CGameScriptInterfaceBase slots 7 NewScriptFrame, 8 StartScriptingEntity,
//   70 GetHero, 264 GetHealth, 296 SetQuestCardObjective, 360 SetThingHasInformation, 361 ClearThingHasInformation, 370 StartMovieSequence,
//   379 PauseAllNonScriptedEntities, 655 GetActiveQuestName; CScriptThing slots 3 GetDataString, 27 MsgIsTalkedToBy.
// Layout: engine/CV_MayorsInvitationScript.h (new, chunk 789): HeroLearnedSecret +0x4c, SecretsLearned +0x54, WhichQuest +0x58.
// Shape: generated entity-script code (the landed CMurderTwistBrotherOlderFriend::Main 0x00EACF00 model). SecretsLearned is compared as a
//   float (`fild` into a frame slot, then fucompp against 0.0 / 1.0 / 2.0 / 3.0). The HeroLearnedSecret test is an early return (nesting the
//   rest under `if (!HeroLearnedSecret)` puts an extra exit block in); the first movie's implicit dtor is expanded at its normal exit only
//   when the terminate check in the preceding StartScriptingEntity wait is wrapped in `do { } while (0)` (the coordinator's per-site lever;
//   wrapping all seven waits, the speak loop or the other checks of that loop gives 5-58 differing lines).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_MayorsInvitationScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
long FABLE_FASTCALL GFCharStringToInt(const CCharString&);   // 0x0099E7F0

void NScript::CV_MayorsInvitationScript::CMI_InfoKeeper::Main()
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
    if (ParentClass.HeroLearnedSecret) return;
    {
        if (HaveSpoken) {
            if (seh_me.IsValid()) seh_me.Release();
            Game->ClearThingHasInformation(Me);
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
        }
        while (ParentClass.WhichQuest != 3) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    do { if (IsActiveThreadTerminating()) return; } while (0);
                }
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_044_GOSSIP_INTRO", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
        }
        if (IsActiveThreadTerminating()) return;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        ChainNumber = GFCharStringToInt(Me.GetDataString());
        Game->SetThingHasInformation(Me, false, true, false);
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        float secrets = (float)ParentClass.SecretsLearned;
        if (secrets == 0.0f) {
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
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_044_GOSSIP1", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
            ++ParentClass.SecretsLearned;
            Game->ClearThingHasInformation(Me);
        } else if (secrets == 1.0f) {
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
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_044_GOSSIP2", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
            ++ParentClass.SecretsLearned;
            Game->ClearThingHasInformation(Me);
        } else if (secrets == 2.0f) {
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
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_044_GOSSIP3", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
            ++ParentClass.SecretsLearned;
            Game->ClearThingHasInformation(Me);
        } else if (secrets == 3.0f) {
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
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_044_GOSSIP4", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
            ParentClass.HeroLearnedSecret = true;
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_04", "OakvaleMemorialGarden", "");
            ++ParentClass.SecretsLearned;
            Game->ClearThingHasInformation(Me);
        }
        Game->ClearThingHasInformation(Me);
        HaveSpoken = true;
        if (seh_me.IsValid()) seh_me.Release();
        for (;;) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
    }
}