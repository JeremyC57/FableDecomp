// NScript::CV_MurderTwistFinalScript::PlayBeingRumbledCutscene() -- the "being rumbled" cutscene: in a paused movie sequence the first guard, the sheriff and the second guard trade the seven TEXT_QST_B07_RUMBLED lines (the last one to the hero), each spoken only while its speaker is alive and waited out; then release the three scripted-thing handles
// Leaf: ?PlayBeingRumbledCutscene@CV_MurderTwistFinalScript@NScript@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EAF1E0, 1573 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (two sources): Ego_r.pdb ?PlayBeingRumbledCutscene@CV_MurderTwistFinalScript@NScript@@QAEXXZ (engine/CV_MurderTwistFinalScript.h
//   declares it at 0x00EAF1E0), and the landed CMurderTwistSheriff::Main 0x00EAEF30 calls it as ParentClass->PlayBeingRumbledCutscene().
//   The seven TEXT_QST_B07_RUMBLED_10..70 literals (0x012EFACC .. 0x012EFA3C) are the cutscene's lines.
// Callees: the landed DoRemindingHeroToKillAssassin 0x00EB0830 model: MovieBase ctor (inline) / its PImp dtor 0x006E7AB0 + ~CBaseObject
//   0x0099A430 (early exits) / dtor 0x006E7B80 (later exits), CScriptGameResourceObjectScriptedThingBase GetScriptThing (slot 12) / Speak
//   (slot 13) / IsPerformingScriptTask (slot 26) / IsValid 0x00CD23B9 / Release 0x00CD2770, ~CScriptThing (expanded for the first three
//   GetScriptThing temporaries, 0x004AA840 for the rest), CScriptBase::IsActiveThreadTerminating 0x00CB7940; GameCalls slots 7
//   NewScriptFrame, 70 GetHero, 264 GetHealth, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities (the RAII pause guard).
// Layout: engine/CV_MurderTwistFinalScript.h: Game +0x40, Sheriff +0x54, Guards +0x60 (Guards[0] spoken to), seh_Sheriff +0x6C,
//   seh_Guards +0x7C (handles [0] and [1]).
// Shape: plain inline ~CScriptThing (VC7.1 itself expands the three hottest temporaries and calls the copy for the rest, as retail);
//   the two guard handles are released through a named reference each (retail keeps &seh_Guards[0] / [1] in ebx / esi).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CV_MurderTwistFinalScript.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_text.h"
#include "engine/TRetVoid.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline CScriptThing::~CScriptThing() {}
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };

void NScript::CV_MurderTwistFinalScript::PlayBeingRumbledCutscene()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectMovieBase speech_movie;
    Game->StartMovieSequence("", speech_movie);
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
    if (Game->GetHealth(seh_Guards[0].GetScriptThing()) > 0.0f) {
        seh_Guards[0].Speak(Sheriff, "TEXT_QST_B07_RUMBLED_10", GROUP_SELECT_NONE, false, true, false);
        while (seh_Guards[0].IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (Game->GetHealth(seh_Sheriff.GetScriptThing()) > 0.0f) {
        seh_Sheriff.Speak(Guards[0], "TEXT_QST_B07_RUMBLED_20", GROUP_SELECT_NONE, false, true, false);
        while (seh_Sheriff.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (Game->GetHealth(seh_Sheriff.GetScriptThing()) > 0.0f) {
        seh_Sheriff.Speak(Guards[0], "TEXT_QST_B07_RUMBLED_30", GROUP_SELECT_NONE, false, true, false);
        while (seh_Sheriff.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (Game->GetHealth(seh_Guards[0].GetScriptThing()) > 0.0f) {
        seh_Guards[0].Speak(Sheriff, "TEXT_QST_B07_RUMBLED_40", GROUP_SELECT_NONE, false, true, false);
        while (seh_Guards[0].IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (Game->GetHealth(seh_Guards[1].GetScriptThing()) > 0.0f) {
        seh_Guards[1].Speak(Guards[0], "TEXT_QST_B07_RUMBLED_50", GROUP_SELECT_NONE, false, true, false);
        while (seh_Guards[1].IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (Game->GetHealth(seh_Sheriff.GetScriptThing()) > 0.0f) {
        seh_Sheriff.Speak(Guards[0], "TEXT_QST_B07_RUMBLED_60", GROUP_SELECT_NONE, false, true, false);
        while (seh_Sheriff.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (Game->GetHealth(seh_Guards[1].GetScriptThing()) > 0.0f) {
        seh_Guards[1].Speak(Game->GetHero(), "TEXT_QST_B07_RUMBLED_70", GROUP_SELECT_NONE, false, true, false);
        while (seh_Guards[1].IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    {
        CScriptGameResourceObjectScriptedThingBase& guard = seh_Guards[0];
        if (guard.IsValid()) guard.Release();
    }
    {
        CScriptGameResourceObjectScriptedThingBase& guard = seh_Guards[1];
        if (guard.IsValid()) guard.Release();
    }
    if (seh_Sheriff.IsValid()) seh_Sheriff.Release();
}