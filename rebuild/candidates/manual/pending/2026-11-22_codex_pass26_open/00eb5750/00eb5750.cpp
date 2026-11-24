// 00eb5750 NScript::CV_MurderTwistFinalScript::CMurderTwistAssassinGuard::DoAskingHeroToTalkToAssassin
// Leaf: ?DoAskingHeroToTalkToAssassin@CMurderTwistAssassinGuard@CV_MurderTwistFinalScript@NScript@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_MURDER_ASSASSIN_GUARD_PASS26
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
// Retail 00EB5AC6 calls the original counted movie-handle destructor.
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_MurderTwistFinalScript.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_text.h"

namespace NScript {
void CV_MurderTwistFinalScript::CMurderTwistAssassinGuard::DoAskingHeroToTalkToAssassin()
{
    TRetVoid _Fail_Val;
    if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        if (HasBeenSpokenTo) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (NextLine > 2) NextLine = 1;
            CCharString full_text_name = "TEXT_QST_B07_HENCHMAN_BUSY" + Me.GetDataString() + GFIntToCharString(NextLine);
            if (Game.GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game.GetHero(),full_text_name,GROUP_SELECT_NONE,false,true,false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ++NextLine;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game.GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game.GetHero(),"TEXT_QST_B07_HENCHMAN_BUSY" + Me.GetDataString(),GROUP_SELECT_NONE,false,true,false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            HasBeenSpokenTo = true;
        }
    }
    if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
        (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") &&
         !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) &&
        !ParentClass.HasHeroEnflamedEntity(Me)) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        if (NextLine > 2) NextLine = 1;
        CCharString full_text_name = "TEXT_QST_B07_HENCHMAN_ATTACKED" + Me.GetDataString() + GFIntToCharString(NextLine);
        if (Game.GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game.GetHero(),full_text_name,GROUP_SELECT_NONE,false,true,false);
            while (MyEntityHandle.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        ++NextLine;
    }
}
}
