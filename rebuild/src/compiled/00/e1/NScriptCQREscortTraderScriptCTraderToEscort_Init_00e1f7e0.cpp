// 00e1f7e0
// Leaf: ?Init@CTraderToEscort@CQR_EscortTraderScript@NScript@@EAEXXZ
// Flags: /O2 /Oy
// Inline-budget: Match the native CScriptThing cleanup depth and expand the CSpawnedFunc1 constructor while retaining its member calls.
#define FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY
#define FABLE_RETAIL_ESCORT_TRADER_INIT_PASS29
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CSPAWNEDFUNC1_ORIGINAL_ACCESS
#include "engine/EScriptingStateGroups.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/NScript_CQR_EscortTraderScript.h"
#include "engine/fable_script_def_global.h"
#ifdef _MSC_VER
#pragma inline_depth(2)
#endif
inline CScriptThing::~CScriptThing() {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
template<>
FABLE_FORCEINLINE CSpawnedFunc1<NScript::CQR_EscortTraderScript,CScriptThing>::CSpawnedFunc1(
    const CCharString& name, unsigned long id, NScript::CQR_EscortTraderScript* pclass,
    void (NScript::CQR_EscortTraderScript::*func)(CScriptThing), CScriptThing par1)
    : CSpawnedFuncBase(name,id), Func(func), PClass(pclass), Par1(par1) {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
void NScript::CQR_EscortTraderScript::CTraderToEscort::Init()
{
    Game.SetThingHasInformation(Me, true, false, false);
    Game.SetThingPersistent(Me, true);
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForTraderDead"), 0, &ParentClass, &CQR_EscortTraderScript::WatchForTraderDead, Me), CCharString(""));
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForTraderHit"), 0, &ParentClass, &CQR_EscortTraderScript::WatchForTraderHit, Me), CCharString(""));
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForExpressionFollow"), 0, &ParentClass, &CQR_EscortTraderScript::WatchForExpressionFollow, Me), CCharString(""));
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForExpressionWait"), 0, &ParentClass, &CQR_EscortTraderScript::WatchForExpressionWait, Me), CCharString(""));
    lastRandomSpeechFollowIdx1 = 10;
    lastRandomSpeechFollowIdx2 = 10;
    lastRandomSpeechWaitIdx1 = 5;
    lastRandomSpeechWaitIdx2 = 5;
    Game.SetWanderCentrePoint(Me, Me.GetPos());
    Game.SetWanderMinDistance(Me, 0.0f);
    Game.SetWanderMaxDistance(Me, pGlobalScriptDef->EscortTraderWanderDistance);
    Game.SetScriptingStateGroup(Me, ESSG_WANDER_NEAR);
    Game.EntitySetAsAllowedToFollowHero(Me, false);
}