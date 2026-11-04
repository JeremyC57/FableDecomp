// NScript::CV_KnotholeGladeGatesScript::CKGG_VillagerOutside::OnInterrupted() -- the villager outside the gates was interrupted: Attacked = true
// Leaf: ?OnInterrupted@CKGG_VillagerOutside@CV_KnotholeGladeGatesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA0050, 5 bytes: mov byte [ecx+1Dh],1; ret (thiscall, private virtual, entity vtable slot 6).
// Identity (naming wave 35c): slot 6 (OnInterrupted) of the RTTI-less entity vtable 0x012EDA38, whose slot 0 is the landed
//   CKGG_VillagerOutside::`scalar deleting destructor' 0x00EA0070 and slot 2 the landed Init 0x00EA0030 (the Alloc 0x00E9FF70 stores it);
//   the vtable dword 0x012EDA50 is the only reference to 0x00EA0050. Ego_r.pdb + FableWin.pdb
//   ?OnInterrupted@CKGG_VillagerOutside@CV_KnotholeGladeGatesScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CKGG_VillagerOutside (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, Moved +0x1c, Attacked +0x1d.
//   Was a generated stand-in CClass_00ea0050::sub_00ea0050 (unattributed_sub00ea0050_00ea0050.cpp).
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
class CV_KnotholeGladeGatesScript {
public:
    class CKGG_VillagerOutside;
};
class CV_KnotholeGladeGatesScript::CKGG_VillagerOutside : public CEntityScriptBase {
public:
    CV_KnotholeGladeGatesScript* ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    bool Moved;                                         // +0x1c
    bool Attacked;                                      // +0x1d
private:
    virtual void OnInterrupted();
};
}

void NScript::CV_KnotholeGladeGatesScript::CKGG_VillagerOutside::OnInterrupted()
{
    Attacked = true;
}
