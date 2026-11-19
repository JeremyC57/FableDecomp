// CParticleProgram::ClingToMesh(CParticleSimulation&, const C3DAnimationBlendState&, C3DAnimationManager&, const C3DVector*, const C3DMesh2&) const
//   -- skin the initial positions with the animated bones (each bone matrix times the mesh bone-space transform) and blend the simulation
//   toward them by the per-particle alphas
// Grade: RELOCATION_MATCH
// Leaf: ?ClingToMesh@CParticleProgram@@ABEXAAVCParticleSimulation@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@PBVC3DVector@@ABVC3DMesh2@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE1320, 681 bytes, `ret 14h`.
// Name: FableWin.pdb / Ego_r.pdb (5-arg private const overload); hand748 NAME_FIXES.
// Locals: the Ego_d.pdb record (debug length 234): params sim, anim, anim_manager, bone_morphs, mesh; locals no_bones, mesh_vertices,
//   no_vertices, bone_transforms; loop scope c_bone -- no other local, so the bone product is the inline
//   CMatrix4x4::AlignedPreMultiplyTransposed (its aligned copy `m` lives in the inlinee; Ego_d.pdb has that member as a proc, retail an
//   out-of-line copy at 0x00ADEFC0) expanding AlignedSetMultiplyTransposed -> AlignedSetMultiply (Ego_d body,
//   FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD). The bone count is read through the inline C3DMesh2::GetBoneCount (same bytes as
//   mesh.BoneCount; descriptive choice).
// Retail-asm: 0x00AE13D2 `mov edi,[esp+18h]` then the GF_SSE_MATRIX_MULTIPLY block (`movaps xmm0,[edx]`, `shufps`, `mulps`, `addps`) of
//   engine/CMatrix4x4AlignedSSE.h via AlignedSetMultiply.
// Inline-budget: FABLE_FORCEINLINE on AlignedPreMultiplyTransposed and AlignedSetMultiplyTransposed (the existing nested opt-ins of
//   landed 0x00BD2D90 / 0x00A9E1E0; plain inline leaves the call to 0x00ADEFC0 in place, 252 bytes).
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
#include "engine/C3DMesh2.h"
#include "engine/C3DAnimationManager.h"
namespace NVectorMath {
void __cdecl GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride, const C4DVector* to,
                                  unsigned long toStride, const float* weights, unsigned long weightStride, unsigned long count);   // 0x00A5BEF0
}
void CParticleProgram::ClingToMesh(CParticleSimulation& sim, const C3DAnimationBlendState& anim, C3DAnimationManager& anim_manager,
                                   const C3DVector* bone_morphs, const C3DMesh2& mesh) const
{
    unsigned long no_vertices = InitialSimulation.Size;
    C4DVector* mesh_vertices = GetVectorBuffer(no_vertices);
    unsigned long no_bones = mesh.GetBoneCount();
    CMatrix4x4* bone_transforms = GetMatrixBuffer(no_bones);
    anim_manager.CalcMeshTransformations(anim, bone_morphs, mesh, bone_transforms, no_bones, true);
    for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)
        bone_transforms[c_bone].AlignedPreMultiplyTransposed(mesh.BoneSpaceTransforms[c_bone]);
    TransformVertices(mesh_vertices, InitialSimulation.Positions, no_vertices, bone_transforms, PeekSimGroups());
    NVectorMath::GFInterpolateVectors(sim.Positions, sizeof(C4DVector), mesh_vertices, sizeof(C4DVector), sim.Positions, sizeof(C4DVector),
                                      sim.SimulationAlphas, sizeof(float), no_vertices);
}
