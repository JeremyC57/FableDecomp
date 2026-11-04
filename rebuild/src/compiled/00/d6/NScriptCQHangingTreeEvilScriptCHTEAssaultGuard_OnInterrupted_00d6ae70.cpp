// NScript::CQ_HangingTreeEvilScript::CHTE_AssaultGuard::OnInterrupted() -- an assault guard ignores interruption (empty override)
// Leaf: ?OnInterrupted@CHTE_AssaultGuard@CQ_HangingTreeEvilScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D6AE70, 1 byte: `ret` (thiscall, private virtual, entity vtable slot 6).
// Identity (naming wave 35c): slot 6 (OnInterrupted) of the RTTI-less entity vtable 0x012D223C, whose slot 0 is the landed
//   CHTE_AssaultGuard::`scalar deleting destructor' 0x00D6AE80 and which the landed Alloc 0x00D6AD70 stores; the vtable dword
//   0x012D2254 is the only reference to 0x00D6AE70. FableWin.pdb ?OnInterrupted@CHTE_AssaultGuard@CQ_HangingTreeEvilScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
//   Was a gap-scan stand-in `T::Hook` (gapscan_sub00d6ae70_00d6ae70.cpp).
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
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeEvilScript {
public:
    class CHTE_AssaultGuard;
};
class CQ_HangingTreeEvilScript::CHTE_AssaultGuard : public CEntityScriptBase {
public:
    CQ_HangingTreeEvilScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void OnInterrupted();
};
}

void NScript::CQ_HangingTreeEvilScript::CHTE_AssaultGuard::OnInterrupted()
{
}
