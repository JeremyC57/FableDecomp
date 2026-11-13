// CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive(CEngineSubPrimitiveAnimatedMesh& sub_primitive, ERenderPassType pass) -- draw one animated (skinned) mesh primitive for a render pass inside a render-state capture block
// Grade: DIFFER(ours v retail) -- 11288v11288 (same size), 2309 of 3160 instructions equal, 315 hunks: frame 0xE4 v 0xE0 and slot / register residues; abort_render is a memory byte (retail tests failed [ebp+6Fh] directly); arms 6/7 APS tail not cross-jumped; reflection-colour copy scheduling (arms 7/10/13); decal loop SIB order
// Leaf: ?RenderMeshPrimitive@CEnginePrimitiveManagerAnimatedMeshesBase@@QAEXAAVCEngineSubPrimitiveAnimatedMesh@@W4ERenderPassType@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ALPHAFADE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_HELPERS_FORCEINLINE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_GETTEXTUREDECAL_FORCEINLINE
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_OCCLUSIONTEST
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_ANIMATEDMESH
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_HELPERS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ALPHABLENDSTATES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ANIMATEDMESH
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SILHOUETTES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SHADERS
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALSHADOWMODE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_FADEDSHADOW_MODE_CHECK
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_DEPTHTRANSFORM
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_GETWIDTH
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ISALPHA
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_DECAL_HEAP
#define FABLE_RETAIL_C3DVECTOR_NAMED_RESULT_SUB
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_XY_THEN_Z
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_SETCHANNEL
#define FABLE_RETAIL_CRGBCOLOUR_GETASINT
#define FABLE_RETAIL_CRGBFLOATCOLOUR_GETRGBCOLOUR_LOCALS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTORS
#define FABLE_RETAIL_C4DVECTOR_INITIALISEFROMCOLOUR
#define FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER
#define FABLE_RETAIL_C3DMESHMATERIAL_GETDECALID
#define FABLE_RETAIL_CRGBFLOATCOLOUR_GETCHANNEL
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_CSHADERRENDERMANAGER_ENABLED_INLINE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_ZERORGBATEXTURE
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CEngineInternalPrimitiveMeshBase.h"
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/CEngineInternalPrimitiveMorphInfo.h"
#include "engine/CEngineSubPrimitiveAnimatedMesh.h"
#include "engine/CEnginePrimitiveManagerOcclusionBase.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEnginePrimitiveManagerAnimatedMeshesBase.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitive2DViewportManager.h"
#include "engine/C2DAlphaMaskInfo.h"
#include "engine/CGraphicInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/C3DPrimitive2.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CEngineTextureDecal.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFastFloatToLong.h"
#include "engine/GFLimit.h"

namespace NEngineInternal {
extern CRenderManager* GPRenderManager;                                  // 0x01436E18
extern CEngineResourceManager* GPResourceManager;                        // 0x01436E98
extern CEngineOcclusionManager* GPOcclusionManager;                      // 0x01436E5C
}
extern CEnginePrimitiveRendererSharedData* g_pPrimitiveRendererSharedData; // 0x01436E78
extern CEngineLightingManager*             g_pEngineLightingManager;       // 0x01436E9C
extern CEngineCamera*                      g_pEngineCamera;                // 0x01436EA0
extern CEngineShadowRenderer*              g_pEngineShadowRenderer;        // 0x01436E60
extern CEnginePrimitiveTypeInfo*           g_pEnginePrimitiveTypeInfo;     // 0x01436E84
extern CEngineScreenEffectOutlineGlow*     g_pEngineScreenEffectOutlineGlow; // 0x01436E3C
extern unsigned char g_highestSetBitTable[256];                            // 0x0143B990
extern const float DELTA_NOTIONAL_ZERO;                                    // 0x0129BA3C

inline unsigned long FABLE_FASTCALL GFGetTopBitInByte(unsigned char value)
{
    return g_highestSetBitTable[value];
}
FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}
FABLE_FORCEINLINE void CRenderStateManager::SetTextureFactor(unsigned long factor) { SetRenderState(factor, DeviceState.TextureFactor); }
FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertexBuffer)
{
    D3DDevice->SetStreamSource(0, vertexBuffer.PVertexBuffer, 0, vertexBuffer.Stride);
}
FABLE_FORCEINLINE void CRenderManagerCore::FlushRender()
{
    SelectVertexBuffer(0, 0);
    PD3DDevice->SetIndices(0);
    for (long stage = 0; stage < TextureStagesInUse; ++stage) {
        if (CurrentlySelectedTextures[stage]) {
            CurrentlySelectedTextures[stage] = 0;
            PD3DDevice->SetTexture(stage, 0);
        }
    }
    TextureStagesInUse = 0;
}
inline void __fastcall CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(shader);
}
FABLE_FORCEINLINE void __fastcall CEnginePrimitiveRenderUtil::SetTextureFactor(const CRGBFloatColour& colour)
{
    if (NEngineInternal::GPShaderRenderManager->GetPixelShadersEnabled())
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(colour));
    else
        NEngineInternal::GPRenderManager->SetTextureFactor(colour.GetRGBColour().IntValue);
}
inline float __fastcall CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() { return g_pPrimitiveRendererSharedData->MipmapSizeMultiplier; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseOnly() { return g_pPrimitiveRendererSharedData->PixelShaderDiffuseOnly; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseColourTextureDiffuseAlpha() { return g_pPrimitiveRendererSharedData->PixelShaderDiffuseColourTextureDiffuseAlpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFill() { return g_pPrimitiveRendererSharedData->PixelShaderOutlineSmoothFill; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillAlpha() { return g_pPrimitiveRendererSharedData->PixelShaderOutlineSmoothFillAlpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillDP3() { return g_pPrimitiveRendererSharedData->PixelShaderOutlineSmoothFillDP3; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillDP3Alpha() { return g_pPrimitiveRendererSharedData->PixelShaderOutlineSmoothFillDP3Alpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuse() { return g_pPrimitiveRendererSharedData->PixelShaderDiffuse; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFog() { return g_pPrimitiveRendererSharedData->PixelShaderDiffuseFog; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseEnvFog() { return g_pPrimitiveRendererSharedData->PixelShaderDiffuseEnvFog; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadow() { return *g_pPrimitiveRendererSharedData->PixelShaderDiffuseFogShadow; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadowOutputFactor() { return *g_pPrimitiveRendererSharedData->PixelShaderDiffuseFogShadowOutputFactor; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseEnvFogShadow() { return *g_pPrimitiveRendererSharedData->PixelShaderDiffuseEnvFogShadow; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseDP3BumpFogShadow() { return *g_pPrimitiveRendererSharedData->PixelShaderDiffuseDP3BumpFogShadow; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseDP3BumpFogShadowOutputFactor() { return *g_pPrimitiveRendererSharedData->PixelShaderDiffuseDP3BumpFogShadowOutputFactor; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuse(EEnginePrimitiveBlendMode mode)
{
    if (mode == ENGINE_PRIMITIVE_BLEND_ADDITIVE)
        return g_pPrimitiveRendererSharedData->PixelShader2DDiffuseAdditiveAlpha;
    return g_pPrimitiveRendererSharedData->PixelShader2DDiffuse;
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuseEnv(EEnginePrimitiveBlendMode mode)
{
    if (mode == ENGINE_PRIMITIVE_BLEND_ADDITIVE)
        return g_pPrimitiveRendererSharedData->PixelShader2DDiffuseEnvAdditiveAlpha;
    return g_pPrimitiveRendererSharedData->PixelShader2DDiffuseEnv;
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuseDP3Bump(EEnginePrimitiveBlendMode mode)
{
    if (mode == ENGINE_PRIMITIVE_BLEND_ADDITIVE)
        return g_pPrimitiveRendererSharedData->PixelShader2DDiffuseDP3BumpAdditiveAlpha;
    return g_pPrimitiveRendererSharedData->PixelShader2DDiffuseDP3Bump;
}
inline void __fastcall CEnginePrimitiveRenderUtil::SetWorldTransform(const CMatrix3x4& transform)
{
    if (NEngineInternal::GPShaderRenderManager->IsFixedFunctionVertexMode())
        NEngineInternal::GPRenderManager->SetWorldTransform(transform);
    else
        NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
}
inline const C2DAlphaMaskInfo* CEnginePrimitive2DViewportManager::GetCurrentAlphaMask() { return &AlphaMaskInfo; }
inline const C2DAlphaMaskInfo* CEnginePrimitiveRenderUtil::GetCurrent2DAlphaMaskInfo()
{
    return ((CEnginePrimitive2DViewportManager*)g_pEnginePrimitiveTypeInfo->GetPrimitiveManager(33))->GetCurrentAlphaMask();
}
inline float CEngineSubPrimitiveMeshBase::GetScale() const { return PeekParent()->Scale; }
inline const CMatrix3x4& CEngineSubPrimitiveAnimatedMesh::PeekObjectTransform() const { return PeekParent()->PeekRenderInfo()->ObjectTransform; }
inline void CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocks(C3DPrimitive2& primitive, const CMatrix4x4* bone_transforms)
{
    RenderPrimitiveBlocksIndexBuffer(primitive, bone_transforms);
}
FABLE_FORCEINLINE unsigned char CEngineInternalPrimitiveBase::GetAlphaIncludingDistanceFade(float distance)
{
    if (Alpha == 0)
        return 0;
    CEngineFadeDistance fade = OverrideFadeDistance ? GetFadeDistance() : FadeDistance;
    if (distance > fade.MaxDrawDistance)
        return 0;
    if (distance < fade.FadeBeginDistance)
        return Alpha;
    {
        int fadeAmount;
        fadeAmount = GFFastFloatToLong((distance - fade.FadeBeginDistance) * 255.0f / (fade.MaxDrawDistance - fade.FadeBeginDistance));
        return (unsigned char)(((255 - fadeAmount) * Alpha) >> 8);
    }
}
FABLE_FORCEINLINE unsigned long FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(const C3DVector& camera_position,
    const CMatrix3x4& transform, const C3DPrimitive2& mesh_primitive, float scale, const C3DMeshMaterial* material,
    const CGraphicDataBank* graphic_bank)
{
    C3DBoundingSphere sphere = mesh_primitive.GetBoundingSphere();
    transform.TransformPoint(sphere.Center);
    unsigned long mipmap = 0;
    float camera_distance = (camera_position - sphere.Center).GetMagnitude() - scale * sphere.Radius;
    float distance = mesh_primitive.GetAverageTextureStretch() * camera_distance;

    if (distance > DELTA_NOTIONAL_ZERO) {
        long graphic_index = material->GetDiffuseMapBankIndex();
        if (graphic_index > 0) {
            const CGraphicInfo* graphic = graphic_bank->PeekGraphic(graphic_index);
            unsigned long width = graphic->GetWidth();
            float texel_size = CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() / distance * scale;
            float ratio = (float)width / texel_size;
            long size = GFFloatToLongFloor(ratio);
            GFLimit(size, 1, 255);
            mipmap = GFGetTopBitInByte((unsigned char)size);
        }
    }
    return mipmap;
}

FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromRenderFlags(unsigned long render_flags, ERenderPassType pass,
    unsigned char alpha, unsigned char alpha_ref)
{
    if (pass == RENDER_PASS_Z_WRITE) {
        NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
        NEngineInternal::GPRenderManager->SetAlphaRef(alpha_ref);
        NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
    } else {
        if (render_flags & 2) {
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
            NEngineInternal::GPRenderManager->SetAlphaRef((alpha * alpha_ref) >> 8);
        } else {
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
            NEngineInternal::GPRenderManager->SetAlphaRef(0);
        }
        if (render_flags & 1) {
            NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
            NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
            NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
            NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
        } else {
            NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
            NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        }
    }
}
void CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive(CEngineSubPrimitiveAnimatedMesh& sub_primitive, ERenderPassType pass)
{
    CRenderStateScopedCaptureBlock capture_block;
    NEngineInternal::GPRenderManager->SetSpecularEnable(true);
    NEngineInternal::GPRenderManager->SetFogEnable(true);
    CEngineInternalPrimitiveAnimatedMesh* primitive = sub_primitive.GetParent();
    C3DPrimitive2* mesh_primitive = sub_primitive.MeshPrimitive;
    const C3DMesh2& mesh = primitive->PeekMesh();
    const C3DMeshMaterial* material = mesh.GetMaterial(mesh_primitive->GetMaterialIndex());
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces = 0;
    CEngineTextureDecal* decal = 0;
    CEngineInternalPrimitiveMorphInfo* morph_info = primitive->GetMorphInfo();
    if (morph_info) {
        composite_surfaces = &morph_info->CompositeSurfaces;
        decal = morph_info->GetTextureDecal(material->GetDiffuseMapBankIndex(), material->GetDecalID());
    }
    const CGraphicDataBank* graphic_bank = primitive->MeshInfo->PeekTextureBank();
    const CTexture* textures[4];
    unsigned long num_textures = 0;
    unsigned long mipmap_level = CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(g_pEngineCamera->PeekCameraPos(), sub_primitive.PeekObjectTransform(),
                                                                                       *mesh_primitive, sub_primitive.GetScale(), material, graphic_bank);
    bool force_load = (primitive->EnginePrimitiveFlags >> 7) & 1;
    unsigned long render_flags = sub_primitive.PeekRenderFlags();
    bool failed = false;
    bool environment_mapped = false;
    SetupAlphaBlendingStatesFromRenderFlags(render_flags, pass, sub_primitive.PeekAlpha(), sub_primitive.GetParent()->AlphaRef);
    if (render_flags & 0x80)
        SetupStencilMask(primitive->MeshEffect.Get(), render_flags, pass);
    CRGBColour tint = primitive->TintColour;
    bool use_output_factor = (tint.GetAsInt() & 0xffffff) != 0xffffff;
    if (decal)
        use_output_factor = true;
    bool abort_render = false;
    if (pass == RENDER_PASS_COMBINED) {
        g_pEngineLightingManager->SetCurrentEnvironmentThemeChannel(primitive->ThemeChannelDesc);
        const CEngineLocalLightArray& lights = sub_primitive.GetParent()->PeekLights();
        switch (sub_primitive.GetRenderMode()) {
        case 2:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShader(lights.LightCount, 0);
            ApplyPixelShader(GetPixelShaderDiffuse());
            break;
        case 3:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShaderSpotLight(lights.LightCount);
            ApplyPixelShader(GetPixelShaderDiffuse());
            g_pEngineLightingManager->SetupShaderSpotLight();
            break;
        case 4:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShader(lights.LightCount, 0);
            ApplyPixelShader(GetPixelShaderDiffuseFog());
            g_pEngineLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 8:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShader(lights.LightCount, 0);
            ApplyPixelShader(GetPixelShaderDiffuseFog());
            g_pEngineLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 6:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = NEngineInternal::GPResourceManager->GetZeroRGBATexture();
            textures[2] = GetReflectionTexture(material, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 3;
            environment_mapped = true;
            ApplyPalSkinVertexShaderEnv(lights.LightCount);
            ApplyPixelShader(GetPixelShaderDiffuseEnvFog());
            g_pEngineLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 7:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = GetReflectionTexture(material, mipmap_level, failed, graphic_bank, force_load);
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(g_pEngineLightingManager->PeekPrimitiveReflectionColour()));
            num_textures = 2;
            environment_mapped = true;
            ApplyPalSkinVertexShaderEnv(lights.LightCount);
            ApplyPixelShader(GetPixelShaderDiffuseEnvFog());
            g_pEngineLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 9:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShaderShadow(lights.LightCount, 0);
            if (use_output_factor)
                ApplyPixelShader(GetPixelShaderDiffuseFogShadowOutputFactor());
            else
                ApplyPixelShader(GetPixelShaderDiffuseFogShadow());
            g_pEngineShadowRenderer->SetupShadowFuncRenderStates(1, primitive->PeekRenderInfo()->ObjectTransform,
                                                                 CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            break;
        case 10:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = GetReflectionTexture(material, mipmap_level, failed, graphic_bank, force_load);
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(g_pEngineLightingManager->PeekPrimitiveReflectionColour()));
            num_textures = 2;
            environment_mapped = true;
            ApplyPalSkinVertexShaderEnvShadow(lights.LightCount);
            ApplyPixelShader(GetPixelShaderDiffuseEnvFogShadow());
            g_pEngineShadowRenderer->SetupShadowFuncRenderStates(2, primitive->PeekRenderInfo()->ObjectTransform,
                                                                 CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            break;
        case 11:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = GetBumpTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 2;
            ApplyPalSkinVertexShaderDP3BumpShadow(lights.LightCount);
            if (use_output_factor)
                ApplyPixelShader(GetPixelShaderDiffuseDP3BumpFogShadowOutputFactor());
            else
                ApplyPixelShader(GetPixelShaderDiffuseDP3BumpFogShadow());
            g_pEngineShadowRenderer->SetupShadowFuncRenderStates(3, primitive->PeekRenderInfo()->ObjectTransform,
                                                                 CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            break;
        case 12:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShaderSpotShadow(lights.LightCount, 0);
            if (use_output_factor)
                ApplyPixelShader(GetPixelShaderDiffuseFogShadowOutputFactor());
            else
                ApplyPixelShader(GetPixelShaderDiffuseFogShadow());
            g_pEngineShadowRenderer->SetupShadowFuncRenderStates(1, primitive->PeekRenderInfo()->ObjectTransform,
                                                                 CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            g_pEngineLightingManager->SetupShaderSpotLight(primitive->PeekRenderInfo()->ObjectTransform);
            break;
        case 13:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = GetReflectionTexture(material, mipmap_level, failed, graphic_bank, force_load);
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(g_pEngineLightingManager->PeekPrimitiveReflectionColour()));
            num_textures = 2;
            environment_mapped = true;
            ApplyPalSkinVertexShaderEnvSpotShadow(lights.LightCount);
            ApplyPixelShader(GetPixelShaderDiffuseEnvFogShadow());
            g_pEngineShadowRenderer->SetupShadowFuncRenderStates(2, primitive->PeekRenderInfo()->ObjectTransform,
                                                                 CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            g_pEngineLightingManager->SetupShaderSpotLight(primitive->PeekRenderInfo()->ObjectTransform);
            break;
        case 1:
            ApplyPalSkinVertexShader(0, 0);
            ApplyPixelShader(GetPixelShaderDiffuseOnly());
            break;
        case 16: {
            const C2DAlphaMaskInfo* mask_info = GetCurrent2DAlphaMaskInfo();
            textures[0] = GetDiffuseTexture(material, composite_surfaces, 0, failed, graphic_bank, force_load);
            textures[1] = Get2DAlphaMaskTexture(mask_info->AlphaMaskBankIndex, 0, failed, mask_info->AlphaMaskBank, force_load);
            num_textures = 2;
            if (!mask_info->FilterMask) {
                NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_POINT);
                NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT);
                NEngineInternal::GPRenderManager->SetTextureMipMapMode(1, TEXTURE_MIPMAP_DISABLE);
            }
            NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
            Apply2DPalSkinVertexShader();
            ApplyPixelShader(GetPixelShader2DDiffuse(ENGINE_PRIMITIVE_BLEND_SOLID));
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask_info->AlphaMaskPositionFactors.X, mask_info->AlphaMaskPositionFactors.Y, mask_info->AlphaMaskPositionFactors.Z), mask_info->AlphaMaskPositionFactors.W);
            SetTextureFactor(CRGBFloatColour(1.0f, 1.0f, 1.0f, sub_primitive.GetAlphaAsFloat()));
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        case 17: {
            const C2DAlphaMaskInfo* mask_info = GetCurrent2DAlphaMaskInfo();
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = Get2DAlphaMaskTexture(mask_info->AlphaMaskBankIndex, 0, failed, mask_info->AlphaMaskBank, force_load);
            textures[2] = GetReflectionTexture(material, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 3;
            if (!mask_info->FilterMask) {
                NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_POINT);
                NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT);
                NEngineInternal::GPRenderManager->SetTextureMipMapMode(1, TEXTURE_MIPMAP_DISABLE);
            }
            NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(g_pEngineLightingManager->PeekPrimitiveReflectionColour()));
            environment_mapped = true;
            Apply2DPalSkinVertexShaderEnv();
            ApplyPixelShader(GetPixelShader2DDiffuseEnv(ENGINE_PRIMITIVE_BLEND_SOLID));
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask_info->AlphaMaskPositionFactors.X, mask_info->AlphaMaskPositionFactors.Y, mask_info->AlphaMaskPositionFactors.Z), mask_info->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        case 18: {
            const C2DAlphaMaskInfo* mask_info = GetCurrent2DAlphaMaskInfo();
            textures[0] = GetDiffuseTexture(material, composite_surfaces, 0, failed, graphic_bank, force_load);
            textures[1] = GetBumpTexture(material, composite_surfaces, 0, failed, graphic_bank, force_load);
            textures[2] = textures[3] = Get2DAlphaMaskTexture(mask_info->AlphaMaskBankIndex, 0, failed, mask_info->AlphaMaskBank, force_load);
            num_textures = 4;
            if (!mask_info->FilterMask) {
                NEngineInternal::GPRenderManager->SetTextureFilterModeMag(2, TEXTURE_FILTER_POINT);
                NEngineInternal::GPRenderManager->SetTextureFilterModeMin(2, TEXTURE_FILTER_POINT);
                NEngineInternal::GPRenderManager->SetTextureMipMapMode(2, TEXTURE_MIPMAP_DISABLE);
            }
            NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_CLAMP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_CLAMP);
            Apply2DPalSkinVertexShaderDP3Bump();
            ApplyPixelShader(GetPixelShader2DDiffuseDP3Bump(ENGINE_PRIMITIVE_BLEND_SOLID));
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask_info->AlphaMaskPositionFactors.X, mask_info->AlphaMaskPositionFactors.Y, mask_info->AlphaMaskPositionFactors.Z), mask_info->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        default:
            __assume(0);
        }
        g_pEngineLightingManager->SetSelfIlluminationAmbient(material->SelfIllumination);
        g_pEngineLightingManager->UpdateGlobalLightingStates();
        g_pEngineLightingManager->SetupShaderPointLights(lights);
        abort_render = failed;
        tint.SetA(sub_primitive.PeekAlpha());
        SetTextureFactor(CRGBFloatColour(tint));
    } else if (pass == RENDER_PASS_Z_WRITE) {
        num_textures = 0;
        if (material->GetDiffuseMapBankIndex() != 0 && graphic_bank->PeekGraphicInfoFrame(material->GetDiffuseMapBankIndex(), 0)->IsAlpha()) {
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, false);
            num_textures = 1;
        }
        ApplyPalSkinVertexShader(0, 0);
        ApplyPixelShader(GetPixelShaderDiffuseFog());
        abort_render = failed;
        SetTextureFactor(CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f));
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_STENCIL) {
        SetupOutlineSmoothStencilStates(*(const CEngineMeshEffectOutline*)primitive->MeshEffect.Get(), primitive->Scale);
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderPalSkinDP3BumpOutline : VertexShaderPalSkinOutline);
        ApplyPixelShader(GetPixelShaderDiffuseOnly());
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_ALPHA_STENCIL_MASK) {
        SetupOutlineSmoothAlphaStencilMaskStates(*(const CEngineMeshEffectOutline*)primitive->MeshEffect.Get());
        num_textures = 0;
        if (material->GetDiffuseMapBankIndex() != 0) {
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, false);
            num_textures = 1;
        }
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderPalSkinDP3BumpOutline : VertexShaderPalSkinOutline);
        ApplyPixelShader(GetPixelShaderDiffuseColourTextureDiffuseAlpha());
        abort_render = failed;
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_FILL) {
        num_textures = 0;
        const CEngineMeshEffectOutline& effect = *(const CEngineMeshEffectOutline*)primitive->MeshEffect.Get();
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderPalSkinDP3BumpOutlineSmoothFill : VertexShaderPalSkinOutlineSmoothFill);
        g_pEngineScreenEffectOutlineGlow->SetupDepthBufferTransform(primitive->PeekRenderInfo()->ObjectTransform);
        long num_stages = mesh_primitive->IsDP3Bump() ? 2 : 1;
        if (material->GetDiffuseMapBankIndex() != 0 && material->Transparent) {
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, false);
            num_textures = 1;
            ApplyPixelShader(mesh_primitive->IsDP3Bump() ? GetPixelShaderOutlineSmoothFillDP3Alpha() : GetPixelShaderOutlineSmoothFillAlpha());
            SetupOutlineSmoothFillStates(effect, primitive->Scale, true, num_stages);
            abort_render = failed;
        } else {
            ApplyPixelShader(mesh_primitive->IsDP3Bump() ? GetPixelShaderOutlineSmoothFillDP3() : GetPixelShaderOutlineSmoothFill());
            SetupOutlineSmoothFillStates(effect, primitive->Scale, false, num_stages);
        }
    } else if (pass == RENDER_PASS_OUTLINE_STENCIL) {
        SetupOutlineStencilStates(*(const CEngineMeshEffectOutline*)primitive->MeshEffect.Get(), primitive->Scale);
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderPalSkinDP3BumpOutline : VertexShaderPalSkinOutline);
        ApplyPixelShader(GetPixelShaderDiffuseOnly());
    } else if (pass == RENDER_PASS_OUTLINE_FILL) {
        SetupOutlineFillStates(*(const CEngineMeshEffectOutline*)primitive->MeshEffect.Get(), primitive->Scale);
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderPalSkinDP3BumpOutline : VertexShaderPalSkinOutline);
        ApplyPixelShader(GetPixelShaderDiffuseOnly());
    }
    if (abort_render) {
        NEngineInternal::GPRenderManager->FlushRender();
        g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
        return;
    }
    unsigned long occlusion_handle;
    PrepareOcclusionTest(*primitive, pass, occlusion_handle);
    if (occlusion_handle != (unsigned long)-1)
        NEngineInternal::GPOcclusionManager->StartOcclusionTest();
    CEnginePrimitiveRenderUtil::SetWorldTransform(sub_primitive.PeekObjectTransform());
    NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*mesh_primitive->GetVertexBuffer());
    unsigned long shader_flags = 0x10b;
    if (environment_mapped)
        shader_flags = 0x12b;
    NEngineInternal::GPRenderManager->RealiseRenderState();
    NEngineInternal::GPShaderRenderManager->PrepareForRender(shader_flags);
    const CMatrix4x4* bone_transforms = sub_primitive.GetParent()->GetBoneTransformations();
    for (unsigned long i = 0; i < num_textures; ++i)
        NEngineInternal::GPRenderManager->AttachTextureToStage(i, textures[i]);
    RenderPrimitiveBlocks(*mesh_primitive, bone_transforms);
    if (occlusion_handle != (unsigned long)-1)
        NEngineInternal::GPOcclusionManager->EndOcclusionTest(occlusion_handle);
    if (decal) {
        long overlay_alpha = primitive->GetAlphaIncludingDistanceFade(primitive->PeekRenderInfo()->Distance * TextureOverlayFadeDistandeFactor);
        if (overlay_alpha > 0) {
            CArray<const CTexture*> overlaid_textures;
            if (GetOverlaidTextures(*decal, overlaid_textures, mipmap_level, graphic_bank)) {
                long num_overlays = decal->OverlaidBlends.size();
                for (long i = 0; i < num_overlays; ++i) {
                    SetupOverlayAlphaBlendingStates(overlay_alpha, decal->OverlaidBlends[i].DiffuseColour);
                    NEngineInternal::GPRenderManager->AttachTextureToStage(0, overlaid_textures[i]);
                    NEngineInternal::GPRenderManager->RealiseRenderState();
                    NEngineInternal::GPShaderRenderManager->PrepareForRender(shader_flags);
                    RenderPrimitiveBlocks(*mesh_primitive, bone_transforms);
                }
            }
        }
    }
    for (unsigned long i = 0; i < num_textures; ++i)
        NEngineInternal::GPRenderManager->AttachTextureToStage(i, 0);
    g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
    NEngineInternal::GPRenderManager->FlushRender();
}
