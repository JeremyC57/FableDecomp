// NScript::CV_KnotholeGladeGatesScript::CKGG_VillagerOutside::Init() -- not moved, not attacked, deed reactions off
// Leaf: ?Init@CKGG_VillagerOutside@CV_KnotholeGladeGatesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA0030, 29 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012EDA38, which the landed
//   NScript::CV_KnotholeGladeGatesScript::CKGG_VillagerOutside::Alloc 0x00E9FF70 stores (site 0x00E9FFA6); Ego_r.pdb has ?Init@CKGG_VillagerOutside@CV_KnotholeGladeGatesScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CKGG_VillagerOutside (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slot 595 EntitySetDeedReactionsEnabled.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
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
    CV_KnotholeGladeGatesScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    bool Moved;                                         // +0x1c
    bool Attacked;                                      // +0x1d
private:
    virtual void Init();
};

void CV_KnotholeGladeGatesScript::CKGG_VillagerOutside::Init()
{
    Moved = false;
    Attacked = false;
    Game->EntitySetDeedReactionsEnabled(Me, false);
}
}