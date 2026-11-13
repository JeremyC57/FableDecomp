// CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive(CEngineInternalPrimitiveBase& primitive_base, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain) -- render a repeated static mesh: when the manager is enabled, take the camera distance of the bounding sphere; in an occlusion pass set up the per-layer occlusion record and, while the primitive is occluded, queue its bounding box (result 1); pick the LOD from the screen-space error, fetch that LOD mesh (bounding box + texture load request when it is not resident, result 1), record mesh and local lights, clip the bounding box against the shadow receiver volume and queue one repeated-mesh sub-primitive per textured mesh primitive with its render mode and render flags (result 2)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9A4A0, 1674 bytes (`ret 0Ch`, ends 0x00B9AB2A, int3 padding then CreatePrimitive 0x00B9AB30), vtable 0x012A3E9C slot 7.
// Identity: Ego_r.pdb / FableWin.pdb ?RenderPrimitive@CEnginePrimitiveManagerRepeatedStaticMeshes@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z
//   (rtti-vtables.tsv slot 7 of the class's vtable; RTTI chain RepeatedStaticMeshes : MeshBase : OcclusionBase : CEnginePrimitiveManager : CEnginePrimitiveRenderUtil).
// Retail-asm: 0x00B9A75F..0x00B9A799 `movss xmm0,[ecx+8] ; movhps xmm0,[ecx] ; ... maxps ; minps ; subps ; mulps ; shufps ; addps ; addss ; movss [esp+4Ch],xmm2`
//   -- C3DBoundingBox::CalcShortestDistanceToPointSquared's hand SSE kernel behind g_SSEAvailable, the shared engine/C3DBoundingBoxSSE.h helper (attempts:
//   that header and rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/); expanded through the inline CShadowClipVolume::ClipBoundingBox. No asm in this row.
// Inline-budget: FABLE_FORCEINLINE on SetupOcclusionInfo (c2 inline score 138), IsOccluded (80), CIVCountedPointer::Reset (80), ClipBoundingBox (534),
//   CalcShortestDistanceToPointSquared and CalculateSubPrimitiveRenderFlags (141) -- FableWin-only inlines expanded by retail; plain inline keeps the
//   CEngineSubPrimitiveBoundingBox ctor called at the occlusion site (91, retail 0x00B9A5BE) / expanded at the not-loaded site (69) and the
//   ~CCountedPointer<const C3DMesh2> called on the not-loaded return (76, retail 0x00B9A6D1) / expanded at the end (47), as retail. No /d2inlT value fits
//   (138 / 534 must expand while 76 / 91 must not). Trace: tools/inl.sh (copy of agent2003 inl_host).
// Shape: the material flags go through the FableWin inline accessors IsTransparent() / IsBooleanAlpha() / GetMapFlags() /
//   GetDiffuseMapBankIndex(). With IsTransparent() the inlined CalculateSubPrimitiveRenderFlags' `translucent` home is a 1-byte,
//   byte-aligned frame node, so c2's packer puts it in the top byte of the B+8 word ([esp+1Bh] at 0x00B9AA25 / 0x00B9AA56); read
//   straight from the member, the same home is a dword-aligned node and lands at [esp+18h] (frame-packer trace, chunk 3013 NOTES.md).
// Leaf: ?RenderPrimitive@CEnginePrimitiveManagerRepeatedStaticMeshes@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ISPERSISTENT
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_ISOCCLUDED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_SETUP_FORCEINLINE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEREPEATEDSTATICMESH
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_BLOCKONLOAD
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_SET
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_OCCLUDED
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADDTYPED
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_C3DMESHMATERIAL_FLAG_ACCESSORS
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#define FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL
#define FABLE_RETAIL_C3DMESHINFO_BEGINLOADINGTEXTURES
#define FABLE_RETAIL_C3DMESHINFO_LODERROR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CSHADOWCLIPVOLUME_CLIPBOUNDINGBOX
#define FABLE_RETAIL_CSHADOWRENDERERSCENEDATA_COPY
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALSHADOWMODE
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#include "fable_compiler.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineCameraDesc.h"
#include "engine/CEngineCamera.h"
#include "engine/C3DBoundingBoxSSE.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/EEngineOcclusionLayer.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CShadowClipVolume.h"
#include "engine/CShadowRendererSceneData.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CEngineInternalPrimitiveRepeatedStaticMesh.h"
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveOcclusionInfo.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CEngineSubPrimitiveBoundingBox.h"
#include "engine/CEngineSubPrimitiveRepeatedStaticMesh.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/CCountedPointer.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineLightingManager.h"

extern bool g_SSEAvailable;                                              // 0x013D2880
extern CEnginePrimitiveOcclusionManager*   g_pPrimitiveOcclusionManager;   // 0x01436E58
extern CEngineShadowRenderer*              g_pEngineShadowRenderer;        // 0x01436E60
extern CEngineSubPrimitiveRenderer*        g_pEngineSubPrimitiveRenderer;  // 0x01436E74
extern CEnginePrimitiveRendererSharedData* g_pPrimitiveRendererSharedData; // 0x01436E78
extern CEnginePrimitiveRenderer*           g_pEnginePrimitiveRenderer;     // 0x01436E80
extern CEngineLightingManager*             g_pEngineLightingManager;       // 0x01436E9C
extern CEngineCamera*                      g_pEngineCamera;                // 0x01436EA0

inline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera() { return *g_pEngineCamera; }
inline float __fastcall CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const C3DVector& position)
{
    return (position - GetCurrentCamera().PeekCameraPos()).GetMagnitude();
}
inline EEngineOcclusionLayer __fastcall CEnginePrimitiveRenderUtil::GetCurrentOcclusionLayer() { return g_pPrimitiveRendererSharedData->GetCurrentOcclusionLayer(); }
inline bool __fastcall CEnginePrimitiveRenderUtil::GetOcclusionEnable() { return g_pPrimitiveRendererSharedData->GetOcclusionEnable(); }
inline float __fastcall CEnginePrimitiveRenderUtil::GetLODErrorTolerance() { return g_pPrimitiveRendererSharedData->LODErrorTolerance; }
inline float __fastcall CEnginePrimitiveRenderUtil::GetLODVirtualScreenSizeFOVAdjusted() { return g_pPrimitiveRendererSharedData->LODVirtualScreenSizeFOVAdjusted; }
FABLE_FORCEINLINE unsigned long CEnginePrimitiveRenderUtil::CalculateSubPrimitiveRenderFlags(bool fading, bool translucent, bool alpha_test,
                                                                                 unsigned long primitive_flags, bool shadowed)
{
    bool alpha = alpha_test;
    if ((primitive_flags & 0x100) && translucent)
        alpha = true;
    unsigned long flags = 0;
    if (fading)
        flags = ((unsigned char)(primitive_flags >> 9) & 1) ? 10 : 5;
    if (alpha)
        flags |= 2;
    else if (translucent)
        flags |= 5;
    if (shadowed && g_pEngineShadowRenderer->PeekRenderEnabled()) {
        flags |= 0x200;
        if (g_pEngineLightingManager->PeekGlobalShadowMode() == 2)
            flags |= 0x10;
    }
    if (primitive_flags & 0x800)
        flags |= 0xc0;
    if (primitive_flags & 8)
        flags |= 0x180;
    return flags;
}
inline bool __fastcall CEnginePrimitiveManager::GetBlockOnLoad() { return g_pEnginePrimitiveRenderer->GetBlockOnLoad(); }
inline void CEnginePrimitiveManager::AddSubPrimitive(CEngineSubPrimitiveBase* sub_primitive, float distance, unsigned char alpha, unsigned long flags)
{
    g_pEngineSubPrimitiveRenderer->AddSubPrimitive(sub_primitive, distance, alpha, flags, this);
}
FABLE_FORCEINLINE bool CEngineInternalPrimitiveOcclusionBase::IsOccluded(EEngineOcclusionLayer layer)
{
    if (EnginePrimitiveFlags & 0x4000)
        return false;
    return !HasOcclusionInfo(layer) || g_pPrimitiveOcclusionManager->IsPrimitiveOccluded(GetOcclusionInfo(layer));
}
FABLE_FORCEINLINE float C3DBoundingBox::CalcShortestDistanceToPointSquared(const C3DVector& point) const
{
    if (g_SSEAvailable) {
        float result;
        GF_SSE_BOX_DISTANCE_SQUARED(this, point, result);
        return result;
    }
    C3DVector closest(point);
    if (closest.X < MinCoord.X)
        closest.X = MinCoord.X;
    else if (closest.X > MaxCoord.X)
        closest.X = MaxCoord.X;
    if (closest.Y < MinCoord.Y)
        closest.Y = MinCoord.Y;
    else if (closest.Y > MaxCoord.Y)
        closest.Y = MaxCoord.Y;
    if (closest.Z < MinCoord.Z)
        closest.Z = MinCoord.Z;
    else if (closest.Z > MaxCoord.Z)
        closest.Z = MaxCoord.Z;
    C3DVector d;
    d.X = point.X - closest.X;
    d.Y = point.Y - closest.Y;
    float dz = point.Z - closest.Z;
    float sum = dz * dz;
    sum += d.Y * d.Y;
    sum += d.X * d.X;
    return sum;
}

ERendered CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive(CEngineInternalPrimitiveBase& primitive_base, EEngineRenderLayer layer,
                                                                       CEngineSubPrimitiveChain& chain)
{
    if (!GetEnable())
        return RENDERED_NOTHING;
    CEngineInternalPrimitiveRepeatedStaticMesh& primitive = static_cast<CEngineInternalPrimitiveRepeatedStaticMesh&>(primitive_base);
    float distance = CalcDistanceFromCamera(primitive.BoundingSphere.GetCenter());
    float radius = primitive.BoundingSphere.GetRadius();
    EEngineOcclusionLayer occlusion_layer = GetCurrentOcclusionLayer();
    if (GetOcclusionEnable()) {
        if (primitive.IsPersistent())
            primitive.SetupOcclusionInfo(occlusion_layer);
        if (primitive.IsOccluded(occlusion_layer)) {
            g_pEngineSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_BOUNDING_VOLUME, new CEngineSubPrimitiveBoundingBox(&primitive), distance, this);
            return RENDERED_BOUNDING_VOLUME;
        }
    }
    float tolerance = GetLODErrorTolerance();
    const C3DMeshInfo* mesh_info = primitive.MeshInfo;
    float nearest = distance - radius;
    if (!(nearest > DELTA_NOTIONAL_ZERO))
        nearest = DELTA_NOTIONAL_ZERO;
    float lod_error = nearest / (radius * GetLODVirtualScreenSizeFOVAdjusted()) * tolerance;
    unsigned long lod = 0;
    while (mesh_info->HasLOD(lod + 1) && !(mesh_info->GetLODError(lod + 1) > lod_error))
        ++lod;
    CCountedPointer<const C3DMesh2> mesh;
    if (!mesh_info->GetMesh(mesh, lod, GetBlockOnLoad() ? 0x11 : 0)) {
        g_pEngineSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_BOUNDING_VOLUME, new CEngineSubPrimitiveBoundingBox(&primitive), distance, this);
        primitive.MeshInfo->BeginLoadingTextures((EBankLoadPriority)1);
        return RENDERED_BOUNDING_VOLUME;
    }
    primitive.GetRenderInfo()->Mesh = const_cast<C3DMesh2*>(mesh.Get());
    primitive.GetRenderInfo()->Lights = g_pEngineLightingManager->GetLightsInArea(primitive.BoundingBox, 5, CEnginePrimitiveThemeChannelDesc());
    const CShadowRendererSceneData scene_data = g_pEngineShadowRenderer->PeekSceneData();
    bool shadowed = scene_data.RecieverClipVolume.ClipBoundingBox(primitive.BoundingBox);
    for (unsigned long i = 0; i < mesh->PrimitiveCount; ++i) {
        C3DPrimitive2* mesh_primitive = &mesh->Primitives[i];
        const C3DMeshMaterial* material = mesh->GetMaterial(mesh_primitive->MaterialIndex);
        if (material->GetDiffuseMapBankIndex() == 0)
            continue;
        ERenderMode render_mode;
        unsigned long map_flags = material->GetMapFlags();
        if (map_flags & 1) {
            switch (map_flags & ~1) {
            case 2: case 6: render_mode = (ERenderMode)8; break;
            case 4: render_mode = (ERenderMode)7; break;
            default: render_mode = (ERenderMode)4; break;
            }
        } else {
            render_mode = (ERenderMode)1;
        }
        CEngineSubPrimitiveBase* sub_primitive = new CEngineSubPrimitiveRepeatedStaticMesh(&primitive, mesh_primitive, render_mode);
        bool fading = radius + distance > g_pEnginePrimitiveRenderer->GetRepeatedMeshDrawDistanceFactor() * primitive.FadeDistance.FadeBeginDistance;
        AddSubPrimitive(sub_primitive, distance, 255,
                        CalculateSubPrimitiveRenderFlags(fading, material->IsTransparent(), material->IsBooleanAlpha(),
                                                         primitive.EnginePrimitiveFlags | 0x200, shadowed));
    }
    return RENDERED_OBJECT;
}