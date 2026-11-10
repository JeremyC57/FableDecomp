#ifndef FABLE_ENGINE_NCAMERAMODES_CPHYSICSLEAF_H
#define FABLE_ENGINE_NCAMERAMODES_CPHYSICSLEAF_H
#ifdef FABLE_RETAIL_NCAMERAMODES_CPHYSICSLEAF_CYLINDER
#include "rebuild_abi.h"
#define FABLE_RETAIL_SCOPED_PTR
#include "boost/scoped_ptr.h"
class CGameCameraManager;
namespace NCameraModes {
class CBase; class CPhysicsState; class CTools; class CPhysicsForceModelBase; class CPhysicsCoordModelBase; class CPhysicsForceModelRigid; class CPhysicsCoordRigid; enum ESubComponentType;
class CSubComponentBase {
public:
 virtual void FrameUpdate() = 0;
 virtual void Reset();
 virtual NCameraModes::ESubComponentType GetType() const = 0;
 CSubComponentBase(const NCameraModes::CSubComponentBase & p0);
 CSubComponentBase(NCameraModes::CBase & p0, const CGameCameraManager & p1);
 virtual ~CSubComponentBase();
protected:
 const NCameraModes::CBase & GetOwner() const;
 NCameraModes::CBase & GetOwner();
 const NCameraModes::CTools & GetCameraTools() const;
 const CGameCameraManager & GetCameraManager() const;
private:
 NCameraModes::CBase & Owner;
 const CGameCameraManager & CameraManager;
};
class CPhysicsTargetModelGoTo : public CSubComponentBase {
public:
 CPhysicsTargetModelGoTo(const NCameraModes::CPhysicsTargetModelGoTo & p0);
 CPhysicsTargetModelGoTo(NCameraModes::CBase & p0, const CGameCameraManager & p1);
 static NCameraModes::ESubComponentType FABLE_FASTCALL StatGetType();
 virtual NCameraModes::ESubComponentType GetType() const;
 virtual void Reset() = 0;
 NCameraModes::CPhysicsState & GetPhysicsState();
 virtual ~CPhysicsTargetModelGoTo();
};
template<class TTarget,class TForce,class TCoord> class CPhysicsLeaf : public TTarget {
public:
 CPhysicsLeaf(CBase&,const CGameCameraManager&);
 virtual void FrameUpdate();
 virtual void Reset();
 virtual ~CPhysicsLeaf();
private:
 boost::scoped_ptr<CPhysicsForceModelBase> OurForce;
 boost::scoped_ptr<CPhysicsCoordModelBase> OurCoords;
};
}

#else
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
#endif // FABLE_RETAIL_NCAMERAMODES_CPHYSICSLEAF_CYLINDER
#endif // FABLE_ENGINE_NCAMERAMODES_CPHYSICSLEAF_H
