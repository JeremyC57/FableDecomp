// NScript::CPersonalScript_EmptyScript::CCinemaMan::Main() -- the cinema test man: guard the thread with "Me and the three cinema markers and both look-at targets are alive", take scripted control, then while the thread runs answer an aggressive / non-aggressive hero special ability with a screen message, or, when the hero talks to him, play the father-intro speech lines under a paused movie
// Grade: DIFFER(3409v3265) -- md 47 with the hdr opt-in FABLE_RETAIL_CPREDICATE_AND_IMPLICIT_DTOR: frame and registers = retail, but every CPredicate_And temporary's dtor is expanded to the leaves; retail expands one level per temporary and calls the nested complete dtor (0x00EF6DE0 / 0x00EF6D90 / 0x00EF6D50 / 0x00EF5B50); the default header (user-declared virtual dtor, probe a) gives md 1174
// Next idea: declared-only explicit specialisations of the four nested ~CPredicate_And (GOTCHAS chunk 790/791 lever) with the implicit dtor of the outermost type only
// Leaf: ?Main@CCinemaMan@CPersonalScript_EmptyScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATE_AND_IMPLICIT_DTOR
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CPersonalScript_EmptyScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CPredicate_And.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline CScriptThing::~CScriptThing() { }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#define CM_T if (IsActiveThreadTerminating()) return
#define CM_SPEAK(key) \
    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
        seh_me.Speak(Game->GetHero(), key, GROUP_SELECT_NONE, false, true, false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            CM_T; \
        } \
        CM_T; \
    }

void NScript::CPersonalScript_EmptyScript::CCinemaMan::Main()
{
    CScriptThing cinema_marker1 = Game->GetThingWithScriptName("CinemaMarker1");
    CScriptThing cinema_marker2 = Game->GetThingWithScriptName("CinemaMarker2");
    CScriptThing cinema_marker3 = Game->GetThingWithScriptName("CinemaMarker3");
    CScriptThing look_at_me1 = Game->GetThingWithScriptName("LookAtMe1");
    CScriptThing look_at_me2 = Game->GetThingWithScriptName("LookAtMe2");
    SetPredicate(CIsThingAlive(Me) && CIsThingAlive(cinema_marker1) && CIsThingAlive(cinema_marker2) && CIsThingAlive(cinema_marker3)
        && CIsThingAlive(look_at_me1) && CIsThingAlive(look_at_me2));
    Game->NewScriptFrame();
    CM_T;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        CM_T;
    }
    CM_T;
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
            CM_T;
            Game->AddScreenMessage("How very mean!", GROUP_SELECT_NONE);
        } else if (Me.MsgIsHitByAnyNonAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
            CM_T;
            Game->AddScreenMessage("How very nice!", GROUP_SELECT_NONE);
        } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            CM_T;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_10");
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_20");
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_30");
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_60");
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_70");
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_80");
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_90");
            CM_SPEAK("TEXT_QST_048_FATHER_INTRO_100");
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                CM_T;
            }
            CM_T;
        }
        Game->NewScriptFrame();
    }
}
