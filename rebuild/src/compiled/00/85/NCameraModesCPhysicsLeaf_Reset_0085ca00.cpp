// NCameraModes::CPhysicsLeaf::Reset() -- reset the leaf's force model (the CPhysicsForceModelRigid / CPhysicsCoordRigid instance)
// Leaf: ?Reset@?$CPhysicsLeaf@VCPhysicsTargetModelGoTo@NCameraModes@@VCPhysicsForceModelRigid@2@VCPhysicsCoordRigid@2@@NCameraModes@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085CA00, 8 bytes: `mov ecx,[ecx+0Ch] ; mov eax,[ecx] ; jmp dword ptr [eax+4]`
//   (OurForce->Reset(), a virtual tail call through the scoped_ptr at +0x0C).
// Instantiation: CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelRigid, CPhysicsCoordRigid>.
// Identity (two sources): (1) slot 1 of the instance's vtable 0x01276AEC (RTTI full name demangled: the template arguments above;
//   the catalog's "vfunc_1"), CSubComponentBase's Reset slot (slot 0 ??_E, slot 2 the shared CPhysicsTargetModelGoTo GetType 0x0084FD10);
//   (2) Ego_r.pdb / FableWin.pdb ?Reset@?$CPhysicsLeaf@..@@UAEXXZ for this instance, and the body forwards to the Ego_r member OurForce
//   (scoped_ptr<CPhysicsForceModelBase>, +0x0C) through CPhysicsForceModelBase's slot 1, ?Reset@CPhysicsForceModelBase@NCameraModes@@UAEXXZ.
//   Retail keeps one copy per instance (0x00850630 and 0x0085CA00 are two template instances, not duplicates).
// Header: engine/NCameraModes_CPhysicsLeaf.h (new), boost/scoped_ptr.h (FABLE_RETAIL_SCOPED_PTR).
#include "engine/NCameraModes_CPhysicsLeaf.h"
namespace NCameraModes {
template <class TTarget, class TForce, class TCoord>
void CPhysicsLeaf<TTarget, TForce, TCoord>::Reset()
{
    OurForce->Reset();
}
template void CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelRigid, CPhysicsCoordRigid>::Reset();
}