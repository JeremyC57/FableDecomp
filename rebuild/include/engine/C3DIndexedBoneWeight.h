#ifndef FABLE_ENGINE_C3DINDEXEDBONEWEIGHT_H
#define FABLE_ENGINE_C3DINDEXEDBONEWEIGHT_H
// New header (chunk 725b): C3DIndexedBoneWeight, Ego_r sizeof 8: BoneIndex +0, Weight +4 (element of
// C3DMeshFileXAnimatedPrimitiveBlockChunk::BoneWeights).
#include "rebuild_abi.h"
class C3DIndexedBoneWeight {
public:
#ifdef FABLE_RETAIL_C3DINDEXEDBONEWEIGHT_CTOR_PASS27
// 00A90AE0: FableWin emits the empty default constructor in resize(BoneWeights).
    C3DIndexedBoneWeight() {}
#endif
    unsigned long BoneIndex;   // +0x0
    unsigned long Weight;      // +0x4
};
FABLE_STATIC_ASSERT_AT(11, sizeof(C3DIndexedBoneWeight) == 8);
#endif
