// CParticleProgram::ClingToMesh(CParticleSimulation&, const C3DAnimationBlendState&, C3DAnimationManager&, const C3DVector*, const C3DMesh2&) const
// Grade: DIFFER(681v681) -- lane gap: x87 term order of the inlined CMatrix4x4::AlignedSetMultiply fallback only (as 0x00989230; retail column order 3,2,1,0 here) -- the SSE kernel, frame, loop, callees match
//   -- skin the initial positions with the animated bones (each bone matrix times the mesh bone-space transform) and blend the simulation
//   toward them by the per-particle alphas; retail 0x00AE1320, 681 bytes, `ret 14h`.
// Retail-asm: 0x00AE13D2 `mov edi,[esp+18h]` then the GF_SSE_MATRIX_MULTIPLY block (`movaps xmm0,[edx]`, `shufps`, `mulps`, `addps`) via AlignedSetMultiply; NOTES site 3
// Leaf: ?ClingToMesh@CParticleProgram@@ABEXAAVCParticleSimulation@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@PBVC3DVector@@ABVC3DMesh2@@@Z
// Flags: none
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLESIMULATION
#include "engine/CParticleProgram.h"
#include "engine/CMatrix4x4.h"
class C3DMesh2 {
public:
    unsigned char _pad00[0x98];
    unsigned long BoneCount;                        // +0x98
    unsigned char _pad9c[0xe0 - 0x9c];
    CMatrix4x4*   BoneSpaceTransforms;              // +0xe0
};
class C3DAnimationManager {
public:
    void CalcMeshTransformations(const C3DAnimationBlendState& state, const C3DVector* pos, const C3DMesh2& mesh, CMatrix4x4* matrices, unsigned long count, bool world);   // 0x00A9F540
};
namespace NVectorMath {
void __cdecl GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride, const C4DVector* to,
                                  unsigned long toStride, const float* weights, unsigned long weightStride, unsigned long count);   // 0x00A5BEF0
}
void CParticleProgram::ClingToMesh(CParticleSimulation& simulation, const C3DAnimationBlendState& blend_state, C3DAnimationManager& animation_manager,
                                   const C3DVector* position, const C3DMesh2& mesh) const
{
    unsigned long count = InitialSimulation.Size;
    C4DVector* skinned = GetVectorBuffer(count);
    unsigned long bone_count = mesh.BoneCount;
    CMatrix4x4* matrices = GetMatrixBuffer(bone_count);
    animation_manager.CalcMeshTransformations(blend_state, position, mesh, matrices, bone_count, true);
    for (unsigned long i = 0; i < bone_count; ++i) {
        const CMatrix4x4& inverse = mesh.BoneSpaceTransforms[i];
        __declspec(align(16)) CMatrix4x4 bone = matrices[i];
        matrices[i].AlignedSetMultiply(bone, inverse);
    }
    TransformVertices(skinned, InitialSimulation.Positions, count, matrices, PeekSimGroups());
    NVectorMath::GFInterpolateVectors(simulation.Positions, sizeof(C4DVector), skinned, sizeof(C4DVector), simulation.Positions, sizeof(C4DVector),
                                      simulation.SimulationAlphas, sizeof(float), count);
}
