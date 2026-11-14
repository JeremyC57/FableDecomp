// CEnginePrimitiveManagerStaticMeshesBase::SetupRenderModeShadersAndConstants(ERenderPassType pass, const C3DMeshMaterial* material, CEngineSubPrimitiveStaticMesh& sub_primitive, C3DTextureStages& texture_stages, long mipmap_level, unsigned long* shader_flags, bool* render_degenerate_triangles) -- pick and apply the vertex / pixel shaders, shader constants, extra texture stages and pass-specific render states for one static-mesh sub-primitive; always returns true
// Grade: DIFFER(9632v9632) -- wfB3: retail size, 2401 of 2401 instructions aligned, natural retail register colouring (mesh_primitive edi, sub_primitive ebx, this ebp + [esp+14h]). Residue: (1) frame: every slot between +10h and +3Ch is 4 bytes off because the eight used_blank share the slot of the first __ftol2 byte temporary instead of the slot of tinted (retail +10h = tinted + G byte temp + used_blank, byte temp alone at +40h); (2) bookmark test/or operand order or mask/flags load order at 13 of the 34 inline SetRenderState expansions (0x00BA253D, 0x00BA32BC, 0x00BA332C, 0x00BA365A..0x00BA3812, 0x00BA413C, 0x00BA46D2..0x00BA483D). See NOTES.md.
// wfB3 statement-shape facts (each EVIDENCED by retail bytes, see NOTES.md): the last two passes are two arms with the whole body duplicated (0x00BA45CF..0x00BA4672);
//   cases 16/17/18 fetch `mask` before the texture_stages stores (0x00BA3214 / 0x00BA3555 / 0x00BA399A); case 5 copies the camera window by value (BRX at +8 of a 16-byte slot,
//   0x00BA275E) and passes a named filter-mode local to the two filter setters (0x00BA27BD..0x00BA2840; name `filter_mode` made up); CArray::operator[] through begin()
//   (existing opt-in FABLE_RETAIL_CARRAY; 0x00BA248D `mov ecx,eax`). FITTED (grading only): keeping the four named floats left/top/right/bottom next to the by-value window.
// Retail: Fable.exe 0x00BA2350, 9632 bytes (+ 18-entry jump table 0x00BA48A8, entry 14 = 0), private thiscall, `ret 1Ch`, esp frame sub esp,0B0h.
// Callees: CShaderRenderManager::ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 / SetUserVertexShaderConstant(KMMMM) 0x00989B00 /
//   (K,C3DVector,M) 0x00989A60; CEngineLightingManager::SetupShaderPointLights 0x00B44F20 / UpdateGlobalLightingStates 0x00B46C80 /
//   SetupShaderSpotLight(CMatrix3x4) 0x00B47390; CEngineShadowRenderer::SetupShadowFuncRenderStates 0x00B4FC40; CEngineScreenEffectOutlineGlow::
//   SetupDepthBufferTransform 0x00B89070; CEnginePrimitiveManagerMeshBase::GetReflectionTexture 0x00BA9830 / Get2DAlphaMaskTexture 0x00BA9A70;
//   CEnginePrimitiveRenderUtil::SetupOutlineSmoothStencilStates 0x00BEFD90 / SetupOutlineSmoothAlphaStencilMaskStates 0x00BF01E0 /
//   SetupOutlineStencilStates 0x00BF0740 / SetupOutlineFillStates 0x00BF0B20 / SetupOutlineSmoothFillStates 0x00BF2240; __ftol2 0x00BFEA70;
//   IDirect3DDevice9::SetTexture (+0x104, inline AttachTextureToStage).
// Inline (FableWin.pdb names, bodies here): ApplyStaticVertexShader* / Apply2DStaticVertexShader* / ApplyVertexShader (members),
//   CEnginePrimitiveRenderUtil::SetTextureFactor / ApplyPixelShader / SetupBlankTextures / DisableTextureFiltering / Set2DAlphaMaskTextureMode /
//   GetCurrent2DAlphaMaskInfo; header opt-ins in hdr/README.md.
// Leaf: ?SetupRenderModeShadersAndConstants@CEnginePrimitiveManagerStaticMeshesBase@@AAE_NW4ERenderPassType@@PBVC3DMeshMaterial@@AAVCEngineSubPrimitiveStaticMesh@@AAVC3DTextureStages@@JPAKPA_N@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_C3DPRIMITIVE2_ISDP3BUMP
#define FABLE_RETAIL_C3DPRIMITIVE2_ISDP3BUMP_MASK
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKRENDERINFO
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_RENDERMESHPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_SETUPRENDERMODE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_REFLECTIONTEXTURE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_2DALPHAMASK
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHSHADERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_STATICMESHSHADERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTH2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETUPBLANKTEXTURES
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVESTATICMESH_PARENTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKLIGHTS
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD_DIRECT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_PEEKWINDOW
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SETUPRENDERMODE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC
#define FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC_CTOR
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_DEPTHBUFFERTRANSFORM
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETSTIPLEFADETEXTURE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_ZERORGBATEXTURE
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER_ALPHAMASK
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#include "engine/CEngineSubPrimitiveStaticMesh.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DTextureStages.h"
#include "engine/CTexture.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C4DVector.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitive2DViewportManager.h"
#include "engine/C2DAlphaMaskInfo.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/GFDebugPDynamicCast.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                          // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }              // 0x01436E14
namespace NEngineInternal { extern CEngineScreenEffectOutlineGlow* GPScreenEffectOutlineGlow; } // 0x01436E3C
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                                                        // 0x01436EA0
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }   // 0x01436E78
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }                                      // 0x01436E9C
namespace NEngineInternal { extern CEngineShadowRenderer* GPShadowRenderer; }                                        // 0x01436E60
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }                                            // 0x01436E98
namespace NEngineInternal { extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; }                                  // 0x01436E84

FABLE_FORCEINLINE bool CShaderRenderManager::GetPixelShadersEnabled() const { return PixelShadersEnabled; }
FABLE_FORCEINLINE void CRenderStateManager::SetTextureFactor(unsigned long factor) { SetRenderState(factor, DeviceState.TextureFactor); }
FABLE_FORCEINLINE void __fastcall CEnginePrimitiveRenderUtil::SetTextureFactor(const CRGBFloatColour& colour)
{
    if (NEngineInternal::GPShaderRenderManager->GetPixelShadersEnabled())
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(colour));
    else
        NEngineInternal::GPRenderManager->SetTextureFactor(colour.GetRGBColour().IntValue);
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuse(EEnginePrimitiveBlendMode blend_mode)
{
    if (blend_mode == ENGINE_PRIMITIVE_BLEND_ADDITIVE || blend_mode == ENGINE_PRIMITIVE_BLEND_ADDSMOOTH)
        return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseAdditiveAlpha;
    return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuse;
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuseDP3Bump(EEnginePrimitiveBlendMode blend_mode)
{
    if (blend_mode == ENGINE_PRIMITIVE_BLEND_ADDITIVE || blend_mode == ENGINE_PRIMITIVE_BLEND_ADDSMOOTH)
        return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseDP3BumpAdditiveAlpha;
    return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseDP3Bump;
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuseEnv(EEnginePrimitiveBlendMode blend_mode)
{
    if (blend_mode == ENGINE_PRIMITIVE_BLEND_ADDITIVE || blend_mode == ENGINE_PRIMITIVE_BLEND_ADDSMOOTH)
        return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseEnvAdditiveAlpha;
    return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseEnv;
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuseOnly(EEnginePrimitiveBlendMode blend_mode)
{
    if (blend_mode == ENGINE_PRIMITIVE_BLEND_ADDITIVE || blend_mode == ENGINE_PRIMITIVE_BLEND_ADDSMOOTH)
        return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseOnlyAdditiveAlpha;
    return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseOnly;
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuse() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuse; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseColourTextureDiffuseAlpha() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseColourTextureDiffuseAlpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseDP3BumpFogShadow() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseDP3BumpFogShadow; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseDP3BumpFogShadowOutputFactor() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseDP3BumpFogShadowOutputFactor; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseEnvFog() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseEnvFog; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseEnvFogShadow() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseEnvFogShadow; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFog() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseFog; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadow() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseFogShadow; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadowOutputFactor() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseFogShadowOutputFactor; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseOnly() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseOnly; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFill() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderOutlineSmoothFill; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillAlpha() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderOutlineSmoothFillAlpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillDP3() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderOutlineSmoothFillDP3; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillDP3Alpha() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderOutlineSmoothFillDP3Alpha; }
FABLE_FORCEINLINE void __fastcall CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(shader);
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::SetupBlankTextures(C3DTextureStages& texture_stages, bool dp3_bump)
{
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, 0);
    texture_stages.UseBlankTextures = true;
    texture_stages.DiffuseTextureStage = 0;
    if (dp3_bump)
        texture_stages.BumpTextureStage = 1;
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::DisableTextureFiltering(unsigned long stage)
{
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(stage, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(stage, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureMipMapMode(stage, TEXTURE_MIPMAP_DISABLE);
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::Set2DAlphaMaskTextureMode(unsigned long stage, bool filter, bool clamp)
{
    if (!filter)
        DisableTextureFiltering(stage);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(stage, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(stage, TEXTURE_UV_CLAMP);
}
FABLE_FORCEINLINE const C2DAlphaMaskInfo* CEnginePrimitiveRenderUtil::GetCurrent2DAlphaMaskInfo()
{
    CEnginePrimitive2DViewportManager* manager = GFDebugPDynamicCast(NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(ENGINE_PRIMITIVE_2D_VIEWPORT),
                                                                      (CEnginePrimitive2DViewportManager*)0);
    return manager->GetCurrentAlphaMask();
}
FABLE_FORCEINLINE void CEnginePrimitiveManagerStaticMeshesBase::ApplyVertexShader(const CVertexShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, true);
}
#define FABLE_STATIC_VS_HELPER(name, expr) \
FABLE_FORCEINLINE void CEnginePrimitiveManagerStaticMeshesBase::name(unsigned long light_count) \
{ unsigned long index = light_count; if (index > 5) index = 5; ApplyVertexShader(expr[PointLightShaderIndex[index]]); }
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShader, VertexShadersStatic)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderStippleAlpha, VertexShadersStaticStippleAlpha)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderEnv, VertexShadersStaticEnv)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderShadow, VertexShadersStaticShadow)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderEnvShadow, VertexShadersStaticEnvShadow)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderDP3BumpShadow, VertexShadersStaticDP3BumpShadow)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderSpotLight, VertexShadersStaticSpotlight)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderSpotShadow, VertexShadersStaticSpotShadow)
FABLE_STATIC_VS_HELPER(ApplyStaticVertexShaderEnvSpotShadow, VertexShadersStaticEnvSpotShadow)
FABLE_FORCEINLINE void CEnginePrimitiveManagerStaticMeshesBase::Apply2DStaticVertexShader() { ApplyVertexShader(VertexShaders2DStatic); }
FABLE_FORCEINLINE void CEnginePrimitiveManagerStaticMeshesBase::Apply2DStaticVertexShaderEnv() { ApplyVertexShader(VertexShaders2DStaticEnv); }
FABLE_FORCEINLINE void CEnginePrimitiveManagerStaticMeshesBase::Apply2DStaticVertexShaderDP3Bump() { ApplyVertexShader(VertexShaders2DStaticDP3Bump); }

bool CEnginePrimitiveManagerStaticMeshesBase::SetupRenderModeShadersAndConstants(ERenderPassType pass, const C3DMeshMaterial* material,
                                                                                 CEngineSubPrimitiveStaticMesh& sub_primitive,
                                                                                 C3DTextureStages& texture_stages, long mipmap_level,
                                                                                 unsigned long* shader_flags, bool* render_degenerate_triangles)
{
    CEngineInternalPrimitiveStaticMesh* primitive = sub_primitive.GetParent();
    const CGraphicDataBank* graphic_bank = primitive->MeshInfo->PeekTextureBank();
    bool block_on_load = (primitive->EnginePrimitiveFlags >> 7) & 1;
    CRGBColour tint_colour = primitive->TintColour;
    bool tinted = (tint_colour.IntValue & 0xffffff) != 0xffffff;
    C3DPrimitive2* mesh_primitive = sub_primitive.MeshPrimitive;
    unsigned long flags = 0x109;
    if (pass == RENDER_PASS_COMBINED) {
        long render_mode = sub_primitive.GetRenderMode();
        unsigned char alpha;
        if (sub_primitive.PeekRenderFlags() & 8) {
            render_mode = 5;
            alpha = sub_primitive.PeekAlpha();
            if (!material->Transparent) {
                long alpha_value = alpha;
                long solid_alpha = StippleAlphaSolidFactor * alpha_value;
                if (solid_alpha >= 255)
                    alpha = 255;
                else
                    alpha = (unsigned char)solid_alpha;
            }
        } else
            alpha = sub_primitive.PeekAlpha();
        tint_colour.A = alpha;
        CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(tint_colour));
        NEngineInternal::GPLightingManager->SetCurrentEnvironmentThemeChannel(primitive->ThemeChannelDesc);
        const CEngineLocalLightArray& lights = sub_primitive.GetParent()->PeekLights();
        switch (render_mode) {
        case 2:
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShader(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuse());
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 3:
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShaderSpotLight(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuse());
            NEngineInternal::GPLightingManager->SetupShaderSpotLight(primitive->ObjectTransform);
            break;
        case 4:
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShader(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFog());
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 5: {
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShaderStippleAlpha(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(PixelShaderDiffuseFogStippleAlpha);
            C2DBoxF window = NEngineInternal::GPCamera->PeekWindow();
            float left = window.TLX;
            float top = window.TLY;
            float right = window.BRX;
            float bottom = window.BRY;
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, (right - left) * 0.015625f, (bottom - top) * -0.015625f,
                                                                                (right + left) * 0.015625f + 0.015625,
                                                                                (bottom + top) * 0.015625f + 0.015625);
            TEXTURE_FILTER_MODE filter_mode = TEXTURE_FILTER_POINT;
            NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, filter_mode);
            NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, filter_mode);
            NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, &NEngineInternal::GPResourceManager->GetStipleFadeTexture());
            NEngineInternal::GPRenderManager->SetTextureTransformMode(1, TEXTURE_TRANSFORM_PROJECTED);
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        }
        case 6: {
            texture_stages.DiffuseTextureStage = 0;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, NEngineInternal::GPResourceManager->GetZeroRGBATexture());
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(2, GetReflectionTexture(material, mipmap_level, used_blank, graphic_bank, block_on_load));
            flags = 0x129;
            ApplyStaticVertexShaderEnv(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseEnvFog());
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        }
        case 7: {
            texture_stages.DiffuseTextureStage = 0;
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, GetReflectionTexture(material, mipmap_level, used_blank, graphic_bank, block_on_load));
            const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            flags = 0x129;
            ApplyStaticVertexShaderEnv(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseEnvFog());
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        }
        case 8:
            texture_stages.DiffuseTextureStage = 0;
            texture_stages.BumpTextureStage = 1;
            ApplyStaticVertexShader(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFog());
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 9:
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShaderShadow(lights.GetCount());
            if (tinted)
                CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadowOutputFactor());
            else
                CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadow());
            NEngineInternal::GPShadowRenderer->SetupShadowFuncRenderStates(1, primitive->ObjectTransform, CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            flags = 0x10b;
            break;
        case 10: {
            texture_stages.DiffuseTextureStage = 0;
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, GetReflectionTexture(material, mipmap_level, used_blank, graphic_bank, block_on_load));
            const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            flags = 0x12b;
            ApplyStaticVertexShaderEnvShadow(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseEnvFogShadow());
            NEngineInternal::GPShadowRenderer->SetupShadowFuncRenderStates(2, primitive->ObjectTransform, CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            break;
        }
        case 11:
            texture_stages.DiffuseTextureStage = 0;
            texture_stages.BumpTextureStage = 1;
            NEngineInternal::GPRenderManager->AttachTextureToStage(2, NEngineInternal::GPResourceManager->GetZeroRGBATexture());
            ApplyStaticVertexShaderDP3BumpShadow(lights.GetCount());
            if (tinted)
                CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseDP3BumpFogShadowOutputFactor());
            else
                CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseDP3BumpFogShadow());
            flags = 0x10b;
            NEngineInternal::GPShadowRenderer->SetupShadowFuncRenderStates(3, primitive->ObjectTransform, CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            break;
        case 12:
            texture_stages.DiffuseTextureStage = 0;
            ApplyStaticVertexShaderSpotShadow(lights.GetCount());
            if (tinted)
                CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadowOutputFactor());
            else
                CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadow());
            NEngineInternal::GPShadowRenderer->SetupShadowFuncRenderStates(1, primitive->ObjectTransform, CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            NEngineInternal::GPLightingManager->SetupShaderSpotLight(primitive->ObjectTransform);
            flags = 0x10b;
            break;
        case 13: {
            texture_stages.DiffuseTextureStage = 0;
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, GetReflectionTexture(material, mipmap_level, used_blank, graphic_bank, block_on_load));
            const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            flags = 0x12b;
            ApplyStaticVertexShaderEnvSpotShadow(lights.GetCount());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseEnvFogShadow());
            NEngineInternal::GPShadowRenderer->SetupShadowFuncRenderStates(2, primitive->ObjectTransform, CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            NEngineInternal::GPLightingManager->SetupShaderSpotLight(primitive->ObjectTransform);
            break;
        }
        case 1:
            ApplyStaticVertexShader(0);
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseOnly());
            break;
        case 16: {
            const C2DAlphaMaskInfo* mask = GetCurrent2DAlphaMaskInfo();
            texture_stages.DiffuseTextureStage = 0;
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, Get2DAlphaMaskTexture(mask->AlphaMaskBankIndex, 0, used_blank, mask->AlphaMaskBank, block_on_load));
            Set2DAlphaMaskTextureMode(1, mask->FilterMask, true);
            Apply2DStaticVertexShader();
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuse(ENGINE_PRIMITIVE_BLEND_ALPHA));
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask->AlphaMaskPositionFactors.X, mask->AlphaMaskPositionFactors.Y,
                                                                                             mask->AlphaMaskPositionFactors.Z), mask->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        case 17: {
            const C2DAlphaMaskInfo* mask = GetCurrent2DAlphaMaskInfo();
            texture_stages.DiffuseTextureStage = 0;
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(1, Get2DAlphaMaskTexture(mask->AlphaMaskBankIndex, 0, used_blank, mask->AlphaMaskBank, block_on_load));
            NEngineInternal::GPRenderManager->AttachTextureToStage(2, GetReflectionTexture(material, mipmap_level, used_blank, graphic_bank, block_on_load));
            Set2DAlphaMaskTextureMode(1, mask->FilterMask, true);
            const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            flags = 0x129;
            Apply2DStaticVertexShaderEnv();
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuseEnv(ENGINE_PRIMITIVE_BLEND_ALPHA));
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask->AlphaMaskPositionFactors.X, mask->AlphaMaskPositionFactors.Y,
                                                                                             mask->AlphaMaskPositionFactors.Z), mask->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        case 18: {
            const C2DAlphaMaskInfo* mask = GetCurrent2DAlphaMaskInfo();
            texture_stages.DiffuseTextureStage = 0;
            texture_stages.BumpTextureStage = 1;
            bool used_blank;
            const CTexture* mask_texture = Get2DAlphaMaskTexture(mask->AlphaMaskBankIndex, 0, used_blank, mask->AlphaMaskBank, block_on_load);
            NEngineInternal::GPRenderManager->AttachTextureToStage(2, mask_texture);
            NEngineInternal::GPRenderManager->AttachTextureToStage(3, mask_texture);
            Set2DAlphaMaskTextureMode(2, mask->FilterMask, true);
            Apply2DStaticVertexShaderDP3Bump();
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuseDP3Bump(ENGINE_PRIMITIVE_BLEND_ALPHA));
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask->AlphaMaskPositionFactors.X, mask->AlphaMaskPositionFactors.Y,
                                                                                             mask->AlphaMaskPositionFactors.Z), mask->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        case 15: {
            const C2DAlphaMaskInfo* mask = GetCurrent2DAlphaMaskInfo();
            bool used_blank;
            NEngineInternal::GPRenderManager->AttachTextureToStage(0, Get2DAlphaMaskTexture(mask->AlphaMaskBankIndex, 0, used_blank, mask->AlphaMaskBank, block_on_load));
            Set2DAlphaMaskTextureMode(0, mask->FilterMask, true);
            Apply2DStaticVertexShader();
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuseOnly(ENGINE_PRIMITIVE_BLEND_ALPHA));
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask->AlphaMaskPositionFactors.X, mask->AlphaMaskPositionFactors.Y,
                                                                                             mask->AlphaMaskPositionFactors.Z), mask->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        default:
            __assume(0);
        }
        NEngineInternal::GPLightingManager->SetupShaderPointLights(lights);
        NEngineInternal::GPLightingManager->SetSelfIlluminationAmbient(material->SelfIllumination);
        NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
    } else if (pass == RENDER_PASS_Z_WRITE) {
        CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f));
        if (material->Transparent) {
            texture_stages.DiffuseTextureStage = 0;
            if (mesh_primitive->IsDP3Bump())
                texture_stages.BumpTextureStage = 1;
        } else {
            SetupBlankTextures(texture_stages, mesh_primitive->IsDP3Bump());
        }
        ApplyStaticVertexShader(0);
        CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFog());
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_STENCIL) {
        SetupOutlineSmoothStencilStates(*static_cast<const CEngineMeshEffectOutline*>(primitive->MeshEffect.Get()), primitive->Scale);
        SetupBlankTextures(texture_stages, mesh_primitive->IsDP3Bump());
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderDP3BumpOutline : VertexShaderOutline);
        CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseOnly());
        *render_degenerate_triangles = true;
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_FILL) {
        const CEngineMeshEffectOutline* effect = static_cast<const CEngineMeshEffectOutline*>(primitive->MeshEffect.Get());
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderDP3BumpOutlineSmoothFill : VertexShaderOutlineSmoothFill);
        *render_degenerate_triangles = true;
        NEngineInternal::GPScreenEffectOutlineGlow->SetupDepthBufferTransform(primitive->ObjectTransform);
        bool dp3_bump = mesh_primitive->IsDP3Bump();
        long stage_count = dp3_bump ? 2 : 1;
        if (material->DiffuseMapBankIndex != 0 && material->Transparent) {
            texture_stages.DiffuseTextureStage = 0;
            if (mesh_primitive->IsDP3Bump()) {
                texture_stages.BumpTextureStage = 1;
                CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillDP3Alpha());
            } else
                CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillAlpha());
            SetupOutlineSmoothFillStates(*effect, primitive->Scale, true, stage_count);
        } else {
            SetupBlankTextures(texture_stages, dp3_bump);
            if (mesh_primitive->IsDP3Bump())
                CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillDP3());
            else
                CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFill());
            SetupOutlineSmoothFillStates(*effect, primitive->Scale, false, stage_count);
        }
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_ALPHA_STENCIL_MASK) {
        const CEngineMeshEffectOutline* effect = static_cast<const CEngineMeshEffectOutline*>(primitive->MeshEffect.Get());
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderDP3BumpOutline : VertexShaderOutline);
        *render_degenerate_triangles = true;
        if (material->DiffuseMapBankIndex != 0 && material->Transparent) {
            texture_stages.DiffuseTextureStage = 0;
            if (mesh_primitive->IsDP3Bump())
                texture_stages.BumpTextureStage = 1;
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseColourTextureDiffuseAlpha());
            SetupOutlineSmoothAlphaStencilMaskStates(*effect);
        } else {
            SetupBlankTextures(texture_stages, mesh_primitive->IsDP3Bump());
            CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseOnly());
            SetupOutlineSmoothAlphaStencilMaskStates(*effect);
        }
    } else if (pass == RENDER_PASS_OUTLINE_STENCIL) {
        SetupOutlineStencilStates(*static_cast<const CEngineMeshEffectOutline*>(primitive->MeshEffect.Get()), primitive->Scale);
        SetupBlankTextures(texture_stages, mesh_primitive->IsDP3Bump());
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderDP3BumpOutline : VertexShaderOutline);
        CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseOnly());
        *render_degenerate_triangles = true;
    } else {
        SetupOutlineFillStates(*static_cast<const CEngineMeshEffectOutline*>(primitive->MeshEffect.Get()), primitive->Scale);
        SetupBlankTextures(texture_stages, mesh_primitive->IsDP3Bump());
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderDP3BumpOutline : VertexShaderOutline);
        CEnginePrimitiveRenderUtil::ApplyPixelShader(CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseOnly());
        *render_degenerate_triangles = true;
    }
    if (texture_stages.DiffuseTextureStage >= 0) {
        NEngineInternal::GPRenderManager->SetTextureUVModeU(texture_stages.DiffuseTextureStage, TEXTURE_UV_WRAP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(texture_stages.DiffuseTextureStage, TEXTURE_UV_WRAP);
    }
    if (texture_stages.BumpTextureStage >= 0) {
        NEngineInternal::GPRenderManager->SetTextureUVModeU(texture_stages.BumpTextureStage, TEXTURE_UV_WRAP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(texture_stages.BumpTextureStage, TEXTURE_UV_WRAP);
    }
    *shader_flags = flags;
    return true;
}
