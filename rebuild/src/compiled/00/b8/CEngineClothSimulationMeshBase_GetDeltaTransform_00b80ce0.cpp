// CEngineClothSimulationMeshBase::GetDeltaTransform(CMatrix3x4& dest) const -- the transform taking the cloth from its current frame to its last one: dest = inverse(CurrentTransform), then pre-multiplied by LastTransform
// Leaf: ?GetDeltaTransform@CEngineClothSimulationMeshBase@@UBEXAAVCMatrix3x4@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B80CE0, 682 bytes (`ret 4`; virtual, CParticleContext slot 3). Constant 1.0f 0x0122DED8.
// Naming: Ego_r.pdb / FableWin.pdb ?GetDeltaTransform@CEngineClothSimulationMeshBase@@UBEXAAVCMatrix3x4@@@Z; parameter `dest` from Ego_d.pdb.
// Source: Ego_d.pdb records the body at 43 debug bytes with no locals -- exactly the two inline calls SetInverse(CurrentTransform)
//   (Ego_d locals det, rec) and PreMultiply(LastTransform) (Ego_d local `m`, a CMatrix3x4 copy of *this: the shared
//   FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY nesting, PreMultiply = SetMultiply(matrix, copy)), both expanded in retail.
//   The direct PreMultiply text (FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY) left four E12/E13 sum-order hunks; the copy form matches.
// Header: new engine/CEngineClothSimulationMeshBase.h (Ego_d class record) over the e03 egod headers CEngineSimulationBase.h /
//   CParticleContext.h.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#include "engine/CMatrix3x4.h"
#include "engine/CEngineClothSimulationMeshBase.h"

void CEngineClothSimulationMeshBase::GetDeltaTransform(CMatrix3x4& dest) const
{
    dest.SetInverse(CurrentTransform);
    dest.PreMultiply(LastTransform);
}