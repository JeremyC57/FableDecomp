// NScript::CV_BordelloScript::CMagicman::Init() -- the bordello magician: has information, cannot be killed, damaged, pushed or force-pushed and adds nothing to the combo multiplier; both speech flags cleared
// Leaf: ?Init@CMagicman@CV_BordelloScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3B060, 142 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012E3B2C, which the landed
//   NScript::CV_BordelloScript::CMagicman::Alloc 0x00E3AF90 stores (site 0x00E3AFC6); Ego_r.pdb has ?Init@CMagicman@CV_BordelloScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CMagicman (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 360 SetThingHasInformation, 517 EntitySetAsKillable, 516 EntitySetAsDamageable, 526
//   EntitySetAsToAddToComboMultiplierWhenHit, 844 SetIsPushableByHero (Me by value), 745 SetIsThingForcePushable.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
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
class CV_BordelloScript {
public:
    class CMagicman;
};
class CV_BordelloScript::CMagicman : public CEntityScriptBase {
public:
    CV_BordelloScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    bool DoneToutIntro;                                 // +0x1c
    bool MentionedPimpHat;                              // +0x1d
private:
    virtual void Init();
};

void CV_BordelloScript::CMagicman::Init()
{
    Game->SetThingHasInformation(Me, true, false, false);
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->SetIsThingForcePushable(Me, false);
    DoneToutIntro = false;
    MentionedPimpHat = false;
}
}