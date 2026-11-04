// NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonGuard::OnInterrupted() -- a prison guard ignores interruption (empty override)
// Leaf: ?OnInterrupted@CPrisonGuard@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD44B0, 1 byte: `ret` (thiscall, private virtual, entity vtable slot 6).
// Identity (naming wave 35c): slot 6 (OnInterrupted) of the RTTI-less entity vtable 0x012DBE68, whose slot 0 is the landed
//   CPrisonGuard::`scalar deleting destructor' 0x00DD92D0 and which the landed Alloc 0x00DD43D0 stores; the vtable dword
//   0x012DBE80 is the only reference to 0x00DD44B0. FableWin.pdb ?OnInterrupted@CPrisonGuard@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
//   Was a gap-scan stand-in `T::Hook` (gapscan_sub00dd44b0_00dd44b0.cpp).
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
class CQ_PrisonEscapeRescueMotherScript {
public:
    class CPrisonGuard;
};
class CQ_PrisonEscapeRescueMotherScript::CPrisonGuard : public CEntityScriptBase {
public:
    CQ_PrisonEscapeRescueMotherScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void OnInterrupted();
};
}

void NScript::CQ_PrisonEscapeRescueMotherScript::CPrisonGuard::OnInterrupted()
{
}
