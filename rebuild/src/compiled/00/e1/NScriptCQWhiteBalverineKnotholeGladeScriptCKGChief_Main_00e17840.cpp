// NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main() -- the Knothole Glade chief: live while alive; not combo / stat counted, not pushable, unkillable, no information marker; then every frame until the thread ends: when the hero talks to him say the line for the current BalverineState (2 / 3 / 4 / 6, or 7 while Q_WhiteBalverineWW is not active) under a paused speech movie; when the hero hits him (not with the heal-life spell) while BalverineState != 7, say BEEN_ATTACKED under a paused speech movie and become friends with everything
// Inline-budget: (1) scoped `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor (with FABLE_RETAIL_CPREDICATEBASE_NOVTABLE, FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY and a row-local inline ~CIsThingAlive: the predicate idiom of 0x00E25F70) -- retail expands the predicate ctor but CALLS its nested CScriptThing copy 0x004ABE90 (0x00E17863), while it expands the by-value SetIsPushableByHero copy (before the slot-844 call 0x00E178F4); without the pragma c2 scores the nested copy 28 (f=1) and the by-value copy 32 (f=0.5) and expands both, so retail calls the hotter site and expands the colder one and no threshold splits them (S13 + T30 / T50 / T65 give 2362 / 2485 / 2531); (2) flag /d2inlS13 -- retail expands both CWideScreenMagicPauseEntities ctors (PauseAllNonScriptedEntities(true) at 0x00E17A2C / 0x00E1803B); with the default cap c2 expands the talk one (score 45) and calls the hit one (f=0.0203, score 133); S13 makes the 25-unit ctor always inline (S13..S15 match, S12 differs). The flag replaces the drafts' FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR opt-in (also a match; ladder: flag first).
// Ablation (each removed in turn): no predicate pragma 2531v2515 (ctor + dtor in the pragma or inline_depth(1) also match), no NOVTABLE DIFFER(2515v2515), no IMPLICIT_COPY 2483, no flag 2525, no row-local inline ~CIsThingAlive 2499; the drafts' MOVIEBASE OUTOFLINE_CTOR / OUTOFLINE_DTOR, the movie counted-pointer declaration and a pragma round the hit speech_movie are unneeded with the family loop form (dropped).
// Leaf: ?Main@CKG_Chief@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ
// Flags: /d2inlS13
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E17840, 2515 bytes, thiscall, plain `ret`, no EH frame.
// Identity: slot 1 (Main) of the CKG_Chief entity vtable 0x012E0B98 (0 sdd 0x00E17820, 2 Init 0x00E17800, 3 GetParentScript 0x00E177F0,
//   5 OnPredicateFail 0x00E17810; Alloc 0x00E17750 bound "KG_Chief" by the landed script Main 0x00E13F10); Ego_r.pdb / FableWin.pdb
//   ?Main@CKG_Chief@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ. Layout: engine/CQ_WhiteBalverineKnotholeGladeScript.h (chunk
//   833 opt-in FABLE_RETAIL_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_CKG_CHIEF, carried in this batch's overlay; parent BalverineState +0x50).
// Source shape: Ego_d.pdb line table (lines 681..742) and locals (seh_me, _Fail_Val; speech_movie and PauseMyEntitiesPlease in each of
//   the two speech scenes), in the family's generated text (TRetVoid returns; the frame loop `for (;!false;Game->NewScriptFrame())` with
//   its terminate test first, so the pre-loop test 0x00E17921 and the bottom test 0x00E18146 share the exit 0x00E1814E, where retail
//   expands ~seh_me: an in-loop return, which c2 scores hot; the drafts' `while (!IsActiveThreadTerminating())` made it the cold natural
//   exit and called the dtor there, then expanded it at the talk wait-loop exit instead).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_CKG_CHIEF
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#include "fable_string.h"
#include "engine/EHeroAbility.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_WhiteBalverineKnotholeGladeScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline NScript::CIsThingAlive::~CIsThingAlive() { }

void NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetAsToAddToStatChangesWhenHit(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAsKillable(Me, false, true);
    Game->SetThingHasInformation(Me, false, false, false);
    for (;!false;Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (ParentClass.BalverineState == 2) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else if (ParentClass.BalverineState == 3) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_AFTER_FIRST_ATTACK", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else if (ParentClass.BalverineState == 4) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_IN_SECOND_ATTACK", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else if (ParentClass.BalverineState == 6) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_IN_THIRD_ATTACK", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else if (ParentClass.BalverineState == 7 && !Game->IsQuestActive("Q_WhiteBalverineWW")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_AFTER_THIRD_ATTACK", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            if (seh_me.IsValid())
                seh_me.Release();
        }
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
             (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) &&
            ParentClass.BalverineState != 7) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BEEN_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Me.SetFriendsWithEverythingFlag(true);
            if (seh_me.IsValid())
                seh_me.Release();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}