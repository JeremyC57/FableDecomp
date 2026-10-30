// NScript::CQ_EndGameScript::CGatewayUndead::Main() -- keep the script alive only while its own thing lives (SetPredicate(CIsThingAlive(Me))), then idle a script frame at a time until the thread is terminating
// Leaf: ?Main@CGatewayUndead@CQ_EndGameScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D293F0, 104 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main, the entity-script contract [dtor, Main, Init, ...]) of the entity vtable 0x012CBC74 that the
//   landed CGatewayUndead::Alloc 0x00D29230 stores; FableWin.pdb ?Main@CGatewayUndead@CQ_EndGameScript@NScript@@EAEXXZ (private virtual).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840 (the predicate's member; the
//   CIsThingAlive vtable 0x012C32F0 stored before each), CEntityScriptBase::SetPredicate 0x00F35B10,
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, CGameScriptInterfaceBase::NewScriptFrame (slot 7, +0x1C).
// Shape (family K067, representative 0x00D293F0): the landed CHTE_Prisoner::Main 0x00D69F50 without the hand-over: the
//   predicate is a scoped 0x10-byte frame object; `NewScriptFrame(); while (!IsActiveThreadTerminating()) NewScriptFrame();`
//   rotates into the test, the aligned loop head and the bottom test.
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
class CScriptBase;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameScript {
public:
    class CGatewayUndead : public CEntityScriptBase {
    private:
        virtual void Main();
    public:
        CQ_EndGameScript* ParentClass;                             // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        float RaiseUndeadBarAmount;                     // +0x1C
    };
};
void CQ_EndGameScript::CGatewayUndead::Main()
{
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}
}