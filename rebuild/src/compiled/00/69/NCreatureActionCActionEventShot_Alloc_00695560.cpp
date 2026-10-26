// NCreatureAction::CActionEventShot::Alloc(long, CThingCreatureBase&, CGameCameraManager&) const -- allocate a new shot event for an action instance
// Leaf: ?Alloc@CActionEventShot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00695560, length 67 bytes (+ int3 padding), `ret 0Ch`, vtable 0x0125C13C slot 3.
// Callees: operator new @ 0x00BFEA1A (0x14 bytes), CBase::CBase @ 0x0099A2D0. Vtable 0x0125C13C.
// Name: FableWin.pdb `?Alloc@CActionEventShot@NCreatureAction@@UBEPAVCActionEventBase@2@JAAVCThingCreatureBase@@AAVCGameCameraManager@@@Z`
//   (the manifest "vfunc_3" is the third virtual after the dtor). Layout: Ego_r.pdb CActionEventBase {vptr, Finished +4,
//   ActivationFrame +8, PThing +0xc, PCameraManager +0x10}; CActionEventShot adds no data members (Ego_r.pdb: 20 bytes).
// Shape: `new` with its null test, then the inlined (activationFrame, thing, camera) constructor: the base members are stored
//   BEFORE this class's vtable store, so they belong to an inlined `__declspec(novtable)` CActionEventBase ctor (landed twin
//   model 0x006955F0); the derived ctor only stores its vtable.
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2D0
}; } using N_0099a2d0::CBase;

class CThingCreatureBase;
class CGameCameraManager;
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

namespace NCreatureAction {
struct __declspec(novtable) CActionEventBase : CBase {
    bool Finished;                                 // +0x04
    long ActivationFrame;                          // +0x08
    CThingCreatureBase* PThing;                    // +0x0c
    CGameCameraManager* PCameraManager;            // +0x10
    CActionEventBase(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera)
        : CBase(), Finished(false), ActivationFrame(activationFrame), PThing(&thing), PCameraManager(&camera) { }
    virtual void v01();
    virtual void v02();
    virtual CActionEventBase* Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const = 0;
};

struct CActionEventShot : CActionEventBase {   // vtable 0x0125C13C
    CActionEventShot(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera)
        : CActionEventBase(activationFrame, thing, camera) { }
    virtual CActionEventBase* Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const;
};
}

NCreatureAction::CActionEventBase* NCreatureAction::CActionEventShot::Alloc(long activationFrame,
    CThingCreatureBase& thing, CGameCameraManager& camera) const
{
    return new CActionEventShot(activationFrame, thing, camera);
}