// NScript::CPersonalScript_EmptyScript::CMeanBandit::OnInterrupted() -- the bandit was interrupted: drop the box it carries and keep its weapons sheathed
// Leaf: ?OnInterrupted@CMeanBandit@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF42D0, 36 bytes (+int3 padding), thiscall, plain `ret` (private virtual, entity vtable slot 6).
// Identity (naming wave 35c): slot 6 (OnInterrupted) of the RTTI-less entity vtable 0x012F6488, whose slot 0 is the landed
//   CMeanBandit::`scalar deleting destructor' 0x00EF4300 (the Alloc 0x00EF4210 stores it); the vtable dword 0x012F64A0 is the only
//   reference to 0x00EF42D0. Ego_r.pdb + FableWin.pdb ?OnInterrupted@CMeanBandit@CPersonalScript_EmptyScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: the game interface (Game, +4) slots 505 EntityDropGenericBox(const CScriptThing&) const (+0x7e4) and 507
//   EntityUnsheatheWeapons(const CScriptThing&, bool) const (+0x7ec) (engine/CGameScriptInterfaceBase.h slot names); &Me stays in edi.
// Layout: Ego_r.pdb CMeanBandit (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18 (CEmptyData*).
//   Was a local stand-in `struct CMeanBandit` (CMeanBandit_OnInterrupted_00ef42d0.cpp, ?OnInterrupted@CMeanBandit@@QAEXXZ).
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
    class CMeanBandit;
};
class CPersonalScript_EmptyScript::CMeanBandit : public CEntityScriptBase {
public:
    CPersonalScript_EmptyScript* ParentClass;           // +0x14
    CEmptyData*                  PMasterData;           // +0x18
private:
    virtual void OnInterrupted();
};
}

void NScript::CPersonalScript_EmptyScript::CMeanBandit::OnInterrupted()
{
    Game->EntityDropGenericBox(Me);
    Game->EntityUnsheatheWeapons(Me, false);
}
