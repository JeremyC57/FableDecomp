#ifndef FABLE_ENGINE_C3DMESH2_H
#define FABLE_ENGINE_C3DMESH2_H
#include "engine/C3DMeshMaterial.h"
#include "engine/C3DPrimitive2.h"
#include <stddef.h>
class C3DBone2;class C3DKeyframe;class CMatrix4x4;
// Complete donor244-byte member range, with native resource prefix8 bytes smaller: full size236.
class C3DMesh2 {
public:
#ifdef FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
    const C3DMeshMaterial* GetMaterial(unsigned long index) const { return &Materials[index]; }
    C3DMeshMaterial* GetMaterial(unsigned long index) { return &Materials[index]; }
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONINFO_BUILD
// Retail opt-in (Codex pass19): original Ego_r/FableWin types and donor calls; native animation rows 00a24d70/00a26ad0. See NAMING_EVIDENCE.md.
    unsigned long GetMeshID()const{return MeshID;}
#endif
    unsigned char UnmodelledBase[0x7c];long MemoryHandle,PrimMemoryHandle,BoneNameMemoryHandle;
    unsigned long MaterialCount;C3DMeshMaterial* Materials;unsigned long PrimitiveCount;C3DPrimitive2* Primitives;
    unsigned long BoneCount;C3DBone2* Bones;C3DKeyframe* BoneKeyframes;unsigned short* BoneNameIndices;char* BoneNameData;unsigned long BoneNameDataSize;
    unsigned char InverseTransform[48];CMatrix4x4* BoneSpaceTransforms;bool ClothFlag,LockedFromMemoryRelocation;unsigned long MeshID;
};
FABLE_STATIC_ASSERT(sizeof(C3DMesh2)==0xec);
FABLE_STATIC_ASSERT(offsetof(C3DMesh2,Materials)==0x8c);
#endif
