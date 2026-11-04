// NScript::CPersonalScript_EmptyScript::CVillager::OnInterrupted() -- if the villager's crate is alive and carried by the villager, and the
//   active thread is not terminating, drop it
// Leaf: ?OnInterrupted@CVillager@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF4950, 78 bytes (+int3 padding), thiscall, plain `ret` (private virtual, entity vtable slot 6).
// Identity (naming wave 35c): slot 6 (OnInterrupted) of the RTTI-less entity vtable 0x012F6518, whose slot 0 is the landed
//   CVillager::`scalar deleting destructor' 0x00EF5040 (the Alloc 0x00EF48A0 stores it); the vtable dword 0x012F6530 is the only
//   reference to 0x00EF4950. Ego_r.pdb + FableWin.pdb ?OnInterrupted@CVillager@CPersonalScript_EmptyScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: CScriptThing slots 75 IsAlive (+0x12c), 1 GetName (+0x04), 63 IsBeingCarriedBy(const CCharString&) (+0xfc) (engine/CScriptThing.h);
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; the game interface slot 505 EntityDropGenericBox (+0x7e4).
// Shape: nested ifs; Crate's vtable is fetched into ebp before the inner Me.GetName() call; &Me in edi is reused as the final argument.
// Layout: Ego_r.pdb CVillager (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18 (CEmptyData*), Crate +0x1c.
//   Was a local stand-in `struct CVillager` (CVillager_OnInterrupted_00ef4950.cpp, ?OnInterrupted@CVillager@@QAEXXZ).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CEmptyData;
class CPersonalScript_EmptyScript {
public:
    class CVillager;
};
class CPersonalScript_EmptyScript::CVillager : public CEntityScriptBase {
public:
    CPersonalScript_EmptyScript* ParentClass;           // +0x14
    CEmptyData*                  PMasterData;           // +0x18
    CScriptThing                 Crate;                 // +0x1c
private:
    virtual void OnInterrupted();
};
}

void NScript::CPersonalScript_EmptyScript::CVillager::OnInterrupted()
{
    if (Crate.IsAlive()) {
        if (Crate.IsBeingCarriedBy(Me.GetName())) {
            if (!IsActiveThreadTerminating())
                Game->EntityDropGenericBox(Me);
        }
    }
}
