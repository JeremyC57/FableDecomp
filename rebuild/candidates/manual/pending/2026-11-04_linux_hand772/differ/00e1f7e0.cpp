// NScript::CQR_EscortTraderScript::CTraderToEscort::Init() -- escort-trader entity setup: Me gets information and persistence, the parent script spawns WatchForTraderDead / WatchForTraderHit / WatchForExpressionFollow / WatchForExpressionWait on Me, the random-speech indices reset (10, 10, 5, 5), Me wanders around its own position (0 .. EscortTraderWanderDistance) in scripting state group 4 and may not follow the hero
// Grade: DIFFER(1357v1286) -- each CSpawnedFunc1 ctor's Par1 copy is expanded inline here; retail calls CScriptThing(const CScriptThing&) 0x004ABE90 there while expanding the by-value argument copies (the two-view-CScriptThing / inline-depth family, docs/pipeline/INLINE_DEPTH_REVISIT.md)
// Leaf: ?Init@CTraderToEscort@CQR_EscortTraderScript@NScript@@EAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1F7E0, 1286 bytes, thiscall, plain `ret`, no EH frame. Literals "ParentClass." 0x012C9934,
//   "WatchForTraderDead" 0x012E1674, "WatchForTraderHit" 0x012E1660, "WatchForExpressionFollow" 0x012E1644,
//   "WatchForExpressionWait" 0x012E162C, "" 0x0122D70E.
// Identity (two sources): slot 2 of the entity vtable 0x012E151C (slot 1 Main 0x00E20600); Ego_r.pdb
//   ?Init@CTraderToEscort@CQR_EscortTraderScript@NScript@@EAEXXZ (CTraderToEscort 0x2c: ParentClass +0x14, PMasterData +0x18,
//   lastRandomSpeechFollowIdx1/2 +0x1c/+0x20, lastRandomSpeechWaitIdx1/2 +0x24/+0x28); the spawned members are
//   CQR_EscortTraderScript's (landed WatchForExpressionFollow 0x00E1FCF0 / WatchForExpressionWait 0x00E1FEF0 are the stored member
//   pointers; WatchForTraderDead 0x00E200B0 and WatchForTraderHit 0x00E20210 by the same literal/member-pointer pairing; PDB
//   ?WatchForTraderDead@CQR_EscortTraderScript@NScript@@QAEXVCScriptThing@@@Z / ?WatchForTraderHit@..). Outer class = CQR_EscortTraderScript.
// Callees: operator new 0x00BFEA1A (0x48), CCharString 0x0099EBF0 / ~ 0x0099EAE0, operator+(const CCharString&, const CCharString&)
//   0x0099F570 (new FABLE_RETAIL_CCHARSTRING_CONCAT_STRING declaration in fable_string.h), CSpawnedFuncBase 0x00CDD450,
//   CScriptBase::AddSpawnedFunction 0x00CB7E50, CScriptThing copy 0x004ABE90 / ~ 0x004AA840; CGameScriptInterfaceBase slots 235,
//   360, 477, 763, 766, 769, 784; pGlobalScriptDef->EscortTraderWanderDistance +0x9d0.
// Tried (6 grades): implicit CScriptThing copy (b), explicit plain-inline copy (c), plain-inline CSpawnedFunc1 ctor (d), /Ob1 (e):
//   all expand the member copy (dl=61). Next idea: the retail mechanism for the nested out-of-line copy inside the expanded functor ctor.
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
enum EScriptingStateGroups { };
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
namespace NScript {
class CQR_EscortTraderScript::CTraderToEscort : public CEntityScriptBase {
public:
    typedef CQR_EscortTraderScript TParentClass;
    virtual CScriptBase* GetParentScript() const;
private:
    virtual void Init();
    virtual void Main();
protected:
    CQR_EscortTraderScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    long lastRandomSpeechFollowIdx1;                    // +0x1c
    long lastRandomSpeechFollowIdx2;                    // +0x20
    long lastRandomSpeechWaitIdx1;                      // +0x24
    long lastRandomSpeechWaitIdx2;                      // +0x28
};
}
void NScript::CQR_EscortTraderScript::CTraderToEscort::Init()
{
    Game->SetThingHasInformation(Me, true, false, false);
    Game->SetThingPersistent(Me, true);
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForTraderDead"), 0, ParentClass, &CQR_EscortTraderScript::WatchForTraderDead, Me), CCharString(""));
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForTraderHit"), 0, ParentClass, &CQR_EscortTraderScript::WatchForTraderHit, Me), CCharString(""));
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForExpressionFollow"), 0, ParentClass, &CQR_EscortTraderScript::WatchForExpressionFollow, Me), CCharString(""));
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForExpressionWait"), 0, ParentClass, &CQR_EscortTraderScript::WatchForExpressionWait, Me), CCharString(""));
    lastRandomSpeechFollowIdx1 = 10;
    lastRandomSpeechFollowIdx2 = 10;
    lastRandomSpeechWaitIdx1 = 5;
    lastRandomSpeechWaitIdx2 = 5;
    Game->SetWanderCentrePoint(Me, Me.GetPos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, pGlobalScriptDef->EscortTraderWanderDistance);
    Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)4);
    Game->EntitySetAsAllowedToFollowHero(Me, false);
}
