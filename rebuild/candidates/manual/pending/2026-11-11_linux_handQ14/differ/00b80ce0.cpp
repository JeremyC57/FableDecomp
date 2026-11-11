// CEngineClothSimulationMeshBase::GetDeltaTransform(CMatrix3x4& delta) const -- the transform taking the cloth from its current frame to its last one: the last transform times the inverse of the current transform
// Grade: DIFFER(ours v retail) -- 682v682, same length: shared-header SetInverse(CurrentTransform)+PreMultiply(LastTransform) matches everything except the E12/E13 row sums of PreMultiply, where retail sums a.E12*l.E11, a.E22*l.E12, a.E32*l.E13 and the header gives E12, E32, E22 (candidate-id order); an E12/E13-only reorder fixes those rows but shifts the mismatch into E21+ (5 probes). The class itself is still a local model (no CEngineClothSimulationMeshBase.h)
// Leaf: ?GetDeltaTransform@CEngineClothSimulationMeshBase@@UBEXAAVCMatrix3x4@@@Z
// Earlier grade note: DIFFER(687v682) -- the inline 3x3 inverse (same family as the parked 0x00B393D0): VC7.1 canonicalises the fld/fmul operand of the cofactor products independent of source operand order (all-swapped source compiles identically), shares c0*inv in a frame word (sub esp,0x34 vs 0x30); the Multiply(inverse copy, LastTransform) tail is the matched 0x00BCEBE0 shape
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80CE0, length 683 bytes (`ret 4`, virtual
// ?GetDeltaTransform@CEngineClothSimulationMeshBase@@UBEXAAVCMatrix3x4@@@Z). Constant 1.0f 0x0122DED8.
// Body: CMatrix3x4::SetInverse (FableWin.pdb ?SetInverse@CMatrix3x4@@QAEXABV1@@Z) of the current transform, then
//   CMatrix3x4::PreMultiply(LastTransform) (FableWin.pdb ?PreMultiply@CMatrix3x4@@QAEXABV1@@Z): delta = last * inverse(current).
// Header debt: no engine/CEngineClothSimulationMeshBase.h yet (bases CEngineSimulationBase / CParticleContext have no headers), so
//   the class is still modelled locally below; the matrix code is the shared engine/CMatrix3x4.h.
// Layout: Ego_r.pdb CEngineClothSimulationMeshBase CurrentTransform +0x1c / LastTransform +0x4c; retail's ParticleSims
// CArray is 12 bytes (not the PDB's 16), so they sit at +0x18 / +0x48 -- modelled locally.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY
#include "engine/CMatrix3x4.h"
class CEngineClothSimulationMeshBase {
public:
    virtual void GetDeltaTransform(CMatrix3x4& delta) const;
    unsigned char _pad04[0x18 - 4];
    CMatrix3x4    CurrentTransform;                     // +0x18
    CMatrix3x4    LastTransform;                        // +0x48
};

void CEngineClothSimulationMeshBase::GetDeltaTransform(CMatrix3x4& delta) const
{
    delta.SetInverse(CurrentTransform);
    delta.PreMultiply(LastTransform);
}
