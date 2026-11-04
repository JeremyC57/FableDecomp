// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::DoRemindingHeroToKillAssassin() -- a sheriff's guard once the hero accepted the sheriff's task: talked to -> paused cycling line, hit -> paused cycling "attacked" line
// Leaf: ?DoRemindingHeroToKillAssassin@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB2900, 1203 bytes, thiscall, plain `ret` (private, non-virtual: AAE).
// Identity: Ego_r.pdb / FableWin.pdb ?DoRemindingHeroToKillAssassin@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@AAEXXZ; donor member order CMurderTwistSheriffGuard
//   (DoAskingHeroToSpeakToBoss, DoRemindingHeroToKillAssassin, DoAskingHeroToSpeakToBossAgain, DoCongratulatingHero,
//   DoSayingThankYouToHero) = retail order 0x00EB23E0 / 0x00EB2900 / 0x00EB2DC0 / 0x00EB3280 / 0x00EB3740; the guard's Main calls this
//   body at 0x00EB22B4 in that order; the texts "TEXT_QST_B07_GUARD_TASK_ACCEPTED" 0x012EFF9C / "TEXT_QST_B07_GUARD_TASK_ACCEPTED_ATTACKED" 0x012EFF70. The three 1203-byte bodies
//   0x00EB2900 / 0x00EB2DC0 / 0x00EB3280 were all labelled DoAskingHeroToSpeakToBossAgain (propagated); they differ only in the texts.
// Behaviour: talked to by the hero -> movie sequence + paused entities, NextLine wrapped to 1 past 2, the line
//   "TEXT_QST_B07_GUARD_TASK_ACCEPTED" + Name + NextLine spoken while the guard is alive, NextLine advanced; hit by the hero (or by a special ability other
//   than HERO_ABILITY_HEAL_LIFE_SPELL) without having been enflamed -> the same with the "_ATTACKED" text. A terminating thread
//   returns at once (RAII unpause and movie release).
// Callees: CScriptThing slots 21/27/41/42 (MsgIsHitBy / MsgIsTalkedToBy / MsgIsHitBySpecialAbilityFrom / ..AnySpecialAbilityFrom),
//   interface slots 7 NewScriptFrame, 70 GetHero, 264 GetHealth, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities,
//   ScriptedThingBase slots 12 GetScriptThing / 13 Speak / 26 IsPerformingScriptTask, CEntityScriptBase::IsActiveThreadTerminating
//   0x00F35B30, CV_MurderTwistFinalScript::HasHeroEnflamedEntity 0x00EB07D0, GFIntToCharString 0x0099F830, operator+ 0x0099F690 /
//   0x0099F570, the MovieBase PImp dtor 0x006E7AB0 / out-of-line dtor 0x006E7B80 (VC7.1's own per-site split, no device).
// Style: Lionhead's generated-script returns (`TRetVoid _Fail_Val; ... return _Fail_Val.Return();`, FableWin TPI TRetVoid;
//   PDB function-local records name _Fail_Val / speech_movie / PauseMyEntitiesPlease in the sibling entity Main 0x00F170C0,
//   codex pass 13 NAMING_EVIDENCE; not checked for this function, whose locals are not dumped).
// Layout: engine/CV_MurderTwistFinalScript.h (CMurderTwistSheriffGuard: ParentClass +0x14, MyEntityHandle +0x1C, NextLine +0x2C, Name +0x34).
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
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };

void NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::DoRemindingHeroToKillAssassin()
{
    TRetVoid _Fail_Val;
    if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (NextLine > 2) NextLine = 1;
        CCharString text = "TEXT_QST_B07_GUARD_TASK_ACCEPTED" + Name + GFIntToCharString(NextLine);
        if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game->GetHero(), text, GROUP_SELECT_NONE, false, true, false);
            while (MyEntityHandle.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        NextLine++;
    }
    if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) && !ParentClass->HasHeroEnflamedEntity(Me)) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (NextLine > 2) NextLine = 1;
        CCharString text = "TEXT_QST_B07_GUARD_TASK_ACCEPTED_ATTACKED" + Name + GFIntToCharString(NextLine);
        if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game->GetHero(), text, GROUP_SELECT_NONE, false, true, false);
            while (MyEntityHandle.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        NextLine++;
    }
}