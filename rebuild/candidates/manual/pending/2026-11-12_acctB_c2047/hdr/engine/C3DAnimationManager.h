#ifndef FABLE_ENGINE_C3DANIMATIONMANAGER_H
#define FABLE_ENGINE_C3DANIMATIONMANAGER_H
// C3DAnimationManager (new class header, chunk 2001, row 0x00A9EB90 BlendAnimations): the retail class.
// RTTI .?AVC3DAnimationManager@@ : CBaseClassNonCopyable : CBaseClass; vtable 0x0129E134 (ctor 0x00AA0F60, landed: stores it and
// builds the two pools with CSimpleMemoryPool(0, 0x1000) 0x00A5C570). Members: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv:
// MeshBank +4, BoneIndicesBuffer, MemoryPool, MemoryPool2; sizeof 0x50) at the retail STLport offsets (CArray = 12-byte vector):
// BoneIndicesBuffer +0x08, MemoryPool +0x14 (BlendAnimations 0x00A9EBF8 `lea ecx,[esi+14h] ; call Allocate`), MemoryPool2 +0x30
// (CalcBlendedMeshTransformations 0x00AA09F0), sizeof 0x4C. Methods: the Ego_r.pdb spellings (class key V, private Blend /
// Combine helpers `AAE`). Only the members the chunk-2001 row uses are declared.
#ifndef FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CArray.h"
class CMeshDataBank;
class C3DKeyframe;
#ifdef FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
class C3DAnimationBlendState;
class C3DAnimationStateBase;
class C3DVector;
class C3DMesh2;
class CMatrix4x4;
#endif

class C3DAnimationManager : public CBaseClassNonCopyable {
public:
    C3DAnimationManager();                                               // 0x00AA0F60 ??0C3DAnimationManager@@QAE@XZ
    virtual ~C3DAnimationManager();                                      // 0x00AA4170
private:
    // Ego_r.pdb ?BlendAnimations@C3DAnimationManager@@AAEXPAVC3DKeyframe@@PBV2@KM@Z (private): blend `count` keyframes of `to`
    // into `from` (positions linearly, rotations by slerp) at weight t.
    void BlendAnimations(C3DKeyframe* from, const C3DKeyframe* to, unsigned long count, float t);   // 0x00A9EB90
#ifdef FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
    // Retail opt-in (chunk 2044, rows 0x00AA0090 / 0x00A9F540): the Ego_r.pdb spellings of the mesh-transform members and the private
    // helpers they call (all out of line in retail): public QAE
    //   ?CalcBlendedMeshTransformations@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@0MPBVC3DVector@@ABVC3DMesh2@@PAVCMatrix4x4@@K_N@Z (0x00AA0090)
    //   ?CalcMeshTransformations@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@PBVC3DVector@@ABVC3DMesh2@@PAVCMatrix4x4@@K_N@Z (0x00A9F540),
    // private const ABE ApplyBoneMorphDisplacement 0x00A9DFA0 / ApplyParentTransformations 0x00A9E1E0 / ApplyBoneMorphScaling 0x00A9D750, and
    // private AAE CombineBaseAnimations 0x00A9F1D0 / CombineBlendedBaseAnimations 0x00A9F2F0.
public:
    void CalcBlendedMeshTransformations(const C3DAnimationBlendState& start_blend, const C3DAnimationBlendState& end_blend, float blend,
                                        const C3DVector* bone_morph, const C3DMesh2& mesh, CMatrix4x4* matrices,
                                        unsigned long matrix_count, bool transpose);
    void CalcMeshTransformations(const C3DAnimationBlendState& blend_state, const C3DVector* bone_morph, const C3DMesh2& mesh,
                                 CMatrix4x4* matrices, unsigned long matrix_count, bool transpose);
private:
    void ApplyBoneMorphDisplacement(C3DKeyframe* keyframes, const C3DVector* bone_morph, const C3DMesh2& mesh) const;      // 0x00A9DFA0
    void ApplyParentTransformations(const C3DKeyframe* keyframes, CMatrix4x4* matrices, const C3DMesh2& mesh, bool transpose) const;   // 0x00A9E1E0
    void ApplyBoneMorphScaling(const C3DVector* bone_morph, CMatrix4x4* matrices, unsigned long count, bool transpose) const;   // 0x00A9D750
    void CombineBaseAnimations(const C3DAnimationStateBase& state, C3DKeyframe* keyframes, unsigned long count, unsigned long depth,
                               const C3DMesh2& mesh);                                                                     // 0x00A9F1D0
    void CombineBlendedBaseAnimations(const C3DAnimationStateBase& start_state, const C3DAnimationStateBase& end_state, float blend,
                                      C3DKeyframe* keyframes, unsigned long count, unsigned long depth, const C3DMesh2& mesh);   // 0x00A9F2F0
#endif
public:
    CMeshDataBank*          MeshBank;                                    // +0x04
    CArray<unsigned long>   BoneIndicesBuffer;                           // +0x08
    CSimpleMemoryPool       MemoryPool;                                  // +0x14
    CSimpleMemoryPool       MemoryPool2;                                 // +0x30
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationManager) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationManager, MeshBank) == 0x4);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationManager, BoneIndicesBuffer) == 0x8);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationManager, MemoryPool) == 0x14);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationManager, MemoryPool2) == 0x30);

#endif // FABLE_ENGINE_C3DANIMATIONMANAGER_H
