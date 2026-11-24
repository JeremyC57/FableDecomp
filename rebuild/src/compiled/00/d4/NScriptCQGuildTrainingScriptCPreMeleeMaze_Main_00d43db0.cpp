// NScript::CQ_GuildTrainingScript::CPreMeleeMaze::Main() -- Handle the guild warning, marker approach and paused movie responses.
// Leaf: ?Main@CPreMeleeMaze@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Flags: default
// Grade: RELOCATION_MATCH
// Inline-budget: Only the hit-movie declaration uses scoped inline_depth(0). PROBES.md records ordinary code and threshold trials.
// Inferred: The outer frame loop tests termination with an in-body break. The four movie timeout checks use one return after scope unwinding.

#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_PREMELEEMAZE
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#define FABLE_RETAIL_SCRIPT_MOVE_ENUM
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "engine/TRetVoid.h"
#include "engine/CQ_GuildTrainingScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/EScriptAIPriority.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_distance.h"

inline CScriptThing::~CScriptThing() {}
inline const C3DVector& CScriptThing::GetPos() const
{
    CScriptThing* implementation = PImp.Data;
    if (!implementation)
        return NullCoord;
    const C3DVector& position = implementation->GetPos();
    return position;
}

void NScript::CQ_GuildTrainingScript::CPreMeleeMaze::Main()
{
    TRetVoid _Fail_Val;
    Game->EntitySetAsKillable(Me, false, true);
    Me.SetFriendsWithEverythingFlag(true);
    CScriptThing PreMeleeMazeTargetMarker = Game->GetThingWithScriptName("PreMeleeMazeTargetMarker");
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) break;
        if (PMasterData->GuildWarningOccuring) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (seh_me.IsValid())
                seh_me.Release();
            while (PMasterData->GuildWarningOccuring) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsDistanceBetweenThingsOver(Me, PreMeleeMazeTargetMarker, 4.0f) && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            seh_me.MoveToPosition(PreMeleeMazeTargetMarker.GetPos(), 3.0f, ENTITY_MOVE_WALK, false, true);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            seh_me.ClearCommands();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_028_MAZE_LEAVE_ME", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        goto scriptEnded;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    goto scriptEnded;
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            seh_me.ClearCommands();
#pragma inline_depth(0)
            CScriptGameResourceObjectMovieBase speech_movie;
#pragma inline_depth()
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_028_MAZE_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        goto scriptEnded;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    goto scriptEnded;
                }
            }
            Me.SetFriendsWithEverythingFlag(true);
        }
    }
    return;
scriptEnded:
    return _Fail_Val.Return();
}