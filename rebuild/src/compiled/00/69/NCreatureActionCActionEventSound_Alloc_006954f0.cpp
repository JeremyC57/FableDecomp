// NCreatureAction::CActionEventSound::Alloc(long, CThingCreatureBase&, CGameCameraManager&) const -- allocate a new sound event for an action instance
// Leaf: ?Alloc@CActionEventSound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006954F0, length 67 bytes (+ int3 padding), `ret 0Ch`, vtable 0x0125C11C slot 3.
// Callees: operator new @ 0x00BFEA1A (0x14 bytes), CBaseClass::CBaseClass @ 0x0099A2D0. Vtable 0x0125C11C.
// Name: FableWin.pdb `?Alloc@CActionEventSound@NCreatureAction@@UBEPAVCActionEventBase@2@JAAVCThingCreatureBase@@AAVCGameCameraManager@@@Z`
//   (the manifest "vfunc_3" is the third virtual after the dtor). Layout: Ego_r.pdb CActionEventBase {vptr, Finished +4,
//   ActivationFrame +8, PThing +0xc, PCameraManager +0x10}; CActionEventSound adds no data members (Ego_r.pdb: 20 bytes).
// Shape: `new` with its null test, then the inlined (activationFrame, thing, camera) constructor: the base members are stored
//   BEFORE this class's vtable store, so they belong to an inlined `__declspec(novtable)` CActionEventBase ctor (landed twin
//   model 0x006955F0); the derived ctor only stores its vtable.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CThingCreatureBase;
class CGameCameraManager;
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

namespace NCreatureAction {
struct __declspec(novtable) CActionEventBase : CBaseClass {
    bool Finished;                                 // +0x04
    long ActivationFrame;                          // +0x08
    CThingCreatureBase* PThing;                    // +0x0c
    CGameCameraManager* PCameraManager;            // +0x10
    CActionEventBase(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera)
        : CBaseClass(), Finished(false), ActivationFrame(activationFrame), PThing(&thing), PCameraManager(&camera) { }
    virtual void v01();
    virtual void v02();
    virtual CActionEventBase* Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const = 0;
};

struct CActionEventSound : CActionEventBase {   // vtable 0x0125C11C
    CActionEventSound(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera)
        : CActionEventBase(activationFrame, thing, camera) { }
    virtual CActionEventBase* Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const;
};
}

NCreatureAction::CActionEventBase* NCreatureAction::CActionEventSound::Alloc(long activationFrame,
    CThingCreatureBase& thing, CGameCameraManager& camera) const
{
    return new CActionEventSound(activationFrame, thing, camera);
}