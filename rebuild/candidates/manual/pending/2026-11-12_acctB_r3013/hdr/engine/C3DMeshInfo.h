#ifndef FABLE_ENGINE_C3DMESHINFO_H
#define FABLE_ENGINE_C3DMESHINFO_H
#ifdef FABLE_RETAIL_CMESHDATABANKENTRY
// Retail opt-in (chunk 749, row 0x005502E0 NUISystem::CMesh::Draw): RTTI .?AVC3DMeshInfo@@ : CMeshDataBankEntry (rtti_bases.tsv);
// the members after the base (Ego_r.pdb +0xC..+0x60) stay opaque here.
#include "rebuild_abi.h"
#include "engine/CMeshDataBankEntry.h"
#ifdef FABLE_RETAIL_C3DMESHINFO_GETMESHSTATS
#include "engine/CCountedPointer.h"
class C3DMeshStats;
#endif
#ifdef FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#include "engine/C3DBoundingBox.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CIVCountedPointer.h"
#endif
#ifdef FABLE_RETAIL_C3DMESHINFO_LODERROR
enum EBankLoadPriority;                                                 // MS forward enum (no enumerator name recovered)
#endif
#ifdef FABLE_RETAIL_C3DMESHINFO_LODS
class C3DMeshLODInfo;
class C3DAnimationManager;
#endif
#ifdef FABLE_RETAIL_C3DMESHINFO_GETMESH
#include "engine/CCountedPointer.h"
class C3DMesh2;
class CGraphicDataBank;
#endif
class C3DMeshInfo : public CMeshDataBankEntry {
public:
#ifdef FABLE_RETAIL_C3DMESHINFO_GETMESHSTATS
    CCountedPointer<const C3DMeshStats> GetMeshStats()const;
#endif
#ifdef FABLE_RETAIL_C3DMESHINFO_GETMESH
    // Retail opt-in (chunk 1021, row 0x00BEC680 CEngineInternalPrimitiveMeshBase::CollectStats): Ego_r.pdb
    // ?GetMesh@C3DMeshInfo@@QBE_NAAV?$CCountedPointer@$$CBVC3DMesh2@@@@KK@Z (0x00B62470 = retail 0x00A25E70, out of line) and the
    // FableWin.pdb inline ?PeekTextureBank@C3DMeshInfo@@QBEPBVCGraphicDataBank@@XZ (0x02E77A90; the caller reads Bank +0x04, then
    // the bank's TextureBank, in place: 0x00BEC74A). Needs FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK. No layout change.
    bool GetMesh(CCountedPointer<const C3DMesh2>& mesh, unsigned long lod, unsigned long flags) const;   // 0x00A25E70
    const CGraphicDataBank* PeekTextureBank() const { return PeekBank()->GetTextureBank(); }
#endif
#ifdef FABLE_RETAIL_C3DMESHINFO_LODS
    // Overlay opt-in (chunk 2011, rows 0x00BF12A0 / 0x00BF18B0 CEnginePrimitiveRenderUtil::AddGeneratedObjects): Ego_r.pdb /
    // FableWin.pdb ?IsLoaded@C3DMeshInfo@@QBE_NXZ (out of line 0x00A23E40), ?BeginLoading@C3DMeshInfo@@QBEXK_N0@Z (0x00A256F0) and
    // the FableWin.pdb inlines ?HasLOD@C3DMeshInfo@@QBE_NK@Z / ?GetBestCurrentDetailLevel@C3DMeshInfo@@QBEKXZ (no Ego_r.pdb or
    // Fable.exe copy; 0x00BF131B..0x00BF1361 expand the walk: MeshData null test, then for each LOD below 4 `LODDataSizes[i] == 0`
    // -> none, a loaded LODMeshes[i] -> i; -1 = none, `jae` = unsigned; the bound test sits in HasLOD: with `lod < 4` in the loop
    // condition VC7.1 threads the first test and rotates the loop, retail keeps the top-tested head 0x00BF1340) and ?GetAnimationManager@C3DMeshInfo@@QBEAAVC3DAnimationManager@@XZ
    // (inline: the bank's AnimationManager +0x3C0 read in place, 0x00BF192D..0x00BF1933). Needs FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
    // (the typed private members). No layout change.
    bool IsLoaded() const;                                                       // 0x00A23E40
    void BeginLoading(unsigned long lod, bool high_priority, bool block) const;  // 0x00A256F0
    bool HasLOD(unsigned long lod) const { return lod < 4 && LODDataSizes[lod] != 0; }
#ifdef FABLE_RETAIL_C3DMESHINFO_LODERROR
    // Retail opt-in (chunk 2028, row 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive): FableWin.pdb inline
    // ?GetLODError@C3DMeshInfo@@QBEMK@Z (no Ego_r.pdb or Fable.exe copy; LODErrors +0x40 read in place, 0x00B9A64A) and Ego_r.pdb
    // ?BeginLoadingTextures@C3DMeshInfo@@QBEXW4EBankLoadPriority@@@Z (donor 0x00B5FEA0 between HasPhysicsMesh / AreTexturesLoaded =
    // retail 0x00A23C50 between 0x00A23C40 / 0x00A23C60: an empty `ret 4` in the PC build).
    float GetLODError(unsigned long lod) const { return LODErrors[lod]; }
    void BeginLoadingTextures(EBankLoadPriority priority) const;                // 0x00A23C50
#endif
    unsigned long GetBestCurrentDetailLevel() const {
        if (!MeshData.IsNull()) {
            for (unsigned long lod = 0; HasLOD(lod); ++lod) {
                if (!MeshData->LODMeshes[lod].IsNull())
                    return lod;
            }
        }
        return 0xffffffff;
    }
#ifdef FABLE_RETAIL_C3DMESHINFO_ANIMATIONMANAGER
    C3DAnimationManager& GetAnimationManager() const { return *PeekBank()->AnimationManager_Data; }
#endif
#endif
#ifdef FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
    C3DBoundingBox GetBoundingBox()const{return BoundingBox;}
#ifdef FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
    // Retail opt-in (Codex pass18): FableWin GetBoundingSphere returns original stored sphere by value.
    C3DBoundingSphere GetBoundingSphere()const{return BoundingSphere;}
#endif
private:
#ifdef FABLE_RETAIL_C3DMESHINFO_LODS
    // Ego_r.pdb C3DMeshInfo::CMeshData (32 bytes; FableWin.pdb ??0CMeshData@C3DMeshInfo@@QAE@XZ): a counted pointee holding the four
    // LOD resources, the stats resource and the mesh id (GetMeshStats 0x00A25543 reads MeshStats +0x18).
    class CMeshData : public CIVCountedPointeeBase {
    public:
        CIVCountedPointer<C3DMeshLODInfo> LODMeshes[4];                      // +0x08
        CIVCountedPointer<C3DMeshLODInfo> MeshStats;                         // +0x18
        unsigned long MeshID;                                                // +0x1c
    };
#else
    class CMeshData;
#endif
    CIVCountedPointer<CMeshData> MeshData;
    C3DBoundingBox BoundingBox;
    C3DBoundingSphere BoundingSphere;
    float SafeBoundingRadius;
    long PhysicsMeshIndex;
    float LODErrors[4];
    unsigned long LODDataSizes[4];
#else
    unsigned char _body_0x0c[0x60 - 0xc];                               // +0x0c MeshData, BoundingBox, BoundingSphere, ...
#endif
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshInfo) == 0x60);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof C3DMeshInfo = 0x60.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CIVCountedPointeeBase;

#pragma pack(push, 1)
class C3DMeshInfo {
public:
    void*                  __vftable;                        // +0x00 vptr, or first dword of a flattened base subobject
    long                   f4;                               // +0x04 retail-only long (previous header)
    unsigned char          _pad_0x08[0x4];                   // +0x08
    CIVCountedPointeeBase* MeshData_Object;                  // +0x0c CIVCountedPointer<C3DMeshInfo::CMeshData>::Object
    unsigned char          BoundingBox[0x18];                // +0x10 C3DBoundingBox
    unsigned char          BoundingSphere[0x10];             // +0x28 C3DBoundingSphere
    float                  SafeBoundingRadius;               // +0x38
    long                   PhysicsMeshIndex;                 // +0x3c
    float                  LODErrors[4];                     // +0x40
    unsigned long          LODDataSizes[4];                  // +0x50
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(C3DMeshInfo) == 0x60);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, f4) == 0x4);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, MeshData_Object) == 0xc);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, BoundingBox) == 0x10);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, BoundingSphere) == 0x28);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, SafeBoundingRadius) == 0x38);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, PhysicsMeshIndex) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, LODErrors) == 0x40);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, LODDataSizes) == 0x50);

#endif
#endif // FABLE_ENGINE_C3DMESHINFO_H
