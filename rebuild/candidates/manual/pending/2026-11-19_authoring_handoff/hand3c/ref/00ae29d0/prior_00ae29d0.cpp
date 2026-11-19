// CParticleProgram::SetupSimulation(CParticleSimulation&, const C3DAnimationBlendState&, C3DAnimationManager&, const C3DVector*, const C3DMesh2&,
// Grade: DIFFER(726v726) -- lane gap: x87 term order of the inlined CMatrix4x4::AlignedSetMultiply fallback only (as 0x00989230; retail column order 3,1,2,0) -- the SSE kernel, CopyFrom / CalcMeshTransformations / TransformVertices / ApplyConstraints / memcpy sequence match
//   CMovableResourceMemoryPool&, const CParticleContext&) const -- copy the initial simulation, skin it to the animated mesh, run the
//   constraints once and seed LastPositions; retail 0x00AE29D0, 726 bytes, `ret 1Ch`.
// Retail-asm: 0x00AE2A96 `movaps xmm0,[edx]` + `shufps xmm4,xmm4,00h` + `addps xmm4,xmm6` (GF_SSE_MATRIX_MULTIPLY via AlignedSetMultiply in the bone loop; NOTES site 3)
// Leaf: ?SetupSimulation@CParticleProgram@@QBE_NAAVCParticleSimulation@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@PBVC3DVector@@ABVC3DMesh2@@AAVCMovableResourceMemoryPool@@ABVCParticleContext@@@Z
// Flags: none
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLESIMULATION
#include "engine/CParticleProgram.h"
#include "engine/CMatrix4x4.h"
#include <string.h>
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
class CParticleContext {
public:
    virtual ~CParticleContext();
    virtual void Slot1() const;
    virtual void Slot2() const;
    virtual void Slot3() const;
    virtual long GetStepCount() const;      // slot 4 (+0x10)
    virtual float GetTimeDelta() const;     // slot 5 (+0x14)
};
bool CParticleProgram::SetupSimulation(CParticleSimulation& simulation, const C3DAnimationBlendState& blend_state, C3DAnimationManager& animation_manager,
                                       const C3DVector* position, const C3DMesh2& mesh, CMovableResourceMemoryPool& pool, const CParticleContext& context) const
{
    if (!simulation.CopyFrom(InitialSimulation, pool))
        return false;
    unsigned long bone_count = mesh.BoneCount;
    CMatrix4x4* matrices = GetMatrixBuffer(bone_count);
    animation_manager.CalcMeshTransformations(blend_state, position, mesh, matrices, bone_count, true);
    for (unsigned long i = 0; i < bone_count; ++i) {
        const CMatrix4x4& inverse = mesh.BoneSpaceTransforms[i];
        __declspec(align(16)) CMatrix4x4 bone = matrices[i];
        matrices[i].AlignedSetMultiply(bone, inverse);
    }
    TransformVertices(simulation.Positions, InitialSimulation.Positions, simulation.Size, matrices, PeekSimGroups());
    ApplyConstraints(simulation, context.GetStepCount(), context.GetTimeDelta());
    memcpy(simulation.LastPositions, simulation.Positions, simulation.Size * sizeof(C4DVector));
    return true;
}
