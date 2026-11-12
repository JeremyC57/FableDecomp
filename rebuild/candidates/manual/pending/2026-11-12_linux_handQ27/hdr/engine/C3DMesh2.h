#ifndef FABLE_ENGINE_C3DMESH2_H
#define FABLE_ENGINE_C3DMESH2_H
#include "engine/C3DMeshMaterial.h"
#include "engine/C3DPrimitive2.h"
#include <stddef.h>
class C3DBone2;class C3DKeyframe;class CMatrix4x4;
// Complete donor244-byte member range, with native resource prefix8 bytes smaller: full size236.
#ifdef FABLE_RETAIL_C3DMESH2_LOADBINARY
// Retail opt-in (chunk Q27, row 0x00A8AD40 C3DMesh2::LoadBinary): the retail base C3DMeshStats (0x7c, engine/C3DMeshStats.h) in
// place of UnmodelledBase (same offsets: MemoryHandle +0x7c .. MeshID +0xe8), and the Ego_r.pdb / FableWin.pdb members
// ?LoadBinary@C3DMesh2@@QAE_NAAVCMemoryDataInputStream@@PAVCMovableResourceMemoryPool@@PBVCGraphicDataBank@@_N@Z (0x00A8AD40),
// private ?InitialisePointers@C3DMesh2@@AAEXKK@Z (0x00A896A0) and ?InitialiseBoneNamePointers@C3DMesh2@@AAEXK@Z (0x00A892E0).
#include "engine/C3DMeshStats.h"
class CMemoryDataInputStream;class CMovableResourceMemoryPool;class CGraphicDataBank;
class C3DMesh2 : public C3DMeshStats {
public:
    bool LoadBinary(CMemoryDataInputStream&, CMovableResourceMemoryPool*, const CGraphicDataBank*, bool);   // 0x00A8AD40
private:
    void InitialisePointers(unsigned long, unsigned long);              // 0x00A896A0
    void InitialiseBoneNamePointers(unsigned long);                     // 0x00A892E0
public:
#else
class C3DMesh2 {
public:
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
#ifndef FABLE_RETAIL_C3DMESH2_LOADBINARY
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
