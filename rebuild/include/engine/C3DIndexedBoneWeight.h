#ifndef FABLE_ENGINE_C3DINDEXEDBONEWEIGHT_H
#define FABLE_ENGINE_C3DINDEXEDBONEWEIGHT_H
// New header (chunk 725b): C3DIndexedBoneWeight, Ego_r sizeof 8: BoneIndex +0, Weight +4 (element of
// C3DMeshFileXAnimatedPrimitiveBlockChunk::BoneWeights).
#include "rebuild_abi.h"
class C3DIndexedBoneWeight {
public:
    unsigned long BoneIndex;   // +0x0
    unsigned long Weight;      // +0x4
};
FABLE_STATIC_ASSERT(sizeof(C3DIndexedBoneWeight) == 8);
#endif
