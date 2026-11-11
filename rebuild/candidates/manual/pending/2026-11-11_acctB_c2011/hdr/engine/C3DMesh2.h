#ifndef FABLE_ENGINE_C3DMESH2_H
#define FABLE_ENGINE_C3DMESH2_H
#include "engine/C3DMeshMaterial.h"
#include "engine/C3DPrimitive2.h"
#include <stddef.h>
#ifdef FABLE_RETAIL_C3DMESH2_INVERSETRANSFORM
#include "engine/CMatrix3x4.h"
#endif
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
#ifdef FABLE_RETAIL_C3DMESH2_STATS
    // Retail opt-in (chunk 1021, row 0x00BEC680 CEngineInternalPrimitiveMeshBase::CollectStats): FableWin.pdb ?GetMaterialCount@C3DMesh2@@QBEKXZ
    // (inline: MaterialCount +0x88 read in place, 0x00BEC709) and the out-of-line Ego_r.pdb ?GetVertexCount@C3DMesh2@@QBEKXZ /
    // ?GetTriangleCount@C3DMesh2@@QBEKXZ (retail 0x00A89320 / 0x00A89350). No layout change.
    unsigned long GetMaterialCount() const { return MaterialCount; }
    unsigned long GetVertexCount() const;                                // 0x00A89320
    unsigned long GetTriangleCount() const;                              // 0x00A89350
#endif
    unsigned char UnmodelledBase[0x7c];long MemoryHandle,PrimMemoryHandle,BoneNameMemoryHandle;
    unsigned long MaterialCount;C3DMeshMaterial* Materials;unsigned long PrimitiveCount;C3DPrimitive2* Primitives;
    unsigned long BoneCount;C3DBone2* Bones;C3DKeyframe* BoneKeyframes;unsigned short* BoneNameIndices;char* BoneNameData;unsigned long BoneNameDataSize;
#ifdef FABLE_RETAIL_C3DMESH2_INVERSETRANSFORM
    // Overlay opt-in (chunk 2011, rows 0x00BF12A0 / 0x00BF18B0): FableWin.pdb ?PeekInverseTransform@C3DMesh2@@QBEABVCMatrix3x4@@XZ
    // (inline: InverseTransform +0xB0 read in place, 0x00BF1398 `fmul [eax+0B4h]`), the member typed as Ego_r.pdb's CMatrix3x4.
    const CMatrix3x4& PeekInverseTransform() const { return InverseTransform; }
    CMatrix3x4 InverseTransform;CMatrix4x4* BoneSpaceTransforms;
#else
    unsigned char InverseTransform[48];CMatrix4x4* BoneSpaceTransforms;
#endif
    bool ClothFlag,LockedFromMemoryRelocation;unsigned long MeshID;
};
FABLE_STATIC_ASSERT(sizeof(C3DMesh2)==0xec);
FABLE_STATIC_ASSERT(offsetof(C3DMesh2,Materials)==0x8c);
#endif
