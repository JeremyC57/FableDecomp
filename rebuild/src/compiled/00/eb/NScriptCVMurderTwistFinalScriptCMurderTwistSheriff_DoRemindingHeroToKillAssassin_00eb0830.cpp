// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoRemindingHeroToKillAssassin() -- the sheriff while the hero owes him the assassin: "hero returned" (assassin still alive) or "hero lost" lines, "attacked" lines when hit
// Leaf: ?DoRemindingHeroToKillAssassin@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB0830, 2185 bytes, thiscall, plain `ret` (private, non-virtual: AAE).
// Identity: Ego_r.pdb / FableWin.pdb ?DoRemindingHeroToKillAssassin@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ;
//   donor member order DoAskingHeroToKillAssassin / DoRemindingHeroToKillAssassin / DoBecomingSuspiciousOfHero = retail 0x00EAF810 /
//   0x00EB0830 / 0x00EB10C0; called from CMurderTwistSheriff::Main at 0x00EAF087 (landed binding of this symbol to 0x00EB0830).
// Behaviour: AssassinState 1: talked to -> paused "TEXT_QST_B07_SHERIFF_HERO_RETURNED"; hit (special ability 14 excepted) ->
//   wait for the enflame reaction, paused "TEXT_QST_B07_SHERIFF_ATTACKED_HERO_LOST" + NextLine (1..2), NextLine advanced.
//   AssassinState 0: talked to -> "TEXT_QST_B07_SHERIFF_HERO_LOST" (first time, then HasGivenReminderToHero) or that line numbered
//   by NextLine; hit -> as above. Any other state: nothing. A terminating thread returns at once (RAII unpause, movie release).
// Callees: as 0x00EB2900 plus CV_MurderTwistFinalScript::WaitForEnflameReactionIfNecessary 0x00EB0700 and the MovieBase ctor
//   0x006E7B60 (VC7.1 expands the first talk-block ctor and calls the COMDAT at the others; every dtor is the COMDAT 0x006E7B80).
// Shape: the two state arms are if / else-if; their identical hit blocks are tail-merged by VC7.1 (one copy at 0x00EB0FBD..).
// Style: Lionhead's generated-script returns (`TRetVoid _Fail_Val; ... return _Fail_Val.Return();`, FableWin TPI TRetVoid;
//   PDB function-local records name _Fail_Val / speech_movie / PauseMyEntitiesPlease in the sibling entity Main 0x00F170C0,
//   codex pass 13 NAMING_EVIDENCE; not checked for this function, whose locals are not dumped).
// Layout: engine/CV_MurderTwistFinalScript.h (CMurderTwistSheriff: ParentClass +0x14, MyEntityHandle +0x1C, NextLine +0x30,
//   HasGivenReminderToHero +0x34; parent AssassinState +0x4C).
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

void NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoRemindingHeroToKillAssassin()
{
    TRetVoid _Fail_Val;
    if (ParentClass->AssassinState == ASSASSIN_STATE_1) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_SHERIFF_HERO_RETURNED", GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass->WaitForEnflameReactionIfNecessary(Me);
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (NextLine > 2) NextLine = 1;
            CCharString text = "TEXT_QST_B07_SHERIFF_ATTACKED_HERO_LOST" + GFIntToCharString(NextLine);
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
    } else if (ParentClass->AssassinState == ASSASSIN_STATE_0) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (HasGivenReminderToHero) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (NextLine > 2) NextLine = 1;
                CCharString text = "TEXT_QST_B07_SHERIFF_HERO_LOST" + GFIntToCharString(NextLine);
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
                    MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_SHERIFF_HERO_LOST", GROUP_SELECT_NONE, false, true, false);
                    while (MyEntityHandle.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                HasGivenReminderToHero = true;
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass->WaitForEnflameReactionIfNecessary(Me);
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (NextLine > 2) NextLine = 1;
            CCharString text = "TEXT_QST_B07_SHERIFF_ATTACKED_HERO_LOST" + GFIntToCharString(NextLine);
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
}