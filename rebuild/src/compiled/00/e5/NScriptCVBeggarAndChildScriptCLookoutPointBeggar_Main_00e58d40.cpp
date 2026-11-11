// NScript::CV_BeggarAndChildScript::CLookoutPointBeggar::Main() -- the lookout-point beggar: until the beggar or the bully is hit or leaves, answers the hero (first chat with the battle-cry lesson offer, repeat chats), replies to the bully's taunts, and reacts to the hero's belch / fart expressions.
// Inline-budget: #pragma inline_depth(0) round the row-local inline CIsThingAlive(const CScriptThing&) definition + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE -- retail calls the CScriptThing copy 0x004ABE90 inside the expanded predicate ctor (0x00E58D62) yet expands the by-value Me copies; with plain inline VC7.1 expands both or neither (GOTCHAS chunks 810/813/835, 20+ spellings); /d2inlT45..T75 do not reach it on the sibling 0x00E5B3B0 (T moves the by-value sites too)
// Leaf: ?Main@CLookoutPointBeggar@CV_BeggarAndChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E58D40, 9616 bytes, thiscall, plain `ret`, no EH frame, incl. its two jump tables (the rand() % 5
//   animation table 0x00E5B2AC, 5 entries, and the belch-count table 0x00E5B2C0, 4 entries). The function ends at 0x00E5B2D0, where the
//   landed CBeggarBully::Alloc starts: the catalog row's 9808 bytes run on over Alloc, GetParentScript 0x00E5B370 and Init 0x00E5B380 (SPLITS.tsv).
// Identity (proven): slot 1 (Main) of the CLookoutPointBeggar entity vtable 0x012E68C4 that Alloc 0x00E58C60 stores (slot 0 the landed
//   scalar deleting dtor 0x00E58D20, slot 2 Init 0x00E58D10); Ego_r.pdb / FableWin.pdb ?Main@CLookoutPointBeggar@CV_BeggarAndChildScript@NScript@@EAEXXZ.
#include "fable_compiler.h"
#ifdef _MSC_VER
#define FABLE_ROW_NOINLINE __declspec(noinline)
#else
#define FABLE_ROW_NOINLINE
#endif
#define FABLE_RETAIL_CV_BEGGARANDCHILDSCRIPT_ENTITY_MAINS
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CV_BeggarAndChildScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern "C" int rand();                             // 0x00BFEB16 (MSVCR71)
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
// Inline control (owner decision 2026-11-08, device 3): the predicate's special members are compiled with no inlining of their own,
// so the CScriptThing member copy / destructor stay calls (0x004ABE90 / 0x004AA840) when Main expands them, as retail does at every
// predicate site; FABLE_RETAIL_CPREDICATEBASE_NOVTABLE keeps the empty base constructor from becoming a call under the region.
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth()
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

template<> inline FABLE_ROW_NOINLINE long FABLE_FASTCALL CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

void NScript::CV_BeggarAndChildScript::CLookoutPointBeggar::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->SetIsPushableByHero(Me, false);
    CScriptThing bully = Game->GetThingWithScriptName("BeggarBully");
    bool first_chat_had = false;
    long battle_cry_count = 0;
    Game->SetThingHasInformation(Me, false, false, false);
    Game->EntitySetAsKillable(Me, false, true);
    bool talked_to = false;
    while (!ParentClass->BeggarHit && !ParentClass->BullyHit && !ParentClass->BeggarLeft && !ParentClass->BullyLeft) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            talked_to = true;
        }
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass->BeggarHit = true;
        } else if (talked_to) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            talked_to = false;
            if (!first_chat_had && !ParentClass->TaughtBattleCry) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                first_chat_had = true;
                if (ParentClass->BelchedAtBeggar != 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase first_belch_movie;
                    Game->StartMovieSequence("", first_belch_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_FIRST_SPEAK_BELCH", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    Game->GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10);
                        if (!ParentClass->ExpressionTutorialShown) {
                            if (Game->IsXbox()) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game->DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS");
                                while (!Game->MsgIsGameInfoClickedPast()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            } else {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game->DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC");
                                while (!Game->MsgIsGameInfoClickedPast()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            ParentClass->ExpressionTutorialShown = true;
                        }
                        ParentClass->TaughtBattleCry = true;
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_TEACH_REFUSED", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase first_movie;
                    Game->StartMovieSequence("", first_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_FIRST_SPEAK", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    Game->GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10);
                        if (!ParentClass->ExpressionTutorialShown) {
                            if (Game->IsXbox()) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game->DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS");
                                while (!Game->MsgIsGameInfoClickedPast()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            } else {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game->DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC");
                                while (!Game->MsgIsGameInfoClickedPast()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            ParentClass->ExpressionTutorialShown = true;
                        }
                        ParentClass->TaughtBattleCry = true;
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_TEACH_REFUSED", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    }
                }
                if (!ParentClass->QuestCardGiven) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BEGGAR_AND_CHILD", Game->GetActiveQuestName(), false);
                    ParentClass->QuestCardGiven = true;
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass->BelchedAtBeggar != 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase belch_movie;
                    Game->StartMovieSequence("", belch_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (ParentClass->TaughtBattleCry) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_REPEAT_BATTLE_CRY", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_REPEAT_BELCH_RETURN", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        Game->GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                        long answer;
                        while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (answer == 1) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10);
                            if (!ParentClass->ExpressionTutorialShown) {
                                if (Game->IsXbox()) {
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                    Game->DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS");
                                    while (!Game->MsgIsGameInfoClickedPast()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                } else {
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                    Game->DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC");
                                    while (!Game->MsgIsGameInfoClickedPast()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                ParentClass->ExpressionTutorialShown = true;
                            }
                            ParentClass->TaughtBattleCry = true;
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT_FAIL_BELCH", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase repeat_movie;
                    Game->StartMovieSequence("", repeat_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (ParentClass->TaughtBattleCry) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_REPEAT_BATTLE_CRY", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                        long answer;
                        while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (answer == 1) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10);
                            if (!ParentClass->ExpressionTutorialShown) {
                                if (Game->IsXbox()) {
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                    Game->DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS");
                                    while (!Game->MsgIsGameInfoClickedPast()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                } else {
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                    Game->DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC");
                                    while (!Game->MsgIsGameInfoClickedPast()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                ParentClass->ExpressionTutorialShown = true;
                            }
                            ParentClass->TaughtBattleCry = true;
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT_FAIL", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        }
                    }
                }
            }
            ParentClass->TauntTimer = ParentClass->TauntTimer.GetTime() + 5;
        } else if (ParentClass->BullyHasTaunted == true && ParentClass->BeggarReplyTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("BeggarBully"), false);
            ParentClass->BullyHasTaunted = false;
            CTimer reply_wait;
            reply_wait = 2;
            while (!talked_to && reply_wait.GetTime() > 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    talked_to = true;
                }
                if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass->BeggarHit = true;
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!talked_to) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                switch (rand() % 5) {
                case 0: seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_DISMISSING_HAND_SWIPE", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                case 1: seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_CALLING_OVER_FOR_FIGHT", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                case 2: seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_POINTING_AWAY_GET_OUT", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                case 3: seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_SHAKE_FIST", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                case 4: seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_POINT_AT", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                }
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, bully);
                if (rand() % 4 == 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_01", false, Me, bully);
                } else if (rand() % 4 == 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_02", false, Me, bully);
                } else if (rand() % 4 == 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_03", false, Me, bully);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_04", false, Me, bully);
                }
            }
        }
        CCharString expression;
        if (Me.MsgExpressionPerformedTo(expression)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            CTimer face_wait;
            face_wait = 2;
            while (!talked_to && face_wait.GetTime() > 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    talked_to = true;
                }
                if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass->BeggarHit = true;
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!talked_to) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (expression == "EXPRESSION_BELCH") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ++ParentClass->BelchedAtBeggar;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    switch (ParentClass->BelchedAtBeggar) {
                    case 1: Game->AddLineToConversation(conversation, "TEXT_QST_015_BEGGAR_FIRST_BELCH_REPLY", false, Me, Game->GetHero()); break;
                    case 2: Game->AddLineToConversation(conversation, "TEXT_QST_015_BEGGAR_SECOND_BELCH_REPLY", false, Me, Game->GetHero()); break;
                    case 3: Game->AddLineToConversation(conversation, "TEXT_QST_015_BEGGAR_THIRD_BELCH_REPLY", false, Me, Game->GetHero()); break;
                    case 4:
                        ParentClass->BeggarLeft = true;
                        ParentClass->BelchedAtBully = true;
                        break;
                    }
                } else if (expression == "EXPRESSION_FART") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ++battle_cry_count;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    switch (battle_cry_count) {
                    case 1: Game->AddLineToConversation(conversation, "TEXT_QST_015_BEGGAR_FIRST_BATTLE_CRY_REPLY", false, Me, Game->GetHero()); break;
                    case 2: Game->AddLineToConversation(conversation, "TEXT_QST_015_BEGGAR_SECOND_BATTLE_CRY_REPLY", false, Me, Game->GetHero()); break;
                    default: Game->AddLineToConversation(conversation, "TEXT_QST_015_BEGGAR_THIRD_BATTLE_CRY_REPLY", false, Me, Game->GetHero()); break;
                    }
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
}