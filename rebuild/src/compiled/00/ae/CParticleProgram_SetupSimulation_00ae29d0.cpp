// CParticleProgram::SetupSimulation(CParticleSimulation&, const C3DAnimationBlendState&, C3DAnimationManager&, const C3DVector*, const C3DMesh2&,
//   CMovableResourceMemoryPool&, const CParticleContext&) const -- copy the initial simulation, skin it to the animated mesh, run the
//   constraints once and seed LastPositions
// Grade: RELOCATION_MATCH
// Leaf: ?SetupSimulation@CParticleProgram@@QBE_NAAVCParticleSimulation@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@PBVC3DVector@@ABVC3DMesh2@@AAVCMovableResourceMemoryPool@@ABVCParticleContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE29D0, 726 bytes, `ret 1Ch`.
// Locals: the Ego_d.pdb record of this overload (S_GPROC32, debug length 280, carved at Ego_d.pdb offset 0x7FCD2BC; the 2-arg overload,
//   length 45, is the record before it): params dest (CParticleSimulation&), anim, anim_manager, bone_morphs, mesh, alloc
//   (CMovableResourceMemoryPool&), context (const CParticleContext&); locals no_bones, bone_transforms; loop scope c_bone -- exactly this
//   text (no copy local: the bone product is the inline AlignedPreMultiplyTransposed, as in twin 0x00AE1320). The constraint arguments are
//   CParticleContext slots 4 / 5 by their Ego_d names (engine/CParticleContext.h: GetMaximumIterations, GetMaximumDistanceError).
// Retail-asm: 0x00AE2A96 `movaps xmm0,[edx]` + `shufps xmm4,xmm4,00h` + `addps xmm4,xmm6` (GF_SSE_MATRIX_MULTIPLY via AlignedSetMultiply).
// Inline-budget: FABLE_FORCEINLINE on AlignedPreMultiplyTransposed and AlignedSetMultiplyTransposed (as 0x00AE1320).
// Flags: none
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_SIMD_ORIGINAL_FLAG
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED
#define FABLE_RETAIL_CMATRIX4X4_SETMULTIPLYTRANSPOSED_FORCEINLINE
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPREMULTIPLYTRANSPOSED_FORCEINLINE
#define FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
#define FABLE_RETAIL_C3DMESH2_BONECOUNT
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLESIMULATION
#include "engine/CParticleProgram.h"
#include "engine/CMatrix4x4.h"
#include <string.h>
#include "engine/C3DMesh2.h"
#include "engine/C3DAnimationManager.h"
#include "engine/CParticleContext.h"
bool CParticleProgram::SetupSimulation(CParticleSimulation& dest, const C3DAnimationBlendState& anim, C3DAnimationManager& anim_manager,
                                       const C3DVector* bone_morphs, const C3DMesh2& mesh, CMovableResourceMemoryPool& alloc, const CParticleContext& context) const
{
    if (!dest.CopyFrom(InitialSimulation, alloc))
        return false;
    unsigned long no_bones = mesh.GetBoneCount();
    CMatrix4x4* bone_transforms = GetMatrixBuffer(no_bones);
    anim_manager.CalcMeshTransformations(anim, bone_morphs, mesh, bone_transforms, no_bones, true);
    for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)
        bone_transforms[c_bone].AlignedPreMultiplyTransposed(mesh.BoneSpaceTransforms[c_bone]);
    TransformVertices(dest.Positions, InitialSimulation.Positions, dest.Size, bone_transforms, PeekSimGroups());
    ApplyConstraints(dest, context.GetMaximumIterations(), context.GetMaximumDistanceError());
    memcpy(dest.LastPositions, dest.Positions, dest.Size * sizeof(C4DVector));
    return true;
}