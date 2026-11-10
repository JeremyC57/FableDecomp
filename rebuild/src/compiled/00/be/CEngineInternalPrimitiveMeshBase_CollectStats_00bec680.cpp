// CEngineInternalPrimitiveMeshBase::CollectStats(const C3DMeshInfo& mesh_info, std::map<unsigned long, unsigned long>& stats, EEnginePrimitiveStatIndex stat_index, unsigned long) -- static: fetch the mesh's LOD 0 and record one statistic in the map: its vertex or triangle count keyed by the mesh info, or each material texture's bank data size keyed by its texture bank index
// Leaf: ?CollectStats@CEngineInternalPrimitiveMeshBase@@SIXABVC3DMeshInfo@@AAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@W4EEnginePrimitiveStatIndex@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEC680, 545 bytes (+15 int3 pad), static `__fastcall` (mesh_info in ecx, stats in edx,
//   stat_index and the unused flags word on the stack, `ret 8`).
// Name: Ego_r.pdb (0x00AA4A90) the static overload; the virtual UBEX overload (Ego_r 0x00AA4CB0 = retail 0x00BEC8B0, landed as a
//   forwarder) passes its MeshInfo, and CEngineInternalPrimitiveRepeatedStaticMesh::CollectStats 0x00B98DE0 calls it too. Both
//   landed callers bind invented spellings (NAME_FIXES.tsv).
// Callees: C3DMeshInfo::GetMesh(CCountedPointer<const C3DMesh2>&, unsigned long, unsigned long) const 0x00A25E70 (lod 0, flags 4),
//   map<unsigned long,unsigned long>::operator[] 0x00553E60 (the STLport body, visible and out of line), C3DMesh2::GetVertexCount
//   0x00A89320 / GetTriangleCount 0x00A89350, CBankFile::GetEntryDataSize 0x009CC240 (on the mesh bank's texture bank), the
//   ~CCountedPointer<const C3DMesh2> COMDAT 0x00A36EC0 (called on the default exit, expanded on the others), operator delete 0x00BFE9BC.
// Enumerators: ENGINE_PRIM_STAT_VERTICES / _TRIANGLES / _TEXTURES are the descriptive names engine/CEnginePersistentPrimitiveBase.h
//   already gives the PDB enum (its values are not in either PDB).
// Headers (overlay opt-ins): FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMESHBASE_COLLECTSTATS, FABLE_RETAIL_C3DMESHINFO_GETMESH,
//   FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK, FABLE_RETAIL_C3DMESH2_STATS, FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES,
//   FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE, FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE (hdr/README.md).
// Shape: FABLE_RETAIL_STLP_MAP gives c2 operator[]'s body (still called): the key is then known to be only read, so one key
//   variable shared by every lookup is homed in the dead stat_index argument slot together with the operator[] result spill
//   (`mov [esp+30h],edi ; lea edx,[esp+30h]`, 0x00BEC744 / 0x00BEC753), and the vertex / triangle keys are stored before the
//   count calls (0x00BEC843 / 0x00BEC859). The material loop walks a cached `const C3DMesh2*` (its &MaterialCount / &Materials
//   spilled, 0x00BEC717 / 0x00BEC72A).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMESHBASE_COLLECTSTATS
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESH2_STATS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankFile.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineInternalPrimitiveMeshBase.h"

void FABLE_FASTCALL CEngineInternalPrimitiveMeshBase::CollectStats(const C3DMeshInfo& mesh_info, std::map<unsigned long, unsigned long>& stats,
                                                                   EEnginePrimitiveStatIndex stat_index, unsigned long)
{
    CCountedPointer<const C3DMesh2> mesh;                // LOD 0 (flags 4)
    if (!mesh_info.GetMesh(mesh, 0, 4))
        return;
    unsigned long key;
    switch (stat_index) {
    case ENGINE_PRIM_STAT_VERTICES: {
        key = (unsigned long)&mesh_info;
        unsigned long vertex_count = mesh->GetVertexCount();
        stats[key] = vertex_count;
        break;
    }
    case ENGINE_PRIM_STAT_TRIANGLES: {
        key = (unsigned long)&mesh_info;
        unsigned long triangle_count = mesh->GetTriangleCount();
        stats[key] = triangle_count;
        break;
    }
    case ENGINE_PRIM_STAT_TEXTURES: {
        const C3DMesh2* p_mesh = mesh.Get();
        for (unsigned long i = 0; i < p_mesh->GetMaterialCount(); ++i) {
            const C3DMeshMaterial* material = p_mesh->GetMaterial(i);
            key = material->GetDiffuseMapBankIndex();
            if (key)
                stats[key] = mesh_info.PeekTextureBank()->GetEntryDataSize(key);
            key = material->GetBumpMapBankIndex();
            if (key)
                stats[key] = mesh_info.PeekTextureBank()->GetEntryDataSize(key);
            key = material->GetReflectionMapBankIndex();
            if (key)
                stats[key] = mesh_info.PeekTextureBank()->GetEntryDataSize(key);
            key = material->GetIlluminationMapBankIndex();
            if (key)
                stats[key] = mesh_info.PeekTextureBank()->GetEntryDataSize(key);
        }
        break;
    }
    default:
        return;
    }
}