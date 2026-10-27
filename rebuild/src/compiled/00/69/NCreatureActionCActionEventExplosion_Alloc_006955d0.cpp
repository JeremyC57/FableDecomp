// NCreatureAction::CActionEventExplosion::Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const -- a new CActionEventExplosion for an action instance (0 when the allocation fails)
// Leaf: ?Alloc@CActionEventExplosion@NCreatureAction@@UBEPAVCActionEventBase@2@JAAVCThingCreatureBase@@AAVCGameCameraManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006955D0, 26 bytes, thiscall, `ret 0Ch`.
// Identity (proven): slot 3 of vtable 0x0125C154 (RTTI .?AVCActionEventExplosion@NCreatureAction@@); FableWin.pdb
//   ?Alloc@CActionEventExplosion@NCreatureAction@@UBEPAVCActionEventBase@2@JAAVCThingCreatureBase@@AAVCGameCameraManager@@@Z. The manifest's
//   "vfunc_3" is a placeholder.
// Callees: operator new 0x00BFEA1A (0x20 bytes), CActionEventExplosion::CActionEventExplosion(long, CThingCreatureBase&, CGameCameraManager&) 0x006955F0
//   (landed; a tail jump).
// Shape (family M195, representative 0x006953E0): `return new CActionEventExplosion(activationFrame, thing, camera);` -- the constructor is out
//   of line here, so VC7.1 null-tests new's result and jumps to the constructor with this call's own arguments still in place
//   (its `ret 0Ch` returns for both). Unlike the landed CActionEventRemoveObject::Alloc 0x00695710, whose constructor inlines.
void* __cdecl operator new(unsigned int size);            // 0x00BFEA1A
class CThingCreatureBase;
class CGameCameraManager;
class CBaseClass {                                         // vtable 0x01231710, RTTI .?AVCBaseClass@@
public:
    virtual ~CBaseClass();
};
namespace NCreatureAction {
class CActionEventBase : public CBaseClass {
public:
    virtual void UnprovenSlot01();
    virtual void UnprovenSlot02();
    virtual CActionEventBase* Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const = 0;
    bool Finished;                                        // +0x04
    long ActivationFrame;                                 // +0x08
    CThingCreatureBase* PThing;                           // +0x0C
    CGameCameraManager* PCameraManager;                   // +0x10
};
class CActionEventExplosion : public CActionEventBase {
public:
    CActionEventExplosion(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera);   // 0x006955F0
    virtual CActionEventBase* Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const;
    unsigned char Members[0x20 - 0x14];               // +0x14
};
CActionEventBase* CActionEventExplosion::Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const
{
    return new CActionEventExplosion(activationFrame, thing, camera);
}
}