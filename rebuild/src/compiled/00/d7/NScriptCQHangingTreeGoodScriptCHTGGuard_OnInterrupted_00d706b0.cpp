// NScript::CQ_HangingTreeGoodScript::CHTG_Guard::OnInterrupted() -- a guard was interrupted (spoken to): the parent script's LeaderSpokenTo = true
// Leaf: ?OnInterrupted@CHTG_Guard@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D706B0, 8 bytes: mov eax,[ecx+14h] (ParentClass); mov byte [eax+48h],1; ret (thiscall, private
//   virtual, entity vtable slot 6).
// Identity (naming wave 35c): slot 6 (OnInterrupted) of the RTTI-less entity vtable 0x012D27A8, whose slot 0 is the landed
//   CHTG_Guard::`scalar deleting destructor' 0x00D706C0 and slot 5 the landed OnPredicateFail 0x00D70650 (the Alloc 0x00D70520 stores
//   it); the vtable dword 0x012D27C0 is the only reference to 0x00D706B0. Ego_r.pdb + FableWin.pdb
//   ?OnInterrupted@CHTG_Guard@CQ_HangingTreeGoodScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CHTG_Guard (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18. Parent (retail = Ego_r.pdb - 0x14, as the
//   landed OnPredicateFail's HTG_State +0x5c / CurrentGuards +0x64): retail +0x48 is Ego_r +0x5c LeaderSpokenTo (bool).
//   Was a generated stand-in CClass_00d706b0::SetFlag_00d706b0 (unattributed_SetFlag00d706b0_00d706b0.cpp).
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
class CQ_HangingTreeGoodScript {
public:
    class CHTG_Guard;
    unsigned char _pad000[0x48];
    bool LeaderSpokenTo;                                // +0x48 (retail = Ego_r.pdb - 0x14)
};
class CQ_HangingTreeGoodScript::CHTG_Guard : public CEntityScriptBase {
public:
    CQ_HangingTreeGoodScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void OnInterrupted();
};
}

void NScript::CQ_HangingTreeGoodScript::CHTG_Guard::OnInterrupted()
{
    ParentClass->LeaderSpokenTo = true;
}
