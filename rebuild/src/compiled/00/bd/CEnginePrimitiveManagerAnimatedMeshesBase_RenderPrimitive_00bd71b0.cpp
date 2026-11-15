// CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitive -- cull and submit animated mesh sub-primitives.
// Leaf: ?RenderPrimitive@CEnginePrimitiveManagerAnimatedMeshesBase@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z
// Flags: /d2inlT256
// Inline-budget: /d2inlT256 -- plain T256 leaves ClipSphere called and T512 also expands the portable float conversion, so ClipSphere is FABLE_FORCEINLINE at its one site; the first bounding-box constructor call (0x00BD7373) is a scoped inline_depth(0) statement, the second expands (MATCH_NOTES.md).
// Retail: 00BD71B0..00BD79F4,2116bytes; reserved00BD71CC skips28-bytehead.
// Retail-asm: 0x00BD76A6 `fistp dword ptr [edx]`, the shared GFFastFloatToLong helper (the fade conversion).
// Evidence: both original PDBs, full donor02E82300 scopes/calls, native vtable012A78DC slot7.
// Original named fade conversion:40fresh C++/intrinsic site diagnostics; original donor/native proof and65536input twin checks in ROUNDING_ATTEMPTS.md.
#define FABLE_RETAIL_PASS22_ANIMATED_MESH_RENDER
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_WORLDPOS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DANIMATEDMESH
#define FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_BLOCKONLOAD
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_APPROPRIATELOD
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ADDGENERATEDOBJECTS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_FINDLOCALLIGHTS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ISPERSISTENT
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_SET
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_OCCLUDED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_ISOCCLUDED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_GETRENDERINFO
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_LODERROR
#define FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL
#define FABLE_RETAIL_C3DMESHINFO_BEGINLOADINGTEXTURES
#define FABLE_RETAIL_C3DMESH2_PRIMITIVES
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_FRONT_PASS22
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SILHOUETTES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CSHADOWRENDERERSCENEDATA_COPY
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADDTYPED
#define FABLE_RETAIL_CENGINECLOTHRENDERER_MEMBERS
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_C3DPRIMITIVE2_CLOTHPRIMITIVES
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_C3DMESHMATERIAL_FLAG_ACCESSORS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALSHADOWMODE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHERE
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CPLANE_ACCESSORS
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CMATRIX3X4
#include "engine/CEngineFadeDistance.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveManagerAnimatedMeshesBase.h"
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/CEngineInternalPrimitiveMorphedAnimatedMesh.h"
#include "engine/CEngineInternalPrimitive2DMorphedAnimatedMesh.h"
#include "engine/CEngineSubPrimitiveAnimatedMesh.h"
#include "engine/CEngineSubPrimitiveBoundingBox.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineClothRenderer.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFastFloatToLong.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CShaderRenderManager.h"

namespace NEngineInternal {
// Both original public sets and native00BA11F2; existing correct binding retained.
extern CEngineCamera* GPCamera;
extern CEnginePrimitiveOcclusionManager* GPPrimitiveOcclusionManager;
extern CEngineLightingManager* GPLightingManager;
extern CShaderRenderManager* GPShaderRenderManager;
extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer;
extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData;
extern CEnginePrimitiveRenderer* GPPrimitiveRenderer;
extern CEngineShadowRenderer* GPShadowRenderer;
}

// Original donor02E74B50/02E74B60/02EA0540: direct shared-data reads.
inline EEngineOcclusionLayer CEnginePrimitiveRenderUtil::GetCurrentOcclusionLayer()
{ return NEngineInternal::GPPrimitiveRendererSharedData->CurrentOcclusionLayer; }
inline bool CEnginePrimitiveRenderUtil::GetOcclusionEnable()
{ return NEngineInternal::GPPrimitiveRendererSharedData->OcclusionEnable; }
inline float CEnginePrimitiveRenderUtil::GetLODErrorTolerance()
{ return NEngineInternal::GPPrimitiveRendererSharedData->LODErrorTolerance; }
inline float CEnginePrimitiveRenderUtil::GetLODVirtualScreenSizeFOVAdjusted()
{ return NEngineInternal::GPPrimitiveRendererSharedData->LODVirtualScreenSizeFOVAdjusted; }
inline bool CEnginePrimitiveManager::GetBlockOnLoad()
{ return NEngineInternal::GPPrimitiveRenderer->GetBlockOnLoad(); }
// Original module915 AddBoundingVolume02E830E0 and five-argument AddSubPrimitive02E833D0.
inline void CEnginePrimitiveManager::AddBoundingVolume(CEngineSubPrimitiveBase* subprim,float distance)
{ NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_BOUNDING_VOLUME,subprim,distance,this); }
inline void CEnginePrimitiveManager::AddSubPrimitive(CEngineSubPrimitiveBase* subprim,float distance,
    unsigned char primitive_alpha,unsigned long render_flags,CEngineSubPrimitiveChain& subprim_chain)
{ NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(subprim,distance,primitive_alpha,render_flags,this,subprim_chain); }
// Original full GetAppropriateLOD02E78E30, debug assertion omitted as native does.


// Complete original bodies: FableWin02EA1500,02E83420,02E82240,02E1C2E0,
// both original PDB types and native parent branch/field accesses. No surrogate callees.

inline ERenderMode CEnginePrimitiveManagerMeshBase::GetAppropriateRenderMode(
    const C3DMeshMaterial* material,unsigned long modifier_flags,unsigned long subprim_render_flags)
{
    if (material->GetMapFlags() & 1) {
        unsigned long additional_maps=material->GetMapFlags() & ~1UL;
        if (subprim_render_flags & 0x200) {
            if (subprim_render_flags & 0x10) {
                if (additional_maps & 4) return RENDER_MODE_TEXTURE_ENV_FOG_SPOT_SHADOW;
                return RENDER_MODE_TEXTURE_FOG_SPOT_SHADOW;
            }
            switch (additional_maps) {
            case 2: case 6: return RENDER_MODE_TEXTURE_DP3_BUMP_FOG_SHADOW;
            case 4: return RENDER_MODE_TEXTURE_ENV_FOG_SHADOW;
            default: return RENDER_MODE_TEXTURE_FOG_SHADOW;
            }
        }
        switch (additional_maps) {
        case 2: case 6: return RENDER_MODE_TEXTURE_DISABLED_BUMP_FOG;
        case 4: return RENDER_MODE_TEXTURE_ENV_FOG;
        default: return RENDER_MODE_TEXTURE_FOG;
        }
    }
    return RENDER_MODE_UNTEXTURED;
}
inline const unsigned char* CEngineInternalPrimitiveMorphInfo::GetHiddenMaterials(const C3DMesh2& mesh)
{
    if (HiddenMaterialNames.empty()) return 0;
    if (HiddenMaterials.empty()) InitialiseHiddenMaterials(mesh);
    return HiddenMaterials.GetAsCArray();
}
FABLE_FORCEINLINE bool CShadowClipVolume::ClipSphere(const C3DVector& pos,float radius)const
{
    if (ClipToSphere) {
        if ((pos-BoundingSphere.Center).GetSquaredMagnitude() > GFSqr(radius+BoundingSphere.Radius))
            return false;
    }
    for(long n=0;n<ClipPlaneCount;++n) {
        if(pos.Dot(ClipPlanes[n].GetNormal())-radius > ClipPlanes[n].GetDistance()) return false;
    }
    return true;
}

// Original module915 CalcDistanceFromCamera02E822C0, true camera accessor.
inline CEngineCamera& CEnginePrimitiveRenderUtil::GetCurrentCamera()
{ return *NEngineInternal::GPCamera; }
inline float CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const C3DVector& world_pos)
{ return (world_pos-GetCurrentCamera().PeekCameraPos()).GetMagnitude(); }
// Original GetAlphaIncludingDistanceFade02E80810: the native virtual fade-distance
// return and conditional local are preserved. Existing portable twin while per-site trials proceed.
inline unsigned char CEngineInternalPrimitiveBase::GetAlphaIncludingDistanceFade(float distance)
{
    if (!Alpha) return 0;
    CEngineFadeDistance fade_dist = OverrideFadeDistance ? GetFadeDistance() : FadeDistance;
    if (distance > fade_dist.MaxDrawDistance) return 0;
    if (distance < fade_dist.FadeBeginDistance) return Alpha;
    long dist_alpha = 255-GFFastFloatToLong((distance-fade_dist.FadeBeginDistance)*255.0f/
        (fade_dist.MaxDrawDistance-fade_dist.FadeBeginDistance));
    return (Alpha*dist_alpha)>>8;
}

// Original GetMaximumLights02E012F0 returns6; native parent pushes5 at00BA143C.
inline long CShaderRenderManager::GetMaximumLights(){return 6;}
// Original module915 FindLocalLights02E837F0, including by-value theme channel.
inline CEngineLocalLightArray* CEnginePrimitiveRenderUtil::FindLocalLights(
    const C3DVector& world_pos,float radius,CEnginePrimitiveThemeChannelDesc theme_channel)
{
    return NEngineInternal::GPLightingManager->GetLightsInSphereFast(C3DBoundingSphere(world_pos,radius),
        NEngineInternal::GPShaderRenderManager->GetMaximumLights()-1,theme_channel);
}
// Original donor02E83520 release path: debug-only error message is absent in native00BA1737..00BA17BE.
inline unsigned long CEnginePrimitiveRenderUtil::CalculateSubPrimitiveRenderFlags(bool primitive_alphaed,
    bool texture_has_alpha,bool texture_has_boolean_alpha,unsigned long primitive_mod_flags,bool in_shadow_range)
{
    if((primitive_mod_flags & 0x100) && texture_has_alpha) texture_has_boolean_alpha=true;
    bool use_stipple_alpha=(primitive_mod_flags & 0x200)!=0;
    unsigned long flags=0;
    if(primitive_alphaed) {
        if(use_stipple_alpha) flags |= 0xa;
        else flags |= 5;
    }
    if(texture_has_boolean_alpha) flags |= 2;
    else if(texture_has_alpha) flags |= 5;
    if(in_shadow_range && NEngineInternal::GPShadowRenderer->PeekRenderEnabled()) {
        flags |= 0x200;
        if(NEngineInternal::GPLightingManager->PeekGlobalShadowMode()==(ELightingManagerShadowMode)2) flags |= 0x10;
    }
    if(primitive_mod_flags & 0x800) flags |= 0xc0;
    if(primitive_mod_flags & 8) flags |= 0x180;
    return flags;
}

// Original02E17A80, both PDB method/global names and native00BA128E..00BA12B1.
// Missing occlusion information is treated as occluded, unless the primitive opts out.
inline bool CEngineInternalPrimitiveOcclusionBase::IsOccluded(EEngineOcclusionLayer layer)
{
    if(!(EnginePrimitiveFlags & 0x4000)) {
        if(!HasOcclusionInfo(layer) || NEngineInternal::GPPrimitiveOcclusionManager->IsPrimitiveOccluded(*OcclusionInfo[layer]))
            return true;
    }
    return false;
}

// Original SetMesh02E78DE0 and native00BA13F1/13F7 store the same two fields.



// Original donor02E78480 evaluates the two by-value sphere expressions separately.
inline float CEnginePrimitiveRenderer::GetBoundingRadiusAnimated(const C3DMeshInfo& mesh_info,float scale)const
{ return (mesh_info.GetBoundingSphere().Radius+mesh_info.GetBoundingSphere().GetCenter().GetMagnitude())*scale+BoundingRadiusFix; }
inline bool CEnginePrimitiveRenderUtil::GetBoneViewEnabled()
{ return NEngineInternal::GPPrimitiveRendererSharedData->BoneViewEnabled; }
ERendered CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitive(
    CEngineInternalPrimitiveBase& primitive_base, EEngineRenderLayer layer,
    CEngineSubPrimitiveChain& subprim_chain)
{
    if (!GetEnable()) return RENDERED_NOTHING;
    CEngineInternalPrimitiveAnimatedMesh& primitive = static_cast<CEngineInternalPrimitiveAnimatedMesh&>(primitive_base);
    CEngineRenderInfoAnimatedMesh* render_info=primitive.GetRenderInfo();
    primitive.CalcInterpolatedObjectTransform(render_info->ObjectTransform,primitive.GetCurrentInterpolate(),true);
    const CMatrix3x4& obj_transform=render_info->ObjectTransform;
    float bounding_radius=NEngineInternal::GPPrimitiveRenderer->GetBoundingRadiusAnimated(*primitive.MeshInfo,primitive.Scale);
    C3DVector center=primitive.MeshInfo->GetBoundingSphere().GetCenter();
    obj_transform.TransformPoint(center);
    EEngineOcclusionLayer occlusion_layer = GetCurrentOcclusionLayer();
    float distance = CalcDistanceFromCamera(center);
    render_info->Distance=distance;
    if (GetOcclusionEnable()) {
        if (primitive.IsPersistent()) primitive.SetupOcclusionInfo(occlusion_layer);
        if (primitive.IsOccluded(occlusion_layer)) {
            // Inline-budget: native00BD7373 calls the real ctor; other site expands it.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            CEngineSubPrimitiveBoundingBox* subprim = new CEngineSubPrimitiveBoundingBox(&primitive);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            AddBoundingVolume(subprim, distance);
            return RENDERED_BOUNDING_VOLUME;
        }
    }
    if (!primitive.SetupMesh(distance,bounding_radius)) {
        AddBoundingVolume(new CEngineSubPrimitiveBoundingBox(&primitive),distance);
        primitive.MeshInfo->BeginLoadingTextures(BANK_LOAD_PRIORITY_NORMAL);
        return RENDERED_BOUNDING_VOLUME;
    }
    const C3DMesh2* mesh=primitive.GetRenderInfo()->Mesh;
    AddGeneratedObjects(primitive,obj_transform);
    if (mesh->ContainsCloth()) primitive.InitialiseSimulation(primitive.MeshInfo, *mesh);
    render_info->Lights = FindLocalLights(center, bounding_radius, primitive.ThemeChannelDesc);
    const unsigned char* hidden_materials = 0;
    if (primitive.GetType() == ENGINE_PRIMITIVE_MORPHED_ANIMATING_MESH) {
        CEngineInternalPrimitiveMorphedAnimatedMesh& morph_primitive = static_cast<CEngineInternalPrimitiveMorphedAnimatedMesh&>(primitive);
        hidden_materials = morph_primitive.Morph.GetHiddenMaterials(*mesh);
    }
    else if(primitive.GetType()==ENGINE_PRIMITIVE_2D_MORPHED_ANIMATING_MESH) {
        CEngineInternalPrimitive2DMorphedAnimatedMesh& morph_primitive=static_cast<CEngineInternalPrimitive2DMorphedAnimatedMesh&>(primitive);
        hidden_materials=morph_primitive.Morph.GetHiddenMaterials(*mesh);
    }
    // Inline-budget: native00BD7522..75C7 expands the actual ClipSphere helper.
    bool in_shadow_range = !(primitive.EnginePrimitiveFlags & 0x1000)
        && NEngineInternal::GPShadowRenderer->PeekSceneData().RecieverClipVolume.ClipSphere(center, bounding_radius);
    unsigned long no_primitives=mesh->GetPrimitiveCount();
    for (unsigned long c_prim = 0; c_prim < no_primitives; ++c_prim) {
        C3DPrimitive2* mesh_prim = mesh->GetPrimitive(c_prim);
        long material_index = mesh_prim->GetMaterialIndex();
        if (hidden_materials && hidden_materials[material_index]) continue;
        const C3DMeshMaterial* material = mesh->GetMaterial(material_index);
        long prim_alpha = primitive.GetAlphaIncludingDistanceFade(distance);
        if (!prim_alpha) continue;
        unsigned long render_flags = CalculateSubPrimitiveRenderFlags(prim_alpha < 255,
            material->IsTransparent(), material->IsBooleanAlpha(), primitive.EnginePrimitiveFlags, in_shadow_range);
        ERenderMode render_mode = GetAppropriateRenderMode(material, primitive.EnginePrimitiveFlags, render_flags);
        bool use_cloth = mesh_prim->HasCloth() && NEngineInternal::GPPrimitiveRenderer->GetClothRenderer().GetEnable()
            && primitive.IsSimulationRunning(c_prim);
        if (primitive.MeshEffect.Get()) {
            const CEngineMeshEffect& effect = *primitive.MeshEffect;
            switch (effect.GetEffect()) {
            case MESH_EFFECT_OUTLINE_SMOOTH: {
                const CEngineMeshEffectOutline& outline_effect = static_cast<const CEngineMeshEffectOutline&>(effect);
                NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_OUTLINE_STENCIL_SMOOTH,
                    new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth), distance, this);
                if (material->IsTransparent())
                    NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_OUTLINE_ALPHA_STENCIL_MASK_SMOOTH,
                        new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth), distance, this);
                NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_OUTLINE_SMOOTH_FILL,
                    new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth), distance, this);
                render_flags |= 0xa0;
                break;
            }
            case MESH_EFFECT_OUTLINE:
                NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_OUTLINE_STENCIL,
                    new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth), distance, this);
                NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(SUBPRIM_TYPE_OUTLINE_FILL,
                    new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth), distance, this);
                render_flags |= 0xa0;
                break;
            }
        }
        CEngineSubPrimitiveAnimatedMesh* subprim = new CEngineSubPrimitiveAnimatedMesh(&primitive, mesh_prim, render_mode, use_cloth);
        AddSubPrimitive(subprim, distance, static_cast<unsigned char>(prim_alpha), render_flags, subprim_chain);
    }
    if(GetBoneViewEnabled()) RenderDebugBoneHierarchy(primitive);
    return RENDERED_OBJECT;
}