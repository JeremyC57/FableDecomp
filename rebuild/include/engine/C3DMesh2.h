#ifndef FABLE_ENGINE_C3DMESH2_H
#define FABLE_ENGINE_C3DMESH2_H
#include "engine/C3DMeshMaterial.h"
#include "engine/C3DPrimitive2.h"
#include <stddef.h>
class C3DBone2;class C3DKeyframe;class CMatrix4x4;
// Complete donor244-byte member range, with native resource prefix8 bytes smaller: full size236.
#ifdef FABLE_RETAIL_C3DMESH2_HELPERPOINTS
// Retail opt-in (wf3, row 0x00A89740 C3DMesh2::GetHelperPointPositionAnimated): the retail base C3DMeshStats (0x7c,
// engine/C3DMeshStats.h; needs FABLE_RETAIL_CMOVABLERESOURCE + FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS) in place of UnmodelledBase
// (same offsets: MemoryHandle +0x7c .. MeshID +0xe8), and the Ego_r.pdb / FableWin.pdb virtual overrides (UBE) of vtable
// 0x0129DEF0: slot 1 RelocateData 0x00A8B4C0, slots 4..9 the animated helper-point / dummy-object queries.
#include "engine/C3DMeshStats.h"
class C3DMesh2 : public C3DMeshStats {
public:
    virtual void RelocateData(long, void*, void*, long);                 // 0x00A8B4C0
    virtual C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint&,const C3DAnimationBlendState&,C3DAnimationManager&)const;   // 0x00A89740
    virtual C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&)const;   // 0x00A898D0
    virtual void GetDummyObjectPositionAnimated(const C3DDummyObject&,C3DVector&,const C3DAnimationBlendState&,C3DAnimationManager&,bool)const;   // 0x00A89A70
    virtual void GetDummyObjectPositionAnimated(const C3DDummyObject&,C3DVector&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&,bool)const;   // 0x00A89E40
    virtual void GetDummyObjectTransformAnimated(const C3DDummyObject&,CMatrix3x4&,const C3DAnimationBlendState&,C3DAnimationManager&,bool)const;   // 0x00A8A220
    virtual void GetDummyObjectTransformAnimated(const C3DDummyObject&,CMatrix3x4&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&,bool)const;   // 0x00A8A5F0
#else
class C3DMesh2 {
public:
#endif
#ifdef FABLE_RETAIL_C3DMESH2_BUILDMESH
    // Retail opt-in (wf22, row 0x00A24520 C3DMeshLODInfo::BuildMesh): Ego_r.pdb ??0C3DMesh2@@QAE@XZ (0x00A89190),
    // ?LoadBinary@C3DMesh2@@QAE_NAAVCMemoryDataInputStream@@PAVCMovableResourceMemoryPool@@PBVCGraphicDataBank@@_N@Z (0x00A8AD40),
    // ?CalculateTextureChangeFlags@C3DMesh2@@QAEXXZ (0x00A89380), ?LockPrimitive@C3DMesh2@@QAEPAVC3DPrimitive2@@K@Z (0x00A89250),
    // ?UnlockPrimitive@C3DMesh2@@QAEXK@Z (0x00A892A0), ?Sizeof@C3DMesh2@@QBEKXZ (0x00A893C0), all out of line. Declarations only.
    C3DMesh2();                                                          // 0x00A89190
    bool LoadBinary(class CMemoryDataInputStream&, class CMovableResourceMemoryPool*, const class CGraphicDataBank*, bool);   // 0x00A8AD40
    void CalculateTextureChangeFlags();                                  // 0x00A89380
    C3DPrimitive2* LockPrimitive(unsigned long);                         // 0x00A89250
    void UnlockPrimitive(unsigned long);                                 // 0x00A892A0
    unsigned long Sizeof() const;                                        // 0x00A893C0
#endif
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
#ifdef FABLE_RETAIL_C3DMESH2_BONES
    // Retail opt-in (chunk Q32, row 0x00BD2A90 CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings): Ego_r.pdb / FableWin.pdb
    // ?GetBoneCount@C3DMesh2@@QBEKXZ and ?GetBone@C3DMesh2@@QBEPBVC3DBone2@@K@Z, inline (BoneCount +0x98 / Bones +0x9C read in
    // place, 0x00BD2AC3 / 0x00BD2B10; C3DBone2 is 0x3C bytes in Ego_r).
    unsigned long GetBoneCount() const { return BoneCount; }
    const C3DBone2* GetBone(unsigned long index) const { return &Bones[index]; }
#endif
#ifndef FABLE_RETAIL_C3DMESH2_HELPERPOINTS
    unsigned char UnmodelledBase[0x7c];
#endif
    long MemoryHandle,PrimMemoryHandle,BoneNameMemoryHandle;
    unsigned long MaterialCount;C3DMeshMaterial* Materials;unsigned long PrimitiveCount;C3DPrimitive2* Primitives;
    unsigned long BoneCount;C3DBone2* Bones;C3DKeyframe* BoneKeyframes;unsigned short* BoneNameIndices;char* BoneNameData;unsigned long BoneNameDataSize;
    unsigned char InverseTransform[48];CMatrix4x4* BoneSpaceTransforms;bool ClothFlag,LockedFromMemoryRelocation;unsigned long MeshID;
};
FABLE_STATIC_ASSERT(sizeof(C3DMesh2)==0xec);
FABLE_STATIC_ASSERT(offsetof(C3DMesh2,Materials)==0x8c);
#endif
