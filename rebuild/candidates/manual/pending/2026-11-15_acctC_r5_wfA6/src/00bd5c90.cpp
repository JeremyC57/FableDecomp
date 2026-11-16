// CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& primitive) -- draw a skinned mesh into the shadow buffer: skip disabled managers and primitives flagged 0x2000; with shaders on and the render state captured, take the interpolated object transform, clip the mesh's world-space sphere (radius widened by the bone-offset fix) against the shadow caster volume, set up the mesh LOD, record the transform, find the hidden materials and composite surfaces of the morphed types, then for every visible primitive set the shadow vertex / pixel shaders and the alpha test (transparent materials also get their diffuse texture at the mipmap they need) and draw it with its bone palette
// Grade: RELOCATION_MATCH (2935 bytes).
// Retail-asm: 0x00BD65D1..0x00BD65EB `fld [ebp+4Ch] ; fsub [ebp-1Ch] ; fistp dword ptr [ebp+28h] ; fild [ebp+28h] ; fadd [ebp-20h] ; fstp [ebp-18h] ;
//   mov eax,[ebp+4Ch] ; cmp eax,[ebp-18h] ; jne ; inc dword ptr [ebp+28h]` = the shared engine/GFFloatToLongFloor.h helper (the mipmap size
//   floor inside the expanded GetAppropriateMipmap, as in the matched static row 0x00BA6240); this row carries no asm of its own.
// FITTED devices (owner acceptance test a/c/d, each measured): `int unusedN = 0;` x9 (Fidelity: re-keys c2 record ids, count-only);
//   the two asserts below; a named `material_index` for its two uses (the asserts
//   otherwise flip the hidden-materials SIB 0x00BD6379); the radius as `((radius + |centre|) * scale) + fix` (a natural grouping paren:
//   its no-code PAREN tuple moves the scene-data push 0x00BD5DBB into the Z-row sum as retail, MATCHING_PLAYBOOK scheduler rule);
//   shared FITTED opt-ins CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED + _BOOKMARK + ADDSTATE_DIRTY. EVIDENCED: TRANSFORMPOINT_LTR (FableWin
//   ?TransformPoint 61i/168b = this text), the depth-pack constant as a C4DVector temporary from a named const float (as matched 0x00BA6240).
// Inferred: 2 dead release asserts, FABLE_RETAIL_ASSERT(render_info) and FABLE_RETAIL_ASSERT(mesh) (fable_assert.h dead arm, no bytes):
//   they fix the bookmark test/or operand order of the first two stage-0 filter setters 0x00BD6239.. (owner acceptance test d).
// Leaf: ?RenderPrimitiveToShadowBuffer@CEnginePrimitiveManagerAnimatedMeshesBase@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD5C90, 2935 bytes, thiscall `ret 4`, ebp frame (lea ebp,[esp-74h] ; sub esp,2D4h), no EH frame.
// Identity: vtable 0x012A78DC slot 8; Ego_r.pdb 0x00A8AB30 / FableWin.pdb 0x02E81680 (same mangled name).
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_HIDDENMATERIALS
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_RENDERINFO
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_GETBONETRANSFORMATIONS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DANIMATEDMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SILHOUETTES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SHADOWBUFFERSHADERS
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#define FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL
#define FABLE_RETAIL_C3DMESHINFO_BEGINLOADINGTEXTURES
#define FABLE_RETAIL_C3DMESHINFO_LODERROR
#define FABLE_RETAIL_C3DMESH2_PRIMITIVES
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DTEXTURESTAGES_CTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_GETWIDTH
#define FABLE_RETAIL_C3DPRIMITIVE2_GETVERTEXBUFFER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SHADOWBUFFERSHADERS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_BOOKMARK
#define FABLE_RETAIL_CRENDERSTATEMANAGER_ADDSTATE_DIRTY
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKMINWCLAMP
#define FABLE_RETAIL_CSHADOWRENDERERSCENEDATA_COPY
#define FABLE_RETAIL_CSHADOWCLIPVOLUME_CLIPSPHERE
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_compiler.h"
#include "fable_assert.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CMatrix3x4.h"
#include "engine/C4DVector.h"
#include "engine/CEngineCameraDesc.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"

#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/CEngineInternalPrimitiveMorphedAnimatedMesh.h"
#include "engine/CEngineInternalPrimitive2DMorphedAnimatedMesh.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEnginePrimitiveManagerAnimatedMeshesBase.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CShadowClipVolume.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DTextureStages.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CMeshDataBank.h"
#include "engine/CCountedPointer.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CTexture.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFLimit.h"

namespace NEngineInternal {
extern CRenderManager* GPRenderManager;                                  // 0x01436E18
extern CShaderRenderManager* GPShaderRenderManager;                      // 0x01436E14
extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; // 0x01436E78
extern CEngineShadowRenderer* GPShadowRenderer; // 0x01436E60
extern CEngineCamera* GPCamera; // 0x01436EA0
extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; // 0x01436E80
}
extern unsigned char GTopBitInByteTable[256];                              // 0x0143B990
extern const float DELTA_NOTIONAL_ZERO;                                    // 0x0129BA3C

inline unsigned long FABLE_FASTCALL GFGetTopBitInByte(unsigned char value)
{
    return GTopBitInByteTable[value];
}

inline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera() { return *NEngineInternal::GPCamera; }
FABLE_FORCEINLINE float __fastcall CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const C3DVector& position)
{
    return (position - GetCurrentCamera().PeekCameraPos()).GetMagnitude();
}
inline float __fastcall CEnginePrimitiveRenderUtil::GetLODErrorTolerance() { return NEngineInternal::GPPrimitiveRendererSharedData->LODErrorTolerance; }
inline float __fastcall CEnginePrimitiveRenderUtil::GetLODVirtualScreenSizeFOVAdjusted() { return NEngineInternal::GPPrimitiveRendererSharedData->LODVirtualScreenSizeFOVAdjusted; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderShadowBufferRender() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderShadowBufferRender; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderShadowBufferRenderAlpha() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderShadowBufferRenderAlpha; }
inline float __fastcall CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() { return NEngineInternal::GPPrimitiveRendererSharedData->MipmapSizeMultiplier; }

FABLE_FORCEINLINE unsigned long FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(const C3DVector& camera_position,
    const CMatrix3x4& transform, const C3DPrimitive2& mesh_primitive, float scale, const C3DMeshMaterial* material,
    const CGraphicDataBank* graphic_bank)
{
    C3DBoundingSphere sphere = mesh_primitive.GetBoundingSphere();
    transform.TransformPoint(sphere.Center);
    unsigned long mipmap = 0;
    float radius = sphere.GetRadius() * scale;
    float distance = (camera_position - sphere.GetCenter()).GetMagnitude();
    distance = (distance - radius) * mesh_primitive.GetAverageTextureStretch();
    if (distance > DELTA_NOTIONAL_ZERO) {
        long graphic_index = material->GetDiffuseMapBankIndex();
        if (graphic_index > 0) {
            const CGraphicInfo* graphic = graphic_bank->PeekGraphic(graphic_index);
            float ratio = (float)graphic->GetWidth() / (CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() / distance * scale);
            long size = GFFloatToLongFloor(ratio);
            GFLimit(size, 1L, 255L);
            mipmap = GFGetTopBitInByte((unsigned char)size);
        }
    }
    return mipmap;
}

FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& buffer)
{
    D3DDevice->SetStreamSource(0, buffer.PVertexBuffer, 0, buffer.Stride);
}
ERendered CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& base_primitive)
{
    // Fidelity: nine unused initialised locals re-key c2 record ids (count-only; FITTED, owner acceptance test c).
    int unused0 = 0;
    int unused1 = 0;
    int unused2 = 0;
    int unused3 = 0;
    int unused4 = 0;
    int unused5 = 0;
    int unused6 = 0;
    int unused7 = 0;
    int unused8 = 0;
    if (!GetEnable())
        return RENDERED_NOTHING;
    CEngineInternalPrimitiveAnimatedMesh& primitive = static_cast<CEngineInternalPrimitiveAnimatedMesh&>(base_primitive);
    if (primitive.EnginePrimitiveFlags & 0x2000)
        return RENDERED_NOTHING;

    CScopedShadersEnable shaders_enable(true, true);
    CRenderStateScopedCaptureBlock capture_block;

    CMatrix3x4 transform;
    primitive.CalcInterpolatedObjectTransform(transform, primitive.GetCurrentInterpolate(), true);
    const C3DMeshInfo* mesh_info = primitive.MeshInfo;
    float radius = ((mesh_info->GetBoundingSphere().Radius + mesh_info->GetBoundingSphere().GetCenter().GetMagnitude()) * primitive.Scale) + NEngineInternal::GPPrimitiveRenderer->BoundingRadiusFix;
    C3DVector centre = mesh_info->GetBoundingSphere().GetCenter();
    transform.TransformPoint(centre);
    if (!NEngineInternal::GPShadowRenderer->PeekSceneData().CasterClipVolume.ClipSphere(centre, radius))
        return RENDERED_NOTHING;
    float distance = CalcDistanceFromCamera(centre);
    float mesh_radius = primitive.MeshInfo->GetBoundingSphere().GetRadius() * primitive.Scale;
    if (!primitive.SetupMesh(distance, mesh_radius))
        return RENDERED_NOTHING;

    CEngineRenderInfoAnimatedMesh* render_info = primitive.GetRenderInfo();
    FABLE_RETAIL_ASSERT(render_info);
    const C3DMesh2* mesh = render_info->Mesh;
    FABLE_RETAIL_ASSERT(mesh);
    render_info->ObjectTransform = transform;
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* surfaces = 0;
    const unsigned char* hidden_materials = 0;
    if (primitive.GetType() == 11) {
        hidden_materials = static_cast<CEngineInternalPrimitiveMorphedAnimatedMesh&>(primitive).Morph.GetHiddenMaterials(*mesh);
        surfaces = &static_cast<CEngineInternalPrimitiveMorphedAnimatedMesh&>(primitive).Morph.CompositeSurfaces;
    } else if (primitive.GetType() == 38) {
        hidden_materials = static_cast<CEngineInternalPrimitive2DMorphedAnimatedMesh&>(primitive).Morph.GetHiddenMaterials(*mesh);
        surfaces = &static_cast<CEngineInternalPrimitive2DMorphedAnimatedMesh&>(primitive).Morph.CompositeSurfaces;
    }

    const CGraphicDataBank* texture_bank = primitive.MeshInfo->PeekTextureBank();
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
    NEngineInternal::GPRenderManager->SetAlphaRef(128);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER_EQUAL);

    long diffuse_stage = 0;
    if (NEngineInternal::GPShadowRenderer->PeekRenderingTechnique() == CEngineShadowRenderer::SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER) {
        diffuse_stage = 1;
        NEngineInternal::GPRenderManager->AttachTextureToStage(0, &NEngineInternal::GPShadowRenderer->GetDepthPackingTexture());
        NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
        const float min_w_clamp = NEngineInternal::GPShadowRenderer->PeekColourBufferShadowMinWClamp();
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C4DVector(128.0f, min_w_clamp, 0.0f, 0.0f));
    }

    for (unsigned long i = 0; i < mesh->GetPrimitiveCount(); ++i) {
        C3DPrimitive2& mesh_primitive = *mesh->GetPrimitive(i);
        if (mesh_primitive.TriangleCount == 0)
            continue;
        const unsigned long material_index = mesh_primitive.GetMaterialIndex();
        const C3DMeshMaterial* material = mesh->GetMaterial(material_index);
        if (hidden_materials && hidden_materials[material_index])
            continue;
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*VertexShadersPalSkinShadowBufferRender, true);
        const bool transparent = material->Transparent;
        if (transparent) {
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(GetPixelShaderShadowBufferRenderAlpha());
        } else {
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(GetPixelShaderShadowBufferRender());
        }
        bool preloading = false;
        if (transparent) {
            if (material->GetDiffuseMapBankIndex() == 0)
                continue;
            unsigned long mipmap = GetAppropriateMipmap(GetCurrentCamera().PeekCameraPos(), transform, mesh_primitive, primitive.Scale, material, texture_bank);
            const CTexture* texture = GetDiffuseTexture(material, surfaces, mipmap, preloading, texture_bank, false);
            if (!preloading)
                NEngineInternal::GPRenderManager->AttachTextureToStage(diffuse_stage, texture);
        }
        NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*mesh_primitive.GetVertexBuffer());
        NEngineInternal::GPRenderManager->RealiseRenderState();
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0x401);
        if (!preloading)
            RenderPrimitiveBlocksIndexBuffer(mesh_primitive, primitive.GetBoneTransformations());
    }
    return RENDERED_OBJECT;
}
