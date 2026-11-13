// CEnginePrimitiveRenderUtil::AddGeneratedObjects(const CEngineInternalPrimitiveAnimatedMesh& primitive, const CMatrix3x4& object_transform) -- add one static mesh for every mesh generator of a loaded animated primitive's mesh, at the generator's animated dummy-object position (made relative to the generated mesh's local origin when asked) times the object transform
// Leaf: ?AddGeneratedObjects@CEnginePrimitiveRenderUtil@@QAEXABVCEngineInternalPrimitiveAnimatedMesh@@ABVCMatrix3x4@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF18B0, 1576 bytes (`sub esp,0E4h` .. `ret 8` at 0x00BF1E98, then the GetMesh-failure tail
//   0x00BF1E9B..0x00BF1ED3 that jumps back into the loop: the whole extent is this function; next function 0x00BF1EE0).
// Identity: Ego_r.pdb (0x00AA99E0) / FableWin.pdb (0x02EEE310) ?AddGeneratedObjects@CEnginePrimitiveRenderUtil@@QAEXABVCEngineInternalPrimitiveAnimatedMesh@@ABVCMatrix3x4@@@Z
//   (the second overload; the static-mesh one is 0x00BF12A0 right before it, donor order 0x00AA9420 < 0x00AA99E0); `ret 8` = the two
//   reference arguments, `this` (an empty CEnginePrimitiveRenderUtil) never read. Callers 0x00BD2144 / 0x00BD7426 (the animated-mesh managers).
// Callees: C3DMeshInfo::IsLoaded 0x00A23E40, GetMeshStats 0x00A25530, CMeshDataBank::PeekMesh 0x00A24270 (on the resource manager's
//   MeshBanks[0], the inline PeekMeshBank), C3DDummyObject::C3DDummyObject 0x00A2E4B0, C3DMeshStats::GetDummyObjectPositionAnimated
//   (vtable slot 6, `call [edx+18h]`: the blend-pair overload; the C3DMesh2 override 0x00A89E40 writes the out object's BoneIndex),
//   C3DMeshInfo::GetMesh 0x00A25E70 (flags 0 here, 4 in the static overload), BeginLoading 0x00A256F0, ~CCountedPointer<const C3DMesh2>
//   0x00A36EC0 (called at the end of the local-origin block, expanded at the `continue`), CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh
//   0x00BA0FC0 (through the inline GetStaticMeshManager), ~CBaseClass 0x0099A2E0 (the expanded ~CEnginePrimitiveHandle), operator delete 0x00BFE9BC.
// Globals: NEngineInternal::GPResourceManager 0x01436E98, g_pEnginePrimitiveRenderer 0x01436E80 (the established spellings), the static
//   CEngineInternalPrimitiveBase::CurrentInterpolationInfo 0x0143B8E0 (InterpolationSetList[set].WFInterpolate = 0x0143B934 + set*12).
// Shape: the inline HasLOD carries the `lod < 4` bound, so GetBestCurrentDetailLevel's loop keeps retail's top-tested head (0x00BF19B0);
//   the handle temporary's destructor is the shared header's FABLE_FORCEINLINE one (no out-of-line ??1CEnginePrimitiveHandle in Ego_r.pdb
//   or Fable.exe, only ??_G 0x0041C500); PreMultiply / PostMultiply are the SetMultiply-on-a-copy nesting (chunk 2006). The x87 term
//   order of the 24 matrix sums is VC7.1's canonical order: a named `const C3DGeneratorObject& generator = *it;` reference (all
//   generator fields read through it) and the inverse transform read in place through `mesh->` give retail's order in every sum.
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
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_B
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
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#define FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL
#define FABLE_RETAIL_C3DMESHSTATS_GENERATORS
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECT_RESULT
#define FABLE_RETAIL_C3DMESH2_INVERSETRANSFORM
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKMESHBANK
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_STATICMESHMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ADDGENERATEDOBJECTS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ANIM
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
#define FABLE_RETAIL_C3DMESHINFO_LOADSTATS
#define FABLE_RETAIL_CMESHDATABANK_GETANIMATIONMANAGER
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
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveRenderUtil.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;                      // 0x01436E80

void CEnginePrimitiveRenderUtil::AddGeneratedObjects(const CEngineInternalPrimitiveAnimatedMesh& primitive, const CMatrix3x4& object_transform)
{
    if (primitive.MeshInfo->IsLoaded()) {
        CCountedPointer<const C3DMeshStats> mesh_stats = primitive.MeshInfo->GetMeshStats();
        for (const C3DGeneratorObject* it = mesh_stats->GetMeshGeneratorsBegin(); it != mesh_stats->GetMeshGeneratorsEnd(); ++it) {
            const C3DGeneratorObject& generator = *it;
            if (generator.GetBankIndex()) {
                const C3DMeshInfo* mesh_info = NEngineInternal::GPResourceManager->PeekMeshBank((EMeshBankNo)0)->PeekMesh(generator.GetBankIndex());
                C3DDummyObject dummy_object;
                mesh_stats->GetDummyObjectPositionAnimated(generator, dummy_object, primitive.Anim.StartState, primitive.Anim.EndState,
                    primitive.GetCurrentInterpolate(), primitive.MeshInfo->GetAnimationManager(), false);
                CMatrix3x4 transform = dummy_object.PeekTransform();
                if (generator.GetUseLocalOrigin()) {
                    CCountedPointer<const C3DMesh2> mesh;
                    unsigned long lod = mesh_info->GetBestCurrentDetailLevel();
                    if (lod == 0xffffffff || !mesh_info->GetMesh(mesh, lod, 0)) {
                        mesh_info->BeginLoading(0xffffffff, false, false);
                        continue;
                    }
                    transform.PreMultiply(mesh->PeekInverseTransform());
                }
                transform.PostMultiply(object_transform);
                g_pEnginePrimitiveRenderer->GetStaticMeshManager().AddStaticMesh(mesh_info, mesh_info, transform, primitive.Scale,
                    primitive.FadeDistance, primitive.Alpha, primitive.TintColour, primitive.AlphaRef, primitive.AlphaMipBias, false,
                    primitive.EnginePrimitiveFlags);
            }
        }
    }
}