// NScript::CV_ExposeMayorScript::CEM_SuspiciousVillager::Init() -- intro/outro not done; the villager has information and is a valid target without line of sight
// Leaf: ?Init@CEM_SuspiciousVillager@CV_ExposeMayorScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E76410, 49 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012E9898, which the landed
//   NScript::CV_ExposeMayorScript::CEM_SuspiciousVillager::Alloc 0x00E76360 stores (site 0x00E76396); Ego_r.pdb has ?Init@CEM_SuspiciousVillager@CV_ExposeMayorScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CEM_SuspiciousVillager (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 360 SetThingHasInformation, 471 EntitySetTargetingValidTargetWithoutLOS.
class CCharString;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
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
class CV_ExposeMayorScript {
public:
    class CEM_SuspiciousVillager;
};
class CV_ExposeMayorScript::CEM_SuspiciousVillager : public CEntityScriptBase {
public:
    CV_ExposeMayorScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    bool DoneIntro;                                     // +0x1c
    bool DoneOutro;                                     // +0x1d
private:
    virtual void Init();
};

void CV_ExposeMayorScript::CEM_SuspiciousVillager::Init()
{
    DoneIntro = false;
    DoneOutro = false;
    Game->SetThingHasInformation(Me, true, true, false);
    Game->EntitySetTargetingValidTargetWithoutLOS(Me, true);
}
}