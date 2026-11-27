// 00f03980 NScript::CQ_AmbushTradersScript::CAT_Phase1Guard::Main
// Leaf: ?Main@CAT_Phase1Guard@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Flags: none
// Inline-budget: The native cleanup after FadeOutAndKillEntity makes an out-of-line destructor call at this closing brace.
#define FABLE_RETAIL_PHASE1_GUARD_PASS29
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#include "engine/EScriptEntityMoveType.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/EScriptAIPriority.h"
#include "engine/C3DVector.h"
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_fade.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
#include "engine/fable_script_distance.h"
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
inline CScriptThing::~CScriptThing() {}
void NScript::CQ_AmbushTradersScript::CAT_Phase1Guard::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase sehMe;
    CScriptThing walkToMkr = Game->GetThingWithScriptName(Me.GetDataString());
    if (walkToMkr.IsAlive()) {
        if (IsActiveThreadTerminating()) {
            _Fail_Val.Return();
            return;
        }
        if (sehMe.IsValid())
            sehMe.Release();
        while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
        }
        if (IsActiveThreadTerminating()) {
            _Fail_Val.Return();
            return;
        }
        sehMe.MoveToPosition(walkToMkr.GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
        while (!IsDistanceBetweenThingsUnder(Me, walkToMkr, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
        }
        if (IsActiveThreadTerminating()) {
            _Fail_Val.Return();
            return;
        }
        CScriptThing lookAtMkr = Game->GetThingWithScriptName("M_AT_ConvoyExitCSTrigger");

        if (lookAtMkr.IsAlive()) {

            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            Game->EntityForceToLookAtThing(Me, lookAtMkr);
        }
        if (sehMe.IsValid())
            sehMe.Release();

    }

    Game->SetWanderCentrePoint(Me, Me.GetPos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, pGlobalScriptDef->AmbushTradersPhase1GuardWanderDistance);
    Game->SetScriptingStateGroup(Me, ESSG_WANDER_NEAR);
    if (false) {} else for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) {
            _Fail_Val.Return();
            return;
        }
        if (Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me)) {
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            Game->SetScriptingStateGroup(Me, ESSG_NONE);
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            do {
                Game->NewScriptFrame();
            } while (!IsActiveThreadTerminating());
            {

                _Fail_Val.Return(); return;

            }
        }
        if (ParentClass.QuestFailed || ParentClass.QuestFinished) {
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            if (sehMe.IsValid())
                sehMe.Release();
            while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) {
                    _Fail_Val.Return();
                    return;
                }
            }
            if (IsActiveThreadTerminating()) {
                _Fail_Val.Return();
                return;
            }
            CScriptThing Exit = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
            if (Exit.IsAlive()) {
                if (IsActiveThreadTerminating()) {
                    _Fail_Val.Return();
                    return;
                }
                if (sehMe.IsValid()) {
                    if (IsActiveThreadTerminating()) {
                        _Fail_Val.Return();
                        return;
                    }
                    sehMe.MoveToPosition(Exit.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                }
                while (!IsDistanceBetweenThingsUnder(Me, Exit, 2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        _Fail_Val.Return();
                        return;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    _Fail_Val.Return();
                    return;
                }
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif

        }
#ifdef _MSC_VER
#pragma inline_depth()
#endif

    }
}