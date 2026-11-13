// CEnginePrimitiveRenderUtil::AddGeneratedObjects(const CEngineInternalPrimitiveStaticMesh& primitive) -- add one static mesh for every mesh generator of a loaded static primitive's mesh, at the generator's transform (made relative to the generated mesh's local origin when asked) times the primitive's object transform
// Leaf: ?AddGeneratedObjects@CEnginePrimitiveRenderUtil@@QAEXABVCEngineInternalPrimitiveStaticMesh@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF12A0, 1549 bytes (`sub esp,0ACh` .. `ret 4` at 0x00BF186D, then the GetMesh-failure tail
//   0x00BF1870..0x00BF18A8 that jumps back into the loop; three int3 pad bytes, the animated overload starts at 0x00BF18B0).
// Identity: Ego_r.pdb (0x00AA9420) / FableWin.pdb (0x02EEDF30) ?AddGeneratedObjects@CEnginePrimitiveRenderUtil@@QAEXABVCEngineInternalPrimitiveStaticMesh@@@Z
//   (the first overload; the animated one 0x00BF18B0 follows it, donor order 0x00AA9420 < 0x00AA99E0); `ret 4` = the one reference argument,
//   `this` (an empty CEnginePrimitiveRenderUtil) never read. Callers 0x00B9EB1E / 0x00BA13FD (the static-mesh managers).
// Callees: C3DMeshInfo::IsLoaded 0x00A23E40, GetMeshStats 0x00A25530, CMeshDataBank::PeekMesh 0x00A24270 (on the resource manager's
//   MeshBanks[0], the inline PeekMeshBank), C3DMeshInfo::GetMesh 0x00A25E70 (flags 4), BeginLoading 0x00A256F0, ~CCountedPointer<const C3DMesh2>
//   0x00A36EC0 (called at the end of the local-origin block, expanded at the `continue`), CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh
//   0x00BA0FC0 (through the inline GetStaticMeshManager), ~CBaseClass 0x0099A2E0 (the expanded ~CEnginePrimitiveHandle), operator delete 0x00BFE9BC.
// Globals: NEngineInternal::GPResourceManager 0x01436E98, g_pEnginePrimitiveRenderer 0x01436E80 (the established spellings).
// Shape: the inline HasLOD carries the `lod < 4` bound, so GetBestCurrentDetailLevel's loop keeps retail's top-tested head (0x00BF1340);
//   the handle temporary's destructor is the shared header's FABLE_FORCEINLINE one (no out-of-line ??1CEnginePrimitiveHandle in Ego_r.pdb
//   or Fable.exe, only ??_G 0x0041C500); PreMultiply / PostMultiply are the SetMultiply-on-a-copy nesting (chunk 2006). The x87 term
//   order of the 24 matrix sums is VC7.1's canonical order, keyed by the symbols the values are first read through: the named
//   `generator` reference, the inverse read in place through `mesh->`, a named `static_mesh` reference for the primitive's fields
//   after the multiply and a named `static_mesh_manager` for the AddStaticMesh receiver give retail's order in every sum (a
//   generator search over ~600 neutral spellings: without the `static_mesh` alias the first post-multiply sum E11 at 0x00BF15A7 or
//   the pre-multiply E12 / E13 sums at 0x00BF13BD / 0x00BF13E6 come out in another term order).
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_C3DMESHINFO_GETMESHSTATS
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_LODS
#define FABLE_RETAIL_C3DMESHSTATS_GENERATORS
#define FABLE_RETAIL_C3DMESH2_INVERSETRANSFORM
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKMESHBANK
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_STATICMESHMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ADDGENERATEDOBJECTS
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#include "engine/CBankFile.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMeshStats.h"
#include "engine/C3DMesh2.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveRenderUtil.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;                      // 0x01436E80

void CEnginePrimitiveRenderUtil::AddGeneratedObjects(const CEngineInternalPrimitiveStaticMesh& primitive)
{
    if (primitive.MeshInfo->IsLoaded()) {
        CCountedPointer<const C3DMeshStats> mesh_stats = primitive.MeshInfo->GetMeshStats();
        for (const C3DGeneratorObject* it = mesh_stats->GetMeshGeneratorsBegin(); it != mesh_stats->GetMeshGeneratorsEnd(); ++it) {
            const C3DGeneratorObject& generator = *it;
            const CEngineInternalPrimitiveStaticMesh& static_mesh = primitive;
            if (generator.GetBankIndex()) {
                const C3DMeshInfo* mesh_info = NEngineInternal::GPResourceManager->PeekMeshBank((EMeshBankNo)0)->PeekMesh(generator.GetBankIndex());
                CMatrix3x4 transform = generator.PeekTransform();
                if (generator.GetUseLocalOrigin()) {
                    CCountedPointer<const C3DMesh2> mesh;
                    unsigned long lod = mesh_info->GetBestCurrentDetailLevel();
                    if (lod == 0xffffffff || !mesh_info->GetMesh(mesh, lod, 4)) {
                        mesh_info->BeginLoading(0xffffffff, false, false);
                        continue;
                    }
                    transform.PreMultiply(mesh->PeekInverseTransform());
                }
                transform.PostMultiply(static_mesh.ObjectTransform);
                CEnginePrimitiveManagerStaticMeshes& static_mesh_manager = g_pEnginePrimitiveRenderer->GetStaticMeshManager();
                static_mesh_manager.AddStaticMesh(mesh_info, mesh_info, transform, static_mesh.Scale,
                    static_mesh.FadeDistance, static_mesh.Alpha, static_mesh.TintColour, static_mesh.AlphaRef, static_mesh.AlphaMipBias, false,
                    static_mesh.EnginePrimitiveFlags);
            }
        }
    }
}
