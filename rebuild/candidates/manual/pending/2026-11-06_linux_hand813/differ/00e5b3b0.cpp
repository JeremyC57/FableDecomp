// NScript::CV_BeggarAndChildScript::CBeggarBully::Main() -- the Bowerstone bully: taunts the beggar on a timer (a disapproval animation and the TAUNT line), offers to teach the hero the belch (yes/no, expression tutorial, quest card), answers later chats, reacts to belches (leaves after the fourth) and farts, and clears the expression quick-access items when he or the beggar is gone
// Grade: DIFFER(6996v7012) -- 138 lines, all from the CIsThingAlive copy blocker (GOTCHAS chunk 810): retail expands the SetIsPushableByHero by-value Me copy (0x00E5B484..A4) while calling the copy 0x004ABE90 inside the expanded CIsThingAlive ctor; ours calls both, so the vtable registers recolour to 0x00E5B578 and one alignment pad moves
// Next idea: the shared lane fix for the chunk 810 blocker (the predicate's nested CScriptThing copy called, top-level by-value copies expanded); every other byte matches
// Leaf: ?Main@CBeggarBully@CV_BeggarAndChildScript@NScript@@EAEXXZ
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CBeggarBully@CV_BeggarAndChildScript@NScript@@EAEXXZ (private virtual); entity vtable 0x012E6C1C
//   slot 1 = 0x00E5B3B0 (its only .rdata reference 0x012E6C20), stored by the landed Alloc 0x00E5B2D0 (reference 0x00E5B306); slot 0 the
//   landed scalar deleting dtor 0x00E5B390, slot 2 Init 0x00E5B380, slot 3 GetParentScript 0x00E5B370.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5B3B0, 7012 bytes incl. the rand()%5 animation table 0x00E5CEF0 (cases 0..4) and the belch_count
//   table 0x00E5CF04 (cases 1..4); no EH frame.
// Layout: new engine/CV_BeggarAndChildScript.h (FABLE_RETAIL_NSCRIPT; Ego_r.pdb offsets - 0x14).
// Levers that worked (a1 220 -> a13 138): the declared-only CCountedPointer<MovieBase> dtor (chunk 804) makes VC7.1 call the implicit
//   ~MovieBase at the first-chat scope and expand it at the friend scope as retail; the same-TU noinline CBasicString<char>::Compare
//   body (landed 0x00F063B0) keeps expression's data pointer in esi across the BELCH / FART compares and fixes the frame slots.
//   Tried without effect: bool declaration order, bools before beggar (worse), inverted talk branches, IMPLICIT_COPY (644),
//   a reference-bound predicate temporary, MOVIEBASE OUTOFLINE_DTOR / FORCEINLINE_CTOR / FORCEINLINE (see STATUS.tsv).
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
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
extern "C" int __cdecl rand();                             // 0x00BFEB16 (MSVCR71)
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

// The body of CBasicString<char>::Compare 0x00411570 (the landed 0x00411570 loop; chunk 807 / 809 lesson): same-TU and not inlined,
// as in retail, so VC7.1 knows the call leaves the string alone and keeps `expression`'s data pointer in esi across the compare chain.
template<> inline __declspec(noinline) long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
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

void NScript::CV_BeggarAndChildScript::CBeggarBully::Main()
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
    CScriptThing beggar = Game->GetThingWithScriptName("LookoutPointBeggar");
    bool friend_talk_said = false;
    bool first_chat_had = false;
    long belch_count = 0;
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
            ParentClass->BullyHit = true;
        } else if (talked_to) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            talked_to = false;
            if (!first_chat_had && !ParentClass->TaughtBelch) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                first_chat_had = true;
                CScriptGameResourceObjectMovieBase first_chat_movie;
                Game->StartMovieSequence("", first_chat_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BULLY_FIRST_CHAT", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                Game->GiveHeroYesNoQuestion("TEXT_QST_015_BULLY_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                long answer;
                while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (answer == 1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BULLY_TEACH_BELCH", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, 10);
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
                    ParentClass->TaughtBelch = true;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BULLY_TEACH_BELCH_REFUSED", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                }
                if (!ParentClass->QuestCardGiven) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BEGGAR_AND_CHILD", Game->GetActiveQuestName(), false);
                    ParentClass->QuestCardGiven = true;
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase friend_movie;
                Game->StartMovieSequence("", friend_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (ParentClass->TaughtBelch) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (ParentClass->BelchedAtBeggar != 0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (!friend_talk_said) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            friend_talk_said = true;
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BULLY_FRIEND_TALK", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BULLY_FRIEND_SNEER_OFFER", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BULLY_FRIEND_BELCH_COMMAND", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BULLY_FRIEND_TALK_20", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    Game->GiveHeroYesNoQuestion("TEXT_QST_015_BULLY_REPEAT_BELCH_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer == 1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BULLY_TEACH_BELCH", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, 10);
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
                        ParentClass->TaughtBelch = true;
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_015_BULLY_REPEAT_BELCH_QUESTION_REFUSAL", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    }
                }
            }
            ParentClass->TauntTimer = ParentClass->TauntTimer.GetTime() + 5;
        } else if (ParentClass->TauntTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("LookoutPointBeggar"), false);
            ParentClass->TauntTimer = 10;
            CTimer taunt_wait;
            taunt_wait = 2;
            while (!talked_to && taunt_wait.GetTime() > 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    talked_to = true;
                }
                if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass->BullyHit = true;
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!talked_to) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                switch (rand() % 5) {
                case 0: seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_DISMISSING_HAND_SWIPE", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                case 1: seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_NYAH_NYAH", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                case 2: seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_POINTING_AWAY_GET_OUT", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                case 3: seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_SHAKE_FIST", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                case 4: seh_me.PlayAnimation("ST_OPINION_DISAPPROVAL_POINT_AT", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                }
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, beggar);
                Game->AddLineToConversation(conversation, "TEXT_QST_015_BULLY_TAUNT", false, Me, beggar);
                Game->Pause(0.2f);
                ParentClass->BullyHasTaunted = true;
                ParentClass->TauntTimer = 12;
                ParentClass->BeggarReplyTimer = 5;
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
                    ParentClass->BullyHit = true;
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!talked_to) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (expression == "EXPRESSION_BELCH") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ++belch_count;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    switch (belch_count) {
                    case 1: Game->AddLineToConversation(conversation, "TEXT_QST_015_BULLY_FIRST_BELCH", false, Me, Game->GetHero()); break;
                    case 2: Game->AddLineToConversation(conversation, "TEXT_QST_015_BULLY_SECOND_BELCH", false, Me, Game->GetHero()); break;
                    case 3: Game->AddLineToConversation(conversation, "TEXT_QST_015_BULLY_THIRD_BELCH", false, Me, Game->GetHero()); break;
                    case 4:
                        ParentClass->BelchedAtBully = true;
                        ParentClass->BullyLeft = true;
                        break;
                    }
                } else if (expression == "EXPRESSION_FART") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass->BullyLeft = true;
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, -1);
    Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 2, -1);
    Game->SetPreferredQuickAccessItem("OBJECT_EXPRESSION_SNEER_DUMMY", 2, -1);
}
