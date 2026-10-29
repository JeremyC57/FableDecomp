// NCreatureAction::CActionEventRemoveObject::Alloc(long, CThingCreatureBase&, CGameCameraManager&) const -- allocate a new remove-object event for an action instance
// Leaf: ?Alloc@CActionEventRemoveObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00695710, length 75 bytes (+ int3 padding), `ret 0Ch`, vtable 0x0125C1A0 slot 3.
// Callees: operator new @ 0x00BFEA1A (0x18 bytes), CBaseClass::CBaseClass @ 0x0099A2D0, CCharString::CCharString() @ 0x0099E4B0. Vtable 0x0125C1A0.
// Name: FableWin.pdb `?Alloc@CActionEventRemoveObject@NCreatureAction@@UBEPAVCActionEventBase@2@JAAVCThingCreatureBase@@AAVCGameCameraManager@@@Z`
//   (the manifest "vfunc_3" is the third virtual after the dtor). Layout: Ego_r.pdb CActionEventBase {vptr, Finished +4,
//   ActivationFrame +8, PThing +0xc, PCameraManager +0x10}; CActionEventRemoveObject adds Hand (CCharString) +0x14.
// Shape: `new` with its null test, then the inlined (activationFrame, thing, camera) constructor: the base members are stored
//   BEFORE this class's vtable store, so they belong to an inlined `__declspec(novtable)` CActionEventBase ctor (landed twin
//   model 0x006955F0); after the vtable store the Hand string is default-constructed out of line (landed default ctor 0x006922C0).
#include "fable_string.h"

class CBaseClass {
public:
    virtual ~CBaseClass();
    CBaseClass();                                       // 0x0099A2D0
};

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

struct CActionEventRemoveObject : CActionEventBase {   // vtable 0x0125C1A0
    CCharString Hand;                              // +0x14
    CActionEventRemoveObject(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera)
        : CActionEventBase(activationFrame, thing, camera), Hand() { }
    virtual CActionEventBase* Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const;
};
}

NCreatureAction::CActionEventBase* NCreatureAction::CActionEventRemoveObject::Alloc(long activationFrame,
    CThingCreatureBase& thing, CGameCameraManager& camera) const
{
    return new CActionEventRemoveObject(activationFrame, thing, camera);
}