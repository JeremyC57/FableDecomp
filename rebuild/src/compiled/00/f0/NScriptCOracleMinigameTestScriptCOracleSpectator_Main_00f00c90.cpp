// NScript::COracleMinigameTestScript::COracleSpectator::Main
// Leaf: ?Main@COracleSpectator@COracleMinigameTestScript@NScript@@EAEXXZ
// Flags: /d2inlT51
// Inline-budget: /d2inlT51 keeps the return cleanup split at 0x00F00E7C and 0x00F010C7. The constructor's scoped depth retains the CScriptThing copy at 0x00F00CA9.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/EScriptAIPriority.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/COracleMinigameTestScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"

CCharString FABLE_FASTCALL GFIntToCharString(long value);

namespace NScript {
#pragma inline_depth(0)
inline CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#pragma inline_depth()
}

inline CBaseClass::CBaseClass() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : PImp(other.PImp) {}

void NScript::COracleMinigameTestScript::COracleSpectator::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();

    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game.SetIsPushableByHero(Me, false);
    Game.SetCreatureBrain(Me, "BRAIN_PASSIVE_OVERRIDE");
    Game.SetWanderCentrePoint(Me, Me.GetPos());
    Game.SetWanderMinDistance(Me, 0.0f);
    Game.SetWanderMaxDistance(Me, 10.0f);
    Game.SetScriptingStateGroup(Me, ESSG_WANDER_NEAR);
    for (;; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_B03_SPECTATOR_" + Me.GetDataString() + "_" + GFIntToCharString(NextLine),
                             GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (NextLine == 40) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                NextLine = 10;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                NextLine += 10;
            }
            if (seh_me.IsValid())
                seh_me.Release();
        }
    }
}