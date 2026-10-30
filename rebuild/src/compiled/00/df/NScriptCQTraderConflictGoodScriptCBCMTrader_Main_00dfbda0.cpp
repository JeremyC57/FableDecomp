// NScript::CQ_TraderConflictGoodScript::CBCMTrader::Main() -- removes the entity itself: Game->RemoveThing(Me, false, false)
// Leaf: ?Main@CBCMTrader@CQ_TraderConflictGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DFBDA0, 22 bytes (+ int3 padding to 32), thiscall, plain `ret`.
// Identity (proven): NScript::CQ_TraderConflictGoodScript::CBCMTrader::Main(); evidence: this body is slot 1 (Main) of the RTTI-less entity-script vtable 0x012DEDDC (.rdata 0x012DEDE0); that vtable is stored, after the CEntityScriptBase vtable 0x012C3224, by the landed factory 0x00DFBCF0 ?Alloc@CBCMTrader@CQ_TraderConflictGoodScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z (`new` 0x1C bytes); the Ego_r/FableWin.pdb name sets hold ?Main@CBCMTrader@CQ_TraderConflictGoodScript@NScript@@EAEXXZ
// Label: the manifest's "sub_00dfbda0" is a gap-scan placeholder (family J033: six one-call entity-script Main / Init bodies).
// Callee: CGameScriptInterfaceBase virtual slot 108 (+0x1b0) RemoveThing(const CScriptThing&, bool, bool) const, the slot
//   ghidra_out/cgsi_slots_aligned.tsv names ?RemoveThing@CGameScriptInterface@@UBEXABVCScriptThing@@_N1@Z (implementation 0x008910D0).
// Layout: Ego_r.pdb CEntityScriptBase (0x14) {vptr, Game (CGameScriptInterfaceBase*) +4, Me (CScriptThing, 0x0C) +8};
//   CBCMTrader (0x1C): ParentClass (CQ_TraderConflictGoodScript*) +0x14, PMasterData (CQ_SunnyvaleMasterData*) +0x18.
// Shape: `mov eax, ecx`, the interface vptr through Game, the two bools pushed as immediates, `add eax, 8` for &Me.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderConflictGoodScript {
public:
    class CBCMTrader;
};
class CQ_TraderConflictGoodScript::CBCMTrader : public CEntityScriptBase {   // vtable 0x012DEDDC
private:
    virtual void Main();                               // slot 1
public:
    CQ_TraderConflictGoodScript*            ParentClass;            // +0x14
    CQ_SunnyvaleMasterData* PMasterData;            // +0x18
};
void CQ_TraderConflictGoodScript::CBCMTrader::Main()
{
    Game->RemoveThing(Me, false, false);
}
}