// CEnginePrimitiveManager2DStaticMeshes::Render2DPrimitive(CEngineInternalPrimitiveBase&, CEngineSubPrimitiveChain&, ESubPrimitiveType) -- queue a 2D static mesh: skip it when off the viewport or outside the active 2D alpha-mask clip box or on a disabled layer; initialise it once, set its interpolated transform, start (or require) the mesh load, then fetch the mesh and queue one pool-allocated CEngineSubPrimitiveStaticMesh per mesh primitive at the layer's sort distance
// Leaf: ?Render2DPrimitive@CEnginePrimitiveManager2DStaticMeshes@@UAEXAAVCEngineInternalPrimitiveBase@@AAVCEngineSubPrimitiveChain@@W4ESubPrimitiveType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9E950, length 901 bytes (`ret 0Ch`, virtual slot 10).
// Identity: Ego_r.pdb / FableWin.pdb ?Render2DPrimitive@CEnginePrimitiveManager2DStaticMeshes@@UAEXAAVCEngineInternalPrimitiveBase@@AAVCEngineSubPrimitiveChain@@W4ESubPrimitiveType@@@Z
//   (vtable slot 10 of .?AVCEnginePrimitiveManager2DStaticMeshes@@).
// Port (agentNF77, round 7; q10478): moved off the local CEngineInternalPrimitive2DStaticMesh / C3DMeshInfo / C3DMesh2 / CMeshMaterial / CSubMesh /
//   CCountedMeshPointer / CDisplayManager / C2DClipInfo / CEnginePrimitiveTypeInfo / CEnginePrimitiveRenderer(2D) / CEngineSubPrimitive* /
//   CEnginePrimitiveManager2DStaticMeshes models and the CEngineLightingManagerDonor renaming include onto the shared headers, with the
//   0x00BA1140 (3D CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitive) preamble and its inline helper bodies, the 0x00BB0970 alpha-mask
//   clip helper, and these overlay opt-ins / new headers (hdr/README.md): engine/CEngineInternalPrimitive2DStaticMesh.h retail branch
//   (FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DSTATICMESH), new engine/CEnginePrimitiveManager2DStaticMeshes.h, CEnginePrimitiveManagerMeshBase.h
//   _RENDERMODE2D (GetAppropriateRenderMode2D), CEngineSubPrimitiveRenderer.h _SORTGRANULARITY (GetSortGranularity), and the repo
//   C3DMeshInfo.h _ISLOADED / _ARETEXTURESLOADED.
// Binders now on their PDB spellings: 0x00A23C60 ?AreTexturesLoaded@C3DMeshInfo@@QBE_NXZ (was the local ?IsBuilt@C3DMeshInfo@@QAE_NXZ; q10478),
//   0x00A23C50 ?BeginLoadingTextures@C3DMeshInfo@@QBEXW4EBankLoadPriority@@@Z (was ?SetLoadPriority@C3DMeshInfo@@QAEXJ@Z), 0x00A256F0
//   ?BeginLoading@C3DMeshInfo@@QBEXK_N0@Z, 0x00A23E40 ?IsLoaded@C3DMeshInfo@@QBE_NXZ, 0x00A25E70 ?GetMesh@C3DMeshInfo@@QBE_NAAV?$CCountedPointer@$$CBVC3DMesh2@@@@KK@Z,
//   0x00A36EC0 ??1?$CCountedPointer@$$CBVC3DMesh2@@@@QAE@XZ, 0x00BF12A0 ?AddGeneratedObjects@CEnginePrimitiveRenderUtil@@QAEXABVCEngineInternalPrimitiveStaticMesh@@@Z,
//   0x00B9DE90 ?InitialiseSimulation@CEngineInternalPrimitiveStaticMesh@@QAEXPBVC3DMeshInfo@@ABVC3DMesh2@@@Z, 0x00BEBBB0
//   ?IsSimulationRunning@CEngineInternalPrimitiveMeshBase@@QBE_NK@Z, 0x00B9CB10 ??0CEngineSubPrimitiveStaticMesh@@QAE@PAVCEngineInternalPrimitiveStaticMesh@@PAVC3DPrimitive2@@W4ERenderMode@@_N@Z,
//   0x00BEB540 ??2CEngineSubPrimitiveBase@@SIPAXI@Z, 0x01436E74 ?GPSubPrimitiveRenderer@NEngineInternal@@3PAVCEngineSubPrimitiveRenderer@@A, and slot 33
//   (0x84) is ?CalcInterpolatedObjectTransform@CEngineInternalPrimitive2DStaticMesh@@UAEXM_N@Z (was a local SetLOD) fed by the inline
//   GetCurrentInterpolate (CurrentInterpolationInfo.InterpolationSetList[InterpolationSet].WFInterpolate).
// Shape: the sub-primitive flags are the inline CalculateSubPrimitiveRenderFlags(false, IsTransparent, IsBooleanAlpha, EnginePrimitiveFlags, false)
//   and the render mode GetAppropriateRenderMode2D (2D_TEXTURE when map flag 1, else 2D_UNTEXTURED); the mesh info is read into named locals
//   for the load block (edi), the layer through a reference (`mov al,[ecx+edx+320h]` order) and the alpha as a long (`movzx`, dword spill).
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
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTH2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_LAYER2DTODISTANCE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_LAYER2DTOZ
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_GETVIEWPORT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#define FABLE_RETAIL_C3DMESHINFO_ARETEXTURESLOADED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DSTATICMESH
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_RENDERMODE2D
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_SORTGRANULARITY
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_SETTERS_ALPHAFLOAT
#include "engine/Direct3D9Types.h"
#include "engine/CRenderManager.h"
#include "engine/CEngineFadeDistance.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/CEngineInternalPrimitiveMorphedStaticMesh.h"
#include "engine/CEngineSubPrimitiveStaticMesh.h"
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
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitive2DViewportManager.h"
#include "engine/C2DAlphaMaskInfo.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/CEnginePrimitiveRenderer2D.h"
#include "engine/CDisplayManager.h"
#include "engine/CEngineInternalPrimitive2DStaticMesh.h"
#include "engine/CEnginePrimitiveManager2DStaticMeshes.h"

namespace NEngineInternal {
extern CDisplayManager* GPDisplayManager;                     // 0x01436E1C
extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo;         // 0x01436E84
extern CEnginePrimitiveRenderer2D* GPPrimitiveRenderer2D;     // 0x01436E7C
extern CEnginePrimitiveRenderer* GPPrimitiveRenderer;         // 0x01436E80
extern CEngineLightingManager* GPLightingManager;             // 0x01436E9C
extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer;   // 0x01436E74
extern CEngineShadowRenderer* GPShadowRenderer;               // 0x01436E60
}
inline bool CEnginePrimitiveManager::GetBlockOnLoad()
{ return NEngineInternal::GPPrimitiveRenderer->GetBlockOnLoad(); }
inline const C2DAlphaMaskInfo* CEnginePrimitive2DViewportManager::GetCurrentAlphaMask() { return &AlphaMaskInfo; }
FABLE_FORCEINLINE const C2DAlphaMaskInfo* CEnginePrimitiveRenderUtil::GetCurrent2DAlphaMaskInfo()
{
    CEnginePrimitive2DViewportManager* manager = GFDebugPDynamicCast(NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(ENGINE_PRIMITIVE_2D_VIEWPORT),
                                                                      (CEnginePrimitive2DViewportManager*)0);
    return manager->GetCurrentAlphaMask();
}
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
inline ERenderMode CEnginePrimitiveManagerMeshBase::GetAppropriateRenderMode2D(const C3DMeshMaterial* material,
    unsigned long modifier_flags,unsigned long subprim_render_flags)
{
    if (material->GetMapFlags() & 1) return RENDER_MODE_2D_TEXTURE;
    return RENDER_MODE_2D_UNTEXTURED;
}
inline void CEngineRenderInfoMeshBase::SetMesh(const C3DMesh2* mesh,unsigned long lod)
{ Mesh=mesh; LOD=lod; }

void CEnginePrimitiveManager2DStaticMeshes::Render2DPrimitive(CEngineInternalPrimitiveBase& primitive_base, CEngineSubPrimitiveChain& subprim_chain, ESubPrimitiveType type)
{
    if (!GetEnable())
        return;
    CEngineInternalPrimitive2DStaticMesh& primitive = static_cast<CEngineInternalPrimitive2DStaticMesh&>(primitive_base);
    const C2DBoxI& viewport = NEngineInternal::GPDisplayManager->GetViewport();
    if (viewport.BRX < primitive.ClipBox.TLX || viewport.TLX > primitive.ClipBox.BRX || viewport.BRY < primitive.ClipBox.TLY || viewport.TLY > primitive.ClipBox.BRY)
        return;
    const C2DAlphaMaskInfo* clip_info = GetCurrent2DAlphaMaskInfo();
    if (clip_info->AlphaMaskBankIndex && clip_info->AlphaMaskBank && clip_info->MaskObjectsOutsideAlphaMask) {
        if (primitive.ClipBox.TLX > clip_info->AlphaMaskClipBox.BRX || primitive.ClipBox.BRX < clip_info->AlphaMaskClipBox.TLX ||
            primitive.ClipBox.TLY > clip_info->AlphaMaskClipBox.BRY || primitive.ClipBox.BRY < clip_info->AlphaMaskClipBox.TLY)
            return;
    }
    const EEngine2DLayer& layer = primitive.Layer;
    if (!NEngineInternal::GPPrimitiveRenderer2D->GetEnable2DLayer(layer))
        return;
    if (!primitive.Initialised)
        primitive.Initialise();
    primitive.CalcInterpolatedObjectTransform(primitive.GetCurrentInterpolate(), true);
    bool urgent = (primitive.EnginePrimitiveFlags >> 7) & 1;
    const C3DMeshInfo* mesh_info = primitive.MeshInfo;
    if (!GetBlockOnLoad()) {
        mesh_info->BeginLoading(0, false, false);
        mesh_info->BeginLoadingTextures(urgent ? (EBankLoadPriority)2 : (EBankLoadPriority)1);
    }
    if (!urgent && !GetBlockOnLoad()) {
        const C3DMeshInfo* current_mesh_info = primitive.MeshInfo;
        if (!current_mesh_info->IsLoaded())
            return;
        if (!current_mesh_info->AreTexturesLoaded())
            return;
    }
    CCountedPointer<const C3DMesh2> mesh;
    if (!primitive.MeshInfo->GetMesh(mesh, 0, 0x10))
        return;
    CEngineRenderInfoStaticMesh* render_info = primitive.GetRenderInfo();
    render_info->SetMesh(mesh.Get(), 0);
    AddGeneratedObjects(primitive);
    if (mesh->ContainsCloth()) primitive.InitialiseSimulation(primitive.MeshInfo, *mesh);
    render_info->Lights = &NEngineInternal::GPLightingManager->EmptyLightArray;
    for (unsigned long c_prim = 0; c_prim < mesh->GetPrimitiveCount(); ++c_prim) {
        C3DPrimitive2* mesh_prim = mesh->GetPrimitive(c_prim);
        const C3DMeshMaterial* material = mesh->GetMaterial(mesh_prim->GetMaterialIndex());
        long prim_alpha = primitive.Alpha;
        unsigned long render_flags = CalculateSubPrimitiveRenderFlags(false, material->IsTransparent(), material->IsBooleanAlpha(),
            primitive.EnginePrimitiveFlags, false);
        ERenderMode render_mode = GetAppropriateRenderMode2D(material, primitive.EnginePrimitiveFlags, render_flags);
        bool use_cloth = mesh_prim->HasCloth() && NEngineInternal::GPPrimitiveRenderer->GetClothRenderer().GetEnable()
            && primitive.IsSimulationRunning(c_prim);
        CEngineSubPrimitiveStaticMesh* subprim = new CEngineSubPrimitiveStaticMesh(&primitive, mesh_prim, render_mode, use_cloth);
        subprim->SetAlpha(static_cast<unsigned char>(prim_alpha));
        subprim->SetRenderFlags(render_flags);
        float distance = NEngineInternal::GPPrimitiveRenderer2D->Layer2DToSortDistance(primitive.Layer);
        if (render_flags & 1)
            distance -= NEngineInternal::GPSubPrimitiveRenderer->GetSortGranularity();
        NEngineInternal::GPSubPrimitiveRenderer->AddSubPrimitive(type, subprim, distance, this);
    }
}
