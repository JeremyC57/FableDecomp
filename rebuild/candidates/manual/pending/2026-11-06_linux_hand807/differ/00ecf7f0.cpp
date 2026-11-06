// NScript::CV_SickChildScript::CWomanToAttract::Main() -- the laughing woman: comments to the hero on a timer by how often he made her laugh, laughs at his funny expressions (the third laugh plays her leaving cutscene and gives morality), sniffs at rude ones, answers him when talked to, and is removed once she has gone.
// Grade: DIFFER(4835v4885) -- retail expands the by-value CScriptThing copy for SetIsPushableByHero (ours calls 0x004ABE90; any inline/implicit copy also expands the predicate copy retail calls); ~CTimer expanded only at the natural loop exit in retail, called (0x00CD4470) at ours; register choices in the conversation block (204 lines)
// Next idea: split the two copies: a by-value CScriptThing temporary built by a named local (CScriptThing t(Me); Game->SetIsPushableByHero(t, ...)) with IMPLICIT_COPY, keeping the predicate copy in the CIsThingConscious mem-initializer out of line; else lane gap: per-site inline choice (gotchas 2099/2131)
// Leaf: ?Main@CWomanToAttract@CV_SickChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ECF7F0, length 4885 bytes.
// Name: slot 1 of the entity vtable 0x012F2404 (0 scalar deleting dtor 0x00ECD5D0, 2 Init 0x00ECD380, 3 GetParentScript
//   0x00ECD370, all landed as CWomanToAttract); Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CWomanToAttract@CV_SickChildScript@NScript@@EAEXXZ.
// Data: pGlobalScriptDef MadeWomanLaugh +0x774; default CV_SickChildScript.h body (no header change).
// Shape: a same-TU `inline __declspec(noinline)` CBasicString<char>::Compare body (the landed 0x00411570 loop) lets VC7.1 see
//   the call has no side effects, so it keeps the expression text pointer in esi across the inline == chains as retail does
//   (907 -> 204 lines). FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ + FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE give the inline ==.
// Tried (10): see STATUS.tsv.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "engine/C3DVector.h"
#include "engine/CV_SickChildScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
// The body of CBasicString<char>::Compare 0x00411570 (the landed 0x00411570 loop): same-TU and not inlined, as in retail, so VC7.1
// knows the call leaves the string alone and keeps `expression`'s data pointer in esi across the compare chains.
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
void NScript::CV_SickChildScript::CWomanToAttract::Main()
{
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
    if (ParentClass.LaughingWomanKilled==true) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->RemoveThing(Me,false,true);
    }
    Game->SetIsPushableByHero(Me,false);
    if (!GivenObject) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->SetThingHasInformation(Me,false,false,false);
    }
    CCharString expression;
    CTimer timer;
    while (!IsActiveThreadTerminating()) {
        if (timer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            switch (TimesMadeLaugh) {
            case 0:
                Game->AddLineToConversation(conversation,"TEXT_QST_B10_LAUGHING_WOMAN_NOT_LAUGHED",false,Me,Game->GetHero());
                seh_me.PlayAnimation("STANDARD_TALK_SADNESS",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                break;
            case 1:
                Game->AddLineToConversation(conversation,"TEXT_QST_B10_LAUGHING_WOMAN_LAUGHED_A_BIT",false,Me,Game->GetHero());
                seh_me.PlayAnimation("ST_OPINION_RIDICULE_SNIGGER",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                Game->AddLineToConversation(conversation,"TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE",false,Me,Game->GetHero());
                break;
            case 2:
                Game->AddLineToConversation(conversation,"TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL",false,Me,Game->GetHero());
                seh_me.PlayAnimation("ST_OPINION_RIDICULE_POINT_AND_LAUGH",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                Game->AddLineToConversation(conversation,"TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE",false,Me,Game->GetHero());
                break;
            }
            timer=20;
        }
        if (Me.MsgExpressionPerformedTo(expression) && !GivenObject) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (expression=="EXPRESSION_PELVIC_THRUST" || expression=="EXPRESSION_COCK_A_DOODLE_DO" || expression=="EXPRESSION_CROTCH_GRAB" || expression=="EXPRESSION_KISS_MY_ASS" || expression=="EXPRESSION_FLAMENCO" || expression=="EXPRESSION_COSSACK" || expression=="EXPRESSION_AIR_GUITAR" || expression=="EXPRESSION_BALLET" || expression=="EXPRESSION_SATURDAY_NIGHT_FEVER" || expression=="EXPRESSION_TAP" || expression=="EXPRESSION_GIGGLE") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ++TimesMadeLaugh;
                CCharString text("TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY");
                if (expression=="EXPRESSION_PELVIC_THRUST") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_VULGAR_ONCE";
                }
                else if (expression=="EXPRESSION_COCK_A_DOODLE_DO") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_COCK_ONCE";
                }
                else if (expression=="EXPRESSION_CROTCH_GRAB") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_CROTCH_ONCE";
                }
                else if (expression=="EXPRESSION_KISS_MY_ASS") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_KISSARSE_ONCE";
                }
                else if (expression=="EXPRESSION_FLAMENCO" || expression=="EXPRESSION_COSSACK" || expression=="EXPRESSION_AIR_GUITAR" || expression=="EXPRESSION_BALLET" || expression=="EXPRESSION_SATURDAY_NIGHT_FEVER" || expression=="EXPRESSION_TAP") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE";
                }
                else if (expression=="EXPRESSION_GIGGLE") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_GIGGLE_ONCE";
                }
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                switch (TimesMadeLaugh) {
                case 1:
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),text,GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                case 2:
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                case 3:
                    DoLeavingCutscene();
                    GivenObject=true;
                    Game->GiveHeroMorality(pGlobalScriptDef->MadeWomanLaugh);
                    break;
                default:
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    break;
                }
                timer=20;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CCharString text("TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY");
                if (expression=="EXPRESSION_SHIT") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_RUDE";
                }
                else if (expression=="EXPRESSION_FLIRT") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_FLIRT";
                }
                else if (expression=="EXPRESSION_HEROIC_STANCE") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_HEROPOSE";
                }
                else if (expression=="EXPRESSION_VICTORY_PUMP") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    text="TEXT_QST_B10_LAUGHING_WOMAN_ARMPUMP";
                }
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),text,GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !GivenObject) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            switch (TimesMadeLaugh) {
            case 0:
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_LAUGHING_WOMAN_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                break;
            case 1:
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_ONCE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                break;
            case 2:
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_TWICE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                break;
            }
            timer=timer.GetTime()+5;
        }
        if (GivenObject) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->RemoveThing(Me,false,true);
            return _Fail_Val.Return();
        }
        Game->NewScriptFrame();
    }
}
