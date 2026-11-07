// NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main() -- demon-door face (three suits): take scripted control of Me, make it persistent and immune, find the nearest REGION_EXIT_POINT and shut it; each time the hero talks to it walk a four-step state machine under a paused speech movie (intro, then knight, mage and bandit outfit checks: SEEN_x when worn and on to the next, WANT_x and stop when not); once all three are seen speak the opening line, fade the face out, open DD_ThreeSuits and reactivate the exit
// Grade: DIFFER(2236v2252) -- 22 normalised lines (unchanged by chunk 856's 88 grades): retail opens with a dead store `mov [esp+0x3c],ebp` (a frame slot speech_movie's PImp later uses) that VC7.1 does not emit from this source, and retail pads the StartScriptingEntity loop head and the main while head to 16 bytes (16 bytes of nop, hence 2236 vs 2252); calls, strings, switch table, frame and register colouring otherwise identical
// Next idea: loop-head alignment follows block weight: chunk 856's plain returns, block-scoped filter, forceinline CWideScreenMagicPauseEntities ctor, g_fZero, main-loop for/while forms and every-statement sweeps leave both heads unaligned (b7, do{}while(0) round the acquisition loop, still aligns it but moves the seh_me dtor); compare with the landed pad=1 siblings CDF_SummonsCreatures 0x00E74D70 / CDF_DoDamage 0x00E70BC0 statement by statement
// Leaf: ?Main@CDF_ThreeSuits@CV_DemonDoorsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E72630, 2252 bytes incl. the 4-entry switch table at 0x00E72EEC, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main) of vtable 0x012E9124 stored by the landed Alloc 0x00E72560 (GetParentScript 0x00E72600 landed);
//   Ego_r.pdb ?Main@CDF_ThreeSuits@CV_DemonDoorsScript@NScript@@EAEXXZ.
// Callees: CBaseObject 0x0099A380 / ~ 0x0099A430, CIsThingAlive (vtable 0x012C32F0, dtor 0x00E241B0) + CScriptThing copy 0x004ABE90,
//   SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, ScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor
//   0x007E74D0 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450, MovieBase dtor 0x006E7B80 and the
//   PImp dtor CCountedPointer<MovieBase>::~ 0x006E7AB0, ~CScriptThing 0x004AA840, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   CDF_ThreeSuits::DressedAsGoodKnight 0x00E730A0 / DressedAsEvilMage 0x00E731F0 / DressedAsBandit 0x00E72F00,
//   CV_DemonDoorsScript::FadeOutFaceAndOpenDoor 0x00E6F040; CGameScriptInterfaceBase slots 7, 8, 70 GetHero, 81 GetNearestWithDefName,
//   264 GetHealth, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 477 SetThingPersistent, 516 / 517 / 526, 567
//   SetRegionExitAsActive; CScriptThing slot 27 MsgIsTalkedToBy.
// Layout: engine/CV_DemonDoorsScript.h CDF_ThreeSuits (Ego_r.pdb 28 bytes = retail new size): ParentClass +0x14, PMasterData +0x18;
//   the conversation state is a register local (ebp), not a member.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDF_THREESUITS
#include "engine/CV_DemonDoorsScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();

void NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->SetThingPersistent(Me,true);
    Game->EntitySetAsDamageable(Me,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game->EntitySetAsKillable(Me,false,false);
    CScriptThing region_exit=Game->GetNearestWithDefName(Me,"REGION_EXIT_POINT");
    Game->SetRegionExitAsActive(region_exit,false);
    long state=0;
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            switch (state) {
            case 0:
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                state=1;
            case 1:
                if (DressedAsGoodKnight()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_KNIGHT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    state=2;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_KNIGHT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                }
            case 2:
                if (DressedAsEvilMage()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_MAGE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    state=3;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_MAGE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                }
            case 3:
                if (DressedAsBandit()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_SEEN_BANDIT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_OPENING",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.FadeOutFaceAndOpenDoor(Me,"DD_ThreeSuits");
                    Game->SetRegionExitAsActive(region_exit,true);
                    return;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_088_THREE_SUITS_WANT_BANDIT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                break;
            }
        }
        Game->NewScriptFrame();
    }
}
