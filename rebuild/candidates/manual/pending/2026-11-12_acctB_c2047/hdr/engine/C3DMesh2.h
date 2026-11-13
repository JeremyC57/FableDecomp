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
#ifdef FABLE_RETAIL_C3DMESH2_BONECOUNT
    // Retail opt-in (chunk 2044, rows 0x00AA0090 / 0x00A9F540): Ego_r.pdb / FableWin.pdb ?GetBoneCount@C3DMesh2@@QBEKXZ, inline in retail
    // (BoneCount +0x98 read in place, 0x00AA00A1). No layout change.
    unsigned long GetBoneCount() const { return BoneCount; }
#endif
#ifdef FABLE_RETAIL_C3DMESH2_STATS
    // Retail opt-in (chunk 1021, row 0x00BEC680 CEngineInternalPrimitiveMeshBase::CollectStats): FableWin.pdb ?GetMaterialCount@C3DMesh2@@QBEKXZ
    // (inline: MaterialCount +0x88 read in place, 0x00BEC709) and the out-of-line Ego_r.pdb ?GetVertexCount@C3DMesh2@@QBEKXZ /
    // ?GetTriangleCount@C3DMesh2@@QBEKXZ (retail 0x00A89320 / 0x00A89350). No layout change.
    unsigned long GetMaterialCount() const { return MaterialCount; }
    unsigned long GetVertexCount() const;                                // 0x00A89320
    unsigned long GetTriangleCount() const;                              // 0x00A89350
#endif
#ifdef FABLE_RETAIL_C3DMESH2_BONES
    // Retail opt-in (chunk 3001, rows 0x00AB2520 / 0x00A4D920): Ego_r.pdb / FableWin.pdb ?GetBone@C3DMesh2@@QBEPBVC3DBone2@@K@Z,
    // ?GetBone@C3DMesh2@@QAEPAVC3DBone2@@K@Z, ?GetBoneCount@C3DMesh2@@QBEKXZ (and, under FABLE_RETAIL_C3DMESH2_BONEKEYFRAME, ?GetBoneKeyframe@C3DMesh2@@QBEPBVC3DKeyframe@@K@Z);
    // no out-of-line copy is bound in retail (inline accessors). No layout change.
    const C3DBone2* GetBone(unsigned long index) const { return &Bones[index]; }
    C3DBone2* GetBone(unsigned long index) { return &Bones[index]; }
#ifndef FABLE_RETAIL_C3DMESH2_BONECOUNT
    unsigned long GetBoneCount() const { return BoneCount; }
#endif
#endif
#ifdef FABLE_RETAIL_C3DMESH2_BONEKEYFRAME
    // needs C3DKeyframe complete (engine/C3DKeyframe.h first)
    const C3DKeyframe* GetBoneKeyframe(unsigned long index) const { return &BoneKeyframes[index]; }
#endif
    unsigned char UnmodelledBase[0x7c];long MemoryHandle,PrimMemoryHandle,BoneNameMemoryHandle;
    unsigned long MaterialCount;C3DMeshMaterial* Materials;unsigned long PrimitiveCount;C3DPrimitive2* Primitives;
    unsigned long BoneCount;C3DBone2* Bones;C3DKeyframe* BoneKeyframes;unsigned short* BoneNameIndices;char* BoneNameData;unsigned long BoneNameDataSize;
    unsigned char InverseTransform[48];CMatrix4x4* BoneSpaceTransforms;bool ClothFlag,LockedFromMemoryRelocation;unsigned long MeshID;
};
FABLE_STATIC_ASSERT(sizeof(C3DMesh2)==0xec);
FABLE_STATIC_ASSERT(offsetof(C3DMesh2,Materials)==0x8c);
#endif
