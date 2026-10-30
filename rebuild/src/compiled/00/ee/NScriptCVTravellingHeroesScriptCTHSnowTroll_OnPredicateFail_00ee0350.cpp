// NScript::CV_TravellingHeroesScript::CTH_SnowTroll::OnPredicateFail() -- the snow troll is gone: when the hero killed it, switch the LostBay creature generators back on and reactivate the region entrance and exit
// Leaf: ?OnPredicateFail@CTH_SnowTroll@CV_TravellingHeroesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE0350, 131 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012F42B0, which the landed
//   constructor CTH_SnowTroll::CTH_SnowTroll 0x00EE02C0 (factory 0x00EE0240) stores (site 0x00EE02EA); Ego_r.pdb has ?OnPredicateFail@CTH_SnowTroll@CV_TravellingHeroesScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CTH_SnowTroll (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptThing slot 18 MsgIsKilledBy ("" 0x0122D70E temporary); interface slots 558 SetCreatureGeneratorsEnabled
//   ("LostBay" 0x012C4744 temporary), 568 SetRegionEntranceAsActive, 567 SetRegionExitAsActive; CCharString(const char*,
//   long) 0x0099EBF0 / ~CCharString 0x0099EAE0.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
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
class CV_TravellingHeroesScript {
public:
    class CTH_SnowTroll;
};
class CV_TravellingHeroesScript::CTH_SnowTroll : public CEntityScriptBase {
public:
    CV_TravellingHeroesScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    CScriptThing Entrance;                              // +0x1c
    CScriptThing Exit;                                  // +0x28
private:
    virtual void OnPredicateFail();
};

void CV_TravellingHeroesScript::CTH_SnowTroll::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        Game->SetCreatureGeneratorsEnabled("LostBay", true);
        Game->SetRegionEntranceAsActive(Entrance, true);
        Game->SetRegionExitAsActive(Exit, true);
    }
}
}