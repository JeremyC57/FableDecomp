// NCameraModes::CFixedDolly::GetPhysicsMode() -- the fixed dolly's physics: a rigid go-to physics leaf owned through a new counted pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087B770, 106 bytes, `ret 4` (CCountedPointer returned through the hidden pointer).
// Leaf: ?GetPhysicsMode@CFixedDolly@NCameraModes@@UAE?AV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Name: vtable 0x0125C48C slot 26; FableWin.pdb ?GetPhysicsMode@CFixedDolly@NCameraModes@@UAE?AV?$CCountedPointer@VCSubComponentBase@NCameraModes@@@@XZ
//       (CCameraPointModeBase::GetPhysicsMode is the same slot).
// Callees: operator new @ 0x00BFEA1A (0x14-byte leaf, 0xc-byte pointer info), CPhysicsLeaf<CPhysicsTargetModelGoTo,
//          CPhysicsForceModelRigid, CPhysicsCoordRigid>::CPhysicsLeaf(CBase&, const CGameCameraManager&) @ 0x0085C950 (landed;
//          FableWin.pdb signature); the info block's delete function CCountedPointer<CSubComponentBase>::DeleteData @ 0x00850000.
// Layout: NCameraModes::CBase::CameraManager at +0x40 (retail; landed CStringAndCageBase::OnInit 0x00872630).
// Shape: `new` null-tests both blocks; the counted pointer's T* ctor stores Data, then `if (Data) Info = new info(DeleteData,
//        Data); else Info = 0;` reading the stored member back (three exits: the info, new's null, the literal 0).
void* __cdecl operator new(unsigned int size);       // 0x00BFEA1A
class CGameCameraManager;
namespace NCameraModes {
class CSubComponentBase;
class CBase;
class CPhysicsTargetModelGoTo;
class CPhysicsForceModelRigid;
class CPhysicsCoordRigid;
}
#include "engine/CCountedPointer.h"
namespace NCameraModes {
class CSubComponentBase {
public:
    virtual ~CSubComponentBase();
    CBase*                    Owner;                 // +0x04
    const CGameCameraManager* CameraManager;         // +0x08
};
template <class TTarget, class TForce, class TCoords> class CPhysicsLeaf : public CSubComponentBase {
public:
    void* OurForce;                                  // +0x0c scoped_ptr<CPhysicsForceModelBase>
    void* OurCoords;                                 // +0x10 scoped_ptr<CPhysicsCoordModelBase>
    CPhysicsLeaf(CBase& owner, const CGameCameraManager& manager);   // 0x0085C950 (Rigid/Rigid instantiation)
};
typedef CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelRigid, CPhysicsCoordRigid> CRigidPhysicsLeaf;

class CBase {
public:
    virtual void v00();
    unsigned char       _pad04[0x40 - 0x04];
    CGameCameraManager* CameraManager;               // +0x40
    unsigned char       _pad44[0xa4 - 0x44];
};
class CFixedDolly : public CBase {
public:
    virtual CCountedPointer<CSubComponentBase> GetPhysicsMode();
};
CCountedPointer<CSubComponentBase> CFixedDolly::GetPhysicsMode()
{
    return CCountedPointer<CSubComponentBase>(new CRigidPhysicsLeaf(*this, *CameraManager));
}
}