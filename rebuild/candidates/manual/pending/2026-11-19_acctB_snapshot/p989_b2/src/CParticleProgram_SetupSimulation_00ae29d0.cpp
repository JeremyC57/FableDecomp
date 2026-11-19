// CParticleProgram::SetupSimulation(CParticleSimulation&, const C3DAnimationBlendState&, C3DAnimationManager&, const C3DVector*, const C3DMesh2&,
//   CMovableResourceMemoryPool&, const CParticleContext&) const -- copy the initial simulation, skin it to the animated mesh, run the
//   constraints once and seed LastPositions
// Grade: RELOCATION_MATCH
// Leaf: ?SetupSimulation@CParticleProgram@@QBE_NAAVCParticleSimulation@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@PBVC3DVector@@ABVC3DMesh2@@AAVCMovableResourceMemoryPool@@ABVCParticleContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE29D0, 726 bytes, `ret 1Ch`.
// Locals: the Ego_d.pdb record of this overload is cut short (only the 2-arg SetupSimulation, `dest` / `alloc`, survives complete);
//   the text follows its twin ClingToMesh 0x00AE1320's Ego_d record (no_bones, bone_transforms, c_bone; the bone product through the
//   inline AlignedPreMultiplyTransposed, no copy local) -- Inferred (twin record); `sim` / `alloc` / `anim` / `anim_manager` /
//   `bone_morphs` as the 1320 and 2-arg records. The constraint arguments are CParticleContext slots 4 / 5 by their Ego_d names
//   (engine/CParticleContext.h: GetMaximumIterations, GetMaximumDistanceError).
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
bool CParticleProgram::SetupSimulation(CParticleSimulation& sim, const C3DAnimationBlendState& anim, C3DAnimationManager& anim_manager,
                                       const C3DVector* bone_morphs, const C3DMesh2& mesh, CMovableResourceMemoryPool& alloc, const CParticleContext& context) const
{
    if (!sim.CopyFrom(InitialSimulation, alloc))
        return false;
    unsigned long no_bones = mesh.GetBoneCount();
    CMatrix4x4* bone_transforms = GetMatrixBuffer(no_bones);
    anim_manager.CalcMeshTransformations(anim, bone_morphs, mesh, bone_transforms, no_bones, true);
    for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)
        bone_transforms[c_bone].AlignedPreMultiplyTransposed(mesh.BoneSpaceTransforms[c_bone]);
    TransformVertices(sim.Positions, InitialSimulation.Positions, sim.Size, bone_transforms, PeekSimGroups());
    ApplyConstraints(sim, context.GetMaximumIterations(), context.GetMaximumDistanceError());
    memcpy(sim.LastPositions, sim.Positions, sim.Size * sizeof(C4DVector));
    return true;
}
