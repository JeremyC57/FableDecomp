#ifndef FABLE_ENGINE_NCAMERAMODES_CPHYSICSLEAF_H
#define FABLE_ENGINE_NCAMERAMODES_CPHYSICSLEAF_H
// NCameraModes::CPhysicsLeaf<TTarget, TForce, TCoord> (new, chunk 767) and the camera physics bases it needs.
// RTTI .?AV?$CPhysicsLeaf@VCPhysicsTargetModelGoTo@NCameraModes@@VCPhysicsForceModelGotoOutsideSimple@2@VCPhysicsCoordCartesian@2@@NCameraModes@@
// (vtable 0x01275974) and the ...CPhysicsForceModelRigid / CPhysicsCoordRigid instance (vtable 0x01276AEC), each : TTarget
// (CPhysicsTargetModelGoTo) : CSubComponentBase. Vtable slots: 0 ??_E, 1 Reset (0x00850630 / 0x0085CA00), 2 GetType
// (CPhysicsTargetModelGoTo, shared 0x0084FD10), 3 FrameUpdate. Ego_r.pdb struct_layouts_egor.tsv: CSubComponentBase { vptr, Owner +4,
// CameraManager +8 } (12 bytes); CPhysicsLeaf adds scoped_ptr<CPhysicsForceModelBase> OurForce +0x0C and
// scoped_ptr<CPhysicsCoordModelBase> OurCoords +0x10 (20 bytes). CPhysicsForceModelBase: slot 0 ~, slot 1 Reset (Ego_r.pdb donor order
// Reset 0x0091F750, FrameUpdate 0x0091F7F0). Only the virtuals rows use are declared.
#define FABLE_RETAIL_SCOPED_PTR
#include "rebuild_abi.h"
#include "boost/scoped_ptr.h"
class CGameCameraManager;
namespace NCameraModes {
class CBase;
class CPhysicsCoordModelBase;
class CPhysicsForceModelGotoOutsideSimple;
class CPhysicsForceModelRigid;
class CPhysicsCoordCartesian;
class CPhysicsCoordRigid;
class CSubComponentBase {
public:
    virtual ~CSubComponentBase();                        // slot 0
    virtual void Reset();                                // slot 1 ?Reset@CSubComponentBase@NCameraModes@@UAEXXZ
    CBase*              Owner;                           // +0x04
    CGameCameraManager* CameraManager;                   // +0x08
};
class CPhysicsTargetModelGoTo : public CSubComponentBase {
public:
    virtual ~CPhysicsTargetModelGoTo();                  // ??1CPhysicsTargetModelGoTo@NCameraModes@@UAE@XZ
};
class CPhysicsForceModelBase {
public:
    virtual ~CPhysicsForceModelBase();                   // slot 0
    virtual void Reset();                                // slot 1 ?Reset@CPhysicsForceModelBase@NCameraModes@@UAEXXZ
};
template <class TTarget, class TForce, class TCoord>
class CPhysicsLeaf : public TTarget {
public:
    virtual ~CPhysicsLeaf();                             // slot 0
    virtual void Reset();                                // slot 1
protected:
    boost::scoped_ptr<CPhysicsForceModelBase> OurForce;  // +0x0c
    boost::scoped_ptr<CPhysicsCoordModelBase> OurCoords; // +0x10
};
} // namespace NCameraModes
#endif // FABLE_ENGINE_NCAMERAMODES_CPHYSICSLEAF_H
