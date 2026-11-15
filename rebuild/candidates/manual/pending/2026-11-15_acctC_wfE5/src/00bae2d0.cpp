// CEnginePrimitiveManager2DSprites::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- draw one 2D sprite or 2D clock sprite sub-primitive through the 2D alpha mask
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManager2DSprites@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BAE2D0, 5892 bytes (0x00BAE2D0..0x00BAF9D3, two `ret 8`), public virtual thiscall, vtable 0x012A5664 slot 5.
// Grade: RELOCATION_MATCH (5892v5892). No flags, no pragma. sub esp,34h, no EH frame (all objects inline: CRenderStateScopedCaptureBlock,
//   CScopedShadersEnable), `pass` unused.
// Identity: Ego_r.pdb / FableWin.pdb ?RenderSubPrimitive@CEnginePrimitiveManager2DSprites@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z;
//   vtable 0x012A5664 slot 5 = 0x00BAE2D0 (slot 13 RenderClockPrim 0x00BAFA00, slot 14 RenderSpritePrim 0x00BB0970).
// Callees (out of line): CShaderRenderManager::EnableVertexShaders 0x00987FE0 / DisableVertexShaders 0x009880E0 / EnablePixelShaders
//   0x00988110 / DisablePixelShaders 0x00988190 / ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 /
//   SetUserVertexShaderConstant(K, const C4DVector&) 0x00989AB0 and PrepareForRender's callees; CRenderManagerCore::DrawAsIndexedTris
//   0x00A0AEA0 (one call, shared by both arms); CRenderStateManager::RestoreCaptureBlock 0x00A05840.
// Expanded helpers (FableWin.pdb names; the three listed right after the row in FableWin are new opt-in
//   FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_2DSPRITES): SetupAlphaBlendingStatesFromBlendMode(mode, alpha, colour_scale&),
//   Setup2DDiffuseShading(textured, colour_scale, alpha, mode), GetPixelShader2DDiffuseOnly; plus the CLOTH2D set of the matched
//   0x00BB30A0 (GetCurrent2DAlphaMaskInfo, Set2DAlphaMaskTextureMode, DisableTextureFiltering, SetTextureFactor).
// Spellings: EVIDENCED -- `unsigned long alpha` read once per arm (retail movzx ebx,[sub+8] at 0x00BAEA69 / 0x00BAF4C2, then the
//   unsigned-long fild fix-up 0x00BAED0B / 0x00BAF731); the clock arm passes the Texture ARRAY to the bool `textured` (0x00BAED53
//   `lea esi,[ebp+120h]; test esi,esi`, always true); GetPixelShader2DDiffuse / ...Only as `mode == ALPHA ? A : B` (0x00BAED68
//   `cmp ecx,2`; fwsize: the ternary is FableWin SAME 19i/58b, if/return 14i). FITTED -- `EEnginePrimitiveBlendMode blend_mode`
//   named in each arm for SetupAlphaBlendingStatesFromBlendMode (without it one test/or operand order flips at 0x00BAF62D; the
//   SETRENDERSTATE_DESIRED / _BOOKMARK / ADDSTATE_DIRTY / OUTLINE_INLINE texts do not move it); the clock textures as an initializer
//   (`textures[1] = ...; textures[0] = ...;` swaps the two loads at 0x00BAEDB6, `[0]` then `[1]` also matches).
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTH2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_2DSPRITES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH2D
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DSPRITES_MANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DSPRITES_MEMBERS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_GETRGBCOLOUR_NAMED_RESULT
#include "fable_compiler.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveManager2DSprites.h"
#include "engine/CEngineSubPrimitive2DSprite.h"
#include "engine/CEngineSubPrimitive2DClockSprite.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C4DVector.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitive2DViewportManager.h"
#include "engine/C2DAlphaMaskInfo.h"
#include "engine/GFDebugPDynamicCast.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                                       // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }                           // 0x01436E14
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }     // 0x01436E78
namespace NEngineInternal { extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; }                         // 0x01436E84

FABLE_FORCEINLINE bool CShaderRenderManager::GetPixelShadersEnabled() const { return PixelShadersEnabled; }
FABLE_FORCEINLINE void CRenderStateManager::SetTextureFactor(unsigned long factor) { SetRenderState(factor, DeviceState.TextureFactor); }
FABLE_FORCEINLINE void __fastcall CEnginePrimitiveRenderUtil::SetTextureFactor(const CRGBFloatColour& colour)
{
    if (NEngineInternal::GPShaderRenderManager->GetPixelShadersEnabled())
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(colour));
    else
        NEngineInternal::GPRenderManager->SetTextureFactor(colour.GetRGBColour().IntValue);
}
inline void __fastcall CEnginePrimitiveRenderUtil::ApplyVertexShader(const CVertexShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, false);
}
inline void __fastcall CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(shader);
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuse(EEnginePrimitiveBlendMode blend_mode)
{
    return blend_mode == ENGINE_PRIMITIVE_BLEND_ALPHA ? NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuse
                                                     : NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseAdditiveAlpha;
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuseOnly(EEnginePrimitiveBlendMode blend_mode)
{
    return blend_mode == ENGINE_PRIMITIVE_BLEND_ALPHA ? NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseOnly
                                                     : NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseOnlyAdditiveAlpha;
}
inline const C2DAlphaMaskInfo* CEnginePrimitive2DViewportManager::GetCurrentAlphaMask() { return &AlphaMaskInfo; }
FABLE_FORCEINLINE const C2DAlphaMaskInfo* CEnginePrimitiveRenderUtil::GetCurrent2DAlphaMaskInfo()
{
    CEnginePrimitive2DViewportManager* manager = GFDebugPDynamicCast(NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(ENGINE_PRIMITIVE_2D_VIEWPORT),
                                                                      (CEnginePrimitive2DViewportManager*)0);
    return manager->GetCurrentAlphaMask();
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
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode, unsigned long alpha,
                                                                                         unsigned long& colour_scale)
{
    switch (mode) {
    case ENGINE_PRIMITIVE_BLEND_ADDITIVE:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        colour_scale = alpha;
        break;
    case ENGINE_PRIMITIVE_BLEND_ADDSMOOTH:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
        colour_scale = alpha;
        break;
    default:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        colour_scale = 255;
        break;
    }
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::Setup2DDiffuseShading(bool textured, unsigned long colour_scale, unsigned long alpha,
                                                                         EEnginePrimitiveBlendMode mode)
{
    float scale = colour_scale / 255.0f;
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(scale, scale, scale, alpha / 255.0f));
    if (textured)
        ApplyPixelShader(GetPixelShader2DDiffuse(mode));
    else
        ApplyPixelShader(GetPixelShader2DDiffuseOnly(mode));
}

void CEnginePrimitiveManager2DSprites::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    CRenderStateScopedCaptureBlock capture_block;
    CScopedShadersEnable shaders(true, true);
    const C2DAlphaMaskInfo* alpha_mask = GetCurrent2DAlphaMaskInfo();
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaRef(1);
    ApplyVertexShader(VertexShader2DSprite);
    Set2DAlphaMaskTextureMode(1, alpha_mask->FilterMask, true);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, alpha_mask->AlphaMaskPositionFactors);
    if (sub.GetParentBase()->GetType() == ENGINE_PRIMITIVE_2D_CLOCK_SPRITE) {
        CEngineSubPrimitive2DClockSprite& clock = static_cast<CEngineSubPrimitive2DClockSprite&>(sub);
        if (clock.WrapTexture) {
            NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
        } else {
            NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_CLAMP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
        }
        if (!clock.Filter)
            DisableTextureFiltering(0);
        unsigned long alpha = clock.PeekAlpha();
        unsigned long colour_scale;
        EEnginePrimitiveBlendMode blend_mode = clock.BlendMode;
        SetupAlphaBlendingStatesFromBlendMode(blend_mode, alpha, colour_scale);
        Setup2DDiffuseShading(clock.Texture != 0, colour_scale, alpha, clock.BlendMode);
        ApplyPixelShader(clock.BlendMode == ENGINE_PRIMITIVE_BLEND_ALPHA ? PixelShader2DClockSprite : PixelShader2DClockSpriteAdditive);
        const CTexture* textures[2] = { clock.Texture[0], clock.AlphaMask };
        long first = clock.IndexOffset / 3;
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(CRGBFloatColour(clock.Colour)));
        CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f));
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 2, clock.Vertices, sizeof(CTVertexColTex2), 7, clock.Indices, first);
        textures[0] = clock.Texture[1];
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(CRGBFloatColour(clock.Colour2)));
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 2, clock.Vertices, sizeof(CTVertexColTex2), 7, &clock.Indices[clock.IndexOffset],
                                                            6 - first);
    } else {
        CEngineSubPrimitive2DSprite& sprite = static_cast<CEngineSubPrimitive2DSprite&>(sub);
        if (sprite.WrapTexture) {
            NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
        } else {
            NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_CLAMP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
        }
        if (!sprite.Filter)
            DisableTextureFiltering(0);
        unsigned long alpha = sprite.PeekAlpha();
        unsigned long colour_scale;
        EEnginePrimitiveBlendMode blend_mode = sprite.BlendMode;
        SetupAlphaBlendingStatesFromBlendMode(blend_mode, alpha, colour_scale);
        Setup2DDiffuseShading(sprite.Texture != 0, colour_scale, alpha, sprite.BlendMode);
        CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f));
        const CTexture* textures[2] = { sprite.Texture, sprite.AlphaMask };
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 2, sprite.Vertices, sizeof(CTVertexColTex2), 4, Indices, 2);
    }
}
