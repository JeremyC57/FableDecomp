// Leaf: ?Main@CBattleMinion@CQ_EndGameFocalSitesScript@NScript@@EAEXXZ
// Flags: /O2 /Oy
// Inline-budget: retail expands the by-value script thing but calls the member copy in the spawned function and predicate.
#include "fable_compiler.h"
#include "engine/TRetVoid.h"
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CSPAWNEDFUNC1_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_END_GAME_BATTLE_MINION_PASS29
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTBASE_OUTOFLINE_CTOR
#include <stdlib.h>
#include "engine/CQ_EndGameFocalSitesScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
#include "engine/CSpawnedFunc1.h"
#ifdef _MSC_VER
#pragma inline_depth()
#endif
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/CIsThingAlive.h"
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
FABLE_FORCEINLINE NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
FABLE_FORCEINLINE NScript::CIsThingAlive::~CIsThingAlive() { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif

inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
FABLE_FORCEINLINE CScriptGameResourceObjectBase::CScriptGameResourceObjectBase() {}
FABLE_FORCEINLINE CScriptGameResourceObjectScriptedThingBase::CScriptGameResourceObjectScriptedThingBase() {}
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
inline CScriptThing::~CScriptThing() {}
template<>
FABLE_FORCEINLINE CSpawnedFunc1<NScript::CQ_EndGameFocalSitesScript,CScriptThing>::CSpawnedFunc1(
    const CCharString& name, unsigned long id, NScript::CQ_EndGameFocalSitesScript* pclass,
    void (NScript::CQ_EndGameFocalSitesScript::*func)(CScriptThing), CScriptThing par1)
    : CSpawnedFuncBase(name,id), Func(func), PClass(pclass), Par1(par1) {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
#ifdef _MSC_VER
#pragma inline_depth(1)
#endif
template class CSpawnedFunc1<NScript::CQ_EndGameFocalSitesScript,CScriptThing>;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
namespace NScript {
void CQ_EndGameFocalSitesScript::CBattleMinion::Main()
{
    TRetVoid _Fail_Val;
    unsigned long action = rand() % 4;
    (void)action;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    ParentClass.AddSpawnedFunction(
        new CSpawnedFunc1<CQ_EndGameFocalSitesScript, CScriptThing>(
            CCharString("ParentClass.") + CCharString("FadeCreatureUp"), 0, &ParentClass,
            &CQ_EndGameFocalSitesScript::FadeCreatureUp, Me),
        CCharString(""));
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    Game.EntitySetInFaction(Me, "FACTION_MONSTERS");
    Game.EntitySetFacingAngleTowardsThing(Me, Game.GetHero(), true);
    while (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 18.0f)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    FadeOutAndKillEntity(Me, 0.2f, true, true);
}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
}   // namespace NScript

#ifdef _MSC_VER
#pragma inline_depth(2)
#endif

#ifdef _MSC_VER
#pragma inline_depth()
#endif
