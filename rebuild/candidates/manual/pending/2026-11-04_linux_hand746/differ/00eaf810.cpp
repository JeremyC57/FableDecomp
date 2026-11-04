// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoAskingHeroToKillAssassin() -- the sheriff's attract call, intro, yes/no task question (quest card on yes) and "attacked" lines
// Grade: DIFFER(3021v2945) -- one extra store `mov [esp+20h],ebx` (pause.Game homed: VC7.1 splits it around the declined arm's Speak, where retail keeps it in ebx and spills the GetHero `Game` to a frame temp) shifts every later temporary 4 bytes and stops the exits cross-jumping (dl=99)
// Leaf: ?DoAskingHeroToKillAssassin@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EAF810, 2945 bytes, thiscall, plain `ret` (private, non-virtual: AAE).
// Identity: Ego_r.pdb / FableWin.pdb ?DoAskingHeroToKillAssassin@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@AAEXXZ;
//   first of the donor Do* members (= retail 0x00EAF810, before 0x00EB0830); called from CMurderTwistSheriff::Main at 0x00EAF039;
//   the texts TEXT_QST_B07_SHERIFF_ATTRACT / _HERO_INTRO / _QUESTION / _HERO_ACCEPTED / _HERO_DECLINED / _ATTACKED.
// Behaviour: no task declined, attract timer expired, hero within 15 and able to see the sheriff -> timer 10 and an "attract"
//   conversation line. Talked to -> movie + paused entities; no task declined yet: hero intro + hero title, "after title", intro
//   (SUSS variant when the assassin trusts the hero); else the "fickle" line. Then the yes/no question; yes -> accepted line,
//   SheriffState 1, the MURDER_TWIST quest card and its KILL_ASSASSIN objective, TaskDeclined false; no -> declined line,
//   TaskDeclined true. Hit (special ability 14 excepted) -> prefix (TaskDeclined ? ATTACKED : ATTACKED_BEFORE_INTRO), wait for the
//   enflame reaction, paused prefix + NextLine (1..2), NextLine advanced. Terminating threads return at once.
// Callees: interface slots 7, 39 MsgIsQuestionAnsweredYesOrNo, 70 GetHero, 89/90 SetTimer/GetTimer (CTimer inline through
//   pDSTGame 0x0143E8F8), 114 GiveHeroYesNoQuestion, 264 GetHealth, 295 GiveHeroQuestCardDirectly, 296 SetQuestCardObjective,
//   364/365/366 AddNewConversation / AddPersonToConversation / AddLineToConversation, 370, 379, 632 CanThingBe_Seen_ByOtherThing,
//   655 GetActiveQuestName; IsDistanceBetweenThingsUnder 0x00CBE2FF, GetHeroTitleAsLHTSTag 0x00CBEBA7,
//   CV_MurderTwistFinalScript::DoesAssassinTrustHero 0x00EB03A0 / WaitForEnflameReactionIfNecessary 0x00EB0700.
// Tried (13 grades + ~30 compile probes, NOTES.md): plain returns (exit block lands at the fickle copy; TRetVoid fixes it),
//   swapped TaskDeclined arms, loop forms for the answer wait, do-while(0) checks (all / initial / loop), const / char / ==false
//   bools, function-scope bool / answer, switch on the answer, plain-inline pause ctor. Removing either accepted/declined ternary or
//   the hit block removes the spill (diagnostic only).
// Next idea: the PDB function-local records for this function (owner's machine) -- a scope or declaration difference of the
//   accepted/declined texts or the answer local is the likeliest lever.
// Layout: engine/CV_MurderTwistFinalScript.h (CMurderTwistSheriff: AttractTimer +0x2C, NextLine +0x30; parent TaskDeclined +0x50).
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
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
CCharString __fastcall GetHeroTitleAsLHTSTag();   // 0x00CBEBA7
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }

void NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::DoAskingHeroToKillAssassin()
{
    TRetVoid _Fail_Val;
    if (!ParentClass->TaskDeclined && AttractTimer.GetTime() == 0
        && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)
        && Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me)) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        AttractTimer = 10;
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_B07_SHERIFF_ATTRACT", false, Me, Game->GetHero());
    }
    if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        bool assassin_distrusts_hero = !ParentClass->DoesAssassinTrustHero();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (!ParentClass->TaskDeclined) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_SHERIFF_HERO_INTRO" + GetHeroTitleAsLHTSTag(), GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_SHERIFF_HERO_INTRO_AFTERTITLE", GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            CCharString text(assassin_distrusts_hero ? "TEXT_QST_B07_SHERIFF_INTRO" : "TEXT_QST_B07_SHERIFF_INTRO_SUSS");
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), text, GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_SHERIFF_HERO_FICKLE", GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        Game->GiveHeroYesNoQuestion("TEXT_QST_B07_SHERIFF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
        long answer = Game->MsgIsQuestionAnsweredYesOrNo();
        while (answer < 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            answer = Game->MsgIsQuestionAnsweredYesOrNo();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (answer == 1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CCharString text(assassin_distrusts_hero ? "TEXT_QST_B07_SHERIFF_HERO_ACCEPTED" : "TEXT_QST_B07_SHERIFF_HERO_ACCEPTED_SUSS");
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), text, GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ParentClass->SheriffState = SHERIFF_STATE_1;
            Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_MURDER_TWIST", Game->GetActiveQuestName(), true);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MURDER_TWIST_OBJECTIVE_KILL_ASSASSIN", "BanditCampBoss", "");
            ParentClass->TaskDeclined = false;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CCharString text(assassin_distrusts_hero ? "TEXT_QST_B07_SHERIFF_HERO_DECLINED" : "TEXT_QST_B07_SHERIFF_HERO_DECLINED_SUSS");
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), text, GROUP_SELECT_NONE, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ParentClass->TaskDeclined = true;
        }
    }
    if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CCharString prefix(ParentClass->TaskDeclined ? "TEXT_QST_B07_SHERIFF_ATTACKED" : "TEXT_QST_B07_SHERIFF_ATTACKED_BEFORE_INTRO");
        ParentClass->WaitForEnflameReactionIfNecessary(Me);
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (NextLine > 2) NextLine = 1;
        CCharString text = prefix + GFIntToCharString(NextLine);
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
