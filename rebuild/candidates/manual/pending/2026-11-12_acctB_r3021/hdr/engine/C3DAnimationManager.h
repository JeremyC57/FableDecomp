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

class C3DAnimationManager : public CBaseClassNonCopyable {
public:
    C3DAnimationManager();                                               // 0x00AA0F60 ??0C3DAnimationManager@@QAE@XZ
    virtual ~C3DAnimationManager();                                      // 0x00AA4170
private:
    // Ego_r.pdb ?BlendAnimations@C3DAnimationManager@@AAEXPAVC3DKeyframe@@PBV2@KM@Z (private): blend `count` keyframes of `to`
    // into `from` (positions linearly, rotations by slerp) at weight t.
    void BlendAnimations(C3DKeyframe* from, const C3DKeyframe* to, unsigned long count, float t);   // 0x00A9EB90
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
