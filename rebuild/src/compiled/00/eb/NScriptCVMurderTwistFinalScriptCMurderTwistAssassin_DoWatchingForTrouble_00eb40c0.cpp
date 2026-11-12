// NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::DoWatchingForTrouble() -- the assassin's idle state: talked to by the
// Grade: RELOCATION_MATCH
//   hero -> paused movie sequence, first the intro and "intro finished" lines (AssassinState 1, HasGivenIntroToHero), later a
//   cycling "start" line (NextLine 1..2); else, feeling threatened for the first time -> paused "threatened" line, the hero's
//   weapons sheathed and waited for (HasBeenThreatened); else hit by the hero (or a special ability other than 14), or threatened
//   again -> wait for the enflame reaction and deal with the hero
// Leaf: ?DoWatchingForTrouble@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB40C0, 1601 bytes, thiscall, plain `ret` (private, non-virtual: AAE).
// Identity: Ego_r.pdb / FableWin.pdb ?DoWatchingForTrouble@CMurderTwistAssassin@CV_MurderTwistFinalScript@NScript@@AAEXXZ;
//   engine/CV_MurderTwistFinalScript.h declares it at 0x00EB40C0.
// Callees: CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430, CCountedPointer<MovieBase> dtor 0x006E7AB0, MovieBase dtor COMDAT
//   0x006E7B80, ~CScriptThing 0x004AA840, CCharString 0x0099EBF0 / ~ 0x0099EAE0 / operator const char* 0x0099E4C0,
//   GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690, CEntityScriptBase::IsActiveThreadTerminating
//   0x00F35B30, IsFeelingThreatened 0x00EB4710, DoDealWithHero 0x00EB4760, WaitForEnflameReactionIfNecessary 0x00EB0700;
//   CScriptThing slots 21 MsgIsHitBy, 27 MsgIsTalkedToBy, 41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom;
//   handle slots 12 GetScriptThing, 13 Speak, 26 IsPerformingScriptTask; CGameScriptInterfaceBase slots 7 NewScriptFrame,
//   70 GetHero, 130 SheatheHeroWeapons, 195 IsPlayerWieldingWeapon, 264 GetHealth, 370 StartMovieSequence,
//   379 PauseAllNonScriptedEntities.
// Layout: engine/CV_MurderTwistFinalScript.h opt-in FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN.
// Shape: the sibling DoDealWithHero 0x00EB4760 script model (TRetVoid _Fail_Val, MovieBase, the RAII pause guard,
//   `return _Fail_Val.Return();` at every terminate exit).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CV_MURDERTWISTFINALSCRIPT_ASSASSIN
#include "engine/CV_MurderTwistFinalScript.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_text.h"
#include "engine/EHeroAbility.h"
#include "engine/TRetVoid.h"
#include "engine/ETextGroupSelectionMethod.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline CScriptThing::~CScriptThing() { }

void NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::DoWatchingForTrouble()
{
    TRetVoid _Fail_Val;
    if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (HasGivenIntroToHero) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (NextLine > 2)
                NextLine = 1;
            CCharString text = "TEXT_QST_B07_ASSASSIN_START" + GFIntToCharString(NextLine);
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), text, GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            NextLine++;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_ASSASSIN_INTRO", GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_ASSASSIN_INTRO_FINISHED", GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ParentClass->AssassinState = ASSASSIN_STATE_1;
            HasGivenIntroToHero = true;
        }
    }
    if (!HasBeenThreatened && IsFeelingThreatened()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_ASSASSIN_THREATENED", GROUP_SELECT_NONE, false, true, false);
            while (MyEntityHandle.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        Game->SheatheHeroWeapons();
        while (Game->IsPlayerWieldingWeapon()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        HasBeenThreatened = true;
    }
    else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
        || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))
        || (HasBeenThreatened && IsFeelingThreatened())) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass->WaitForEnflameReactionIfNecessary(Me);
        DoDealWithHero();
    }
}