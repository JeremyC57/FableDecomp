// CEngineScreenEffectGlowRenderer::CEngineScreenEffectGlowRenderer(const CEngineInit& init) -- construct the screen glow effect: the component base and the nine shader wrappers; the effect is supported only when the init flags carry 0x80 (otherwise the component is disabled); when supported, look up the glow / blur / apply / debug shaders, create the glow textures, clear the texture pointers and fill the five screen quads (the down-sample quads with a half-texel offset of 1/256 and 1/64, the lower-left debug quad); then the threshold 1.33, intensity 0.275, motion blur 1.0 and the environment theme on
// Leaf: ??0CEngineScreenEffectGlowRenderer@@QAE@ABVCEngineInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B5F090, 2093 bytes, thiscall, `ret 4` (returns this), no EH frame.
// Identity (proven): the vtable stored after the CEngineComponent base ctor is 0x012A27CC, RTTI .?AVCEngineScreenEffectGlowRenderer@@
//   (engine/CEngineScreenEffectGlowRenderer.h); its only caller is CEngine::Initialise 0x00B26360 (call at 0x00B269FD right after
//   `new` of 0x280 bytes, the class size); Ego_r.pdb / FableWin.pdb ??0CEngineScreenEffectGlowRenderer@@QAE@ABVCEngineInit@@@Z; the
//   shader literals are the glow set ("VSHADER_SCREEN_FX_GLOW" 0x012A28CC .. "PSHADER_TEXTURE_ONLY" 0x012A1978). The ledger name
//   "CEnginePrimitiveManagerDecal::CEnginePrimitiveManagerDecal" is wrong (that class's ctor is the landed 0x00BBA480).
// Callees: CEngineComponent::CEngineComponent 0x00B59710, CVertexShader() 0x00A62FB0, CPixelShader() 0x00A5EC00,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CShaderResource::GetVertexShader / GetPixelShader
//   (const CCharString&) const 0x00A5D5F0 / 0x00A5D720 on the resource manager 0x01436E98's VShaderResources[13] (+0x2A28) and
//   ShaderResourcePixel (+0x2A98), InitialiseTextures 0x00B5ECE0 (FableWin.pdb ?InitialiseTextures@CEngineScreenEffectGlowRenderer@@AAE_NXZ;
//   the landed row spells it ?InitialiseTextures@CGlowRendererMethods@@QAEHXZ, NAME_FIXES.tsv). Inline: CVertexShader::operator=,
//   CPixelShader::operator= (nulls Data), the temporaries' ~CVertexShader / ~CPixelShader (C*ShaderData::Release) and ~CShaderBase.
// Layout: hdr/engine/CEngineScreenEffectGlowRenderer.h opt-in FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_MEMBERS (Ego_r.pdb names and
//   offsets, which are retail's: no STLport member), CTVertexColTex1 (new hdr/engine/CTVertexColTex1.h), CEngineComponent::RenderEnabled
//   +0x08 through the access-only opt-in FABLE_RETAIL_CENGINECOMPONENT_PROTECTED.
// Shape: one statement per vertex field, quad by quad (positions, then UVs; the first quad has no UVs), the 16 diffuse colours as one
//   `for (i < 4)` loop over the four full-screen quads (VC7.1 unrolls it; the -1 register is the CCharString length too), the
//   thresholds and intensities as chained assignments (retail stores B, G, R).
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT_PROTECTED
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CEngineScreenEffectGlowRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/CEngineInit.h"
extern CEngineResourceManager* g_pResourceManager;    // 0x01436E98 (descriptive; no PDB name for this global)
CEngineScreenEffectGlowRenderer::CEngineScreenEffectGlowRenderer(const CEngineInit& init)
{
    if (init.InitFlags & 0x80)
        EffectSupported = true;
    else {
        EffectSupported = false;
        RenderEnabled = false;
    }
    DynamicTexturePageHandle = -1;
    ShowDebug = 0;
    if (EffectSupported) {
        VertexShaderCopy = g_pResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_SCREEN_FX_GLOW");
        VertexShaderBlur = g_pResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_SCREEN_FX_GLOW_BLUR");
        PixelShaderGlowNeg = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SCREEN_FX_GLOW_NEG");
        PixelShaderGlow = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SCREEN_FX_GLOW");
        PixelShaderBlur = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SCREEN_FX_GLOW_BLUR");
        VertexShaderApplyGlow = g_pResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_SCREEN_FX_DISPLACEMENT");
        PixelShaderApplyGlow = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_APPLY_GLOW");
        VertexShaderShowDebug = g_pResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_SCREEN_FX_COPY");
        PixelShaderShowDebug = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_ONLY");
        InitialiseTextures();
        GlowTexture256 = 0;
        GlowTexture64 = 0;
        GlowTexture64Alt = 0;
        GlowTexture16 = 0;
        BackBufferToTextureVerts[0].X = -1.0f;
        BackBufferToTextureVerts[0].Y = -1.0f;
        BackBufferToTextureVerts[0].Z = 0.0f;
        BackBufferToTextureVerts[1].X = 1.0f;
        BackBufferToTextureVerts[1].Y = -1.0f;
        BackBufferToTextureVerts[1].Z = 0.0f;
        BackBufferToTextureVerts[2].X = 1.0f;
        BackBufferToTextureVerts[2].Y = 1.0f;
        BackBufferToTextureVerts[2].Z = 0.0f;
        BackBufferToTextureVerts[3].X = -1.0f;
        BackBufferToTextureVerts[3].Y = 1.0f;
        BackBufferToTextureVerts[3].Z = 0.0f;
        Glow256ToGlow64Verts[0].X = -1.0f;
        Glow256ToGlow64Verts[0].Y = -1.0f;
        Glow256ToGlow64Verts[0].Z = 0.0f;
        Glow256ToGlow64Verts[1].X = 1.0f;
        Glow256ToGlow64Verts[1].Y = -1.0f;
        Glow256ToGlow64Verts[1].Z = 0.0f;
        Glow256ToGlow64Verts[2].X = 1.0f;
        Glow256ToGlow64Verts[2].Y = 1.0f;
        Glow256ToGlow64Verts[2].Z = 0.0f;
        Glow256ToGlow64Verts[3].X = -1.0f;
        Glow256ToGlow64Verts[3].Y = 1.0f;
        Glow256ToGlow64Verts[3].Z = 0.0f;
        Glow256ToGlow64Verts[0].U1 = 0.00390625f;
        Glow256ToGlow64Verts[0].V1 = 1.00390625f;
        Glow256ToGlow64Verts[1].U1 = 1.00390625f;
        Glow256ToGlow64Verts[1].V1 = 1.00390625f;
        Glow256ToGlow64Verts[2].U1 = 1.00390625f;
        Glow256ToGlow64Verts[2].V1 = 0.00390625f;
        Glow256ToGlow64Verts[3].U1 = 0.00390625f;
        Glow256ToGlow64Verts[3].V1 = 0.00390625f;
        Glow64ToGlow16Verts[0].X = -1.0f;
        Glow64ToGlow16Verts[0].Y = -1.0f;
        Glow64ToGlow16Verts[0].Z = 0.0f;
        Glow64ToGlow16Verts[1].X = 1.0f;
        Glow64ToGlow16Verts[1].Y = -1.0f;
        Glow64ToGlow16Verts[1].Z = 0.0f;
        Glow64ToGlow16Verts[2].X = 1.0f;
        Glow64ToGlow16Verts[2].Y = 1.0f;
        Glow64ToGlow16Verts[2].Z = 0.0f;
        Glow64ToGlow16Verts[3].X = -1.0f;
        Glow64ToGlow16Verts[3].Y = 1.0f;
        Glow64ToGlow16Verts[3].Z = 0.0f;
        Glow64ToGlow16Verts[0].U1 = 0.015625f;
        Glow64ToGlow16Verts[0].V1 = 1.015625f;
        Glow64ToGlow16Verts[1].U1 = 1.015625f;
        Glow64ToGlow16Verts[1].V1 = 1.015625f;
        Glow64ToGlow16Verts[2].U1 = 1.015625f;
        Glow64ToGlow16Verts[2].V1 = 0.015625f;
        Glow64ToGlow16Verts[3].U1 = 0.015625f;
        Glow64ToGlow16Verts[3].V1 = 0.015625f;
        TextureToBackBufferVerts[0].X = -1.0f;
        TextureToBackBufferVerts[0].Y = -1.0f;
        TextureToBackBufferVerts[0].Z = 0.0f;
        TextureToBackBufferVerts[1].X = 1.0f;
        TextureToBackBufferVerts[1].Y = -1.0f;
        TextureToBackBufferVerts[1].Z = 0.0f;
        TextureToBackBufferVerts[2].X = 1.0f;
        TextureToBackBufferVerts[2].Y = 1.0f;
        TextureToBackBufferVerts[2].Z = 0.0f;
        TextureToBackBufferVerts[3].X = -1.0f;
        TextureToBackBufferVerts[3].Y = 1.0f;
        TextureToBackBufferVerts[3].Z = 0.0f;
        TextureToBackBufferVerts[0].U1 = 0.0f;
        TextureToBackBufferVerts[0].V1 = 1.0f;
        TextureToBackBufferVerts[1].U1 = 1.0f;
        TextureToBackBufferVerts[1].V1 = 1.0f;
        TextureToBackBufferVerts[2].U1 = 1.0f;
        TextureToBackBufferVerts[2].V1 = 0.0f;
        TextureToBackBufferVerts[3].U1 = 0.0f;
        TextureToBackBufferVerts[3].V1 = 0.0f;
        GlowToBackBufferVerts[0].X = -1.0f;
        GlowToBackBufferVerts[0].Y = -1.0f;
        GlowToBackBufferVerts[0].Z = 0.0f;
        GlowToBackBufferVerts[1].X = 0.0f;
        GlowToBackBufferVerts[1].Y = -1.0f;
        GlowToBackBufferVerts[1].Z = 0.0f;
        GlowToBackBufferVerts[2].X = 0.0f;
        GlowToBackBufferVerts[2].Y = 0.0f;
        GlowToBackBufferVerts[2].Z = 0.0f;
        GlowToBackBufferVerts[3].X = -1.0f;
        GlowToBackBufferVerts[3].Y = 0.0f;
        GlowToBackBufferVerts[3].Z = 0.0f;
        GlowToBackBufferVerts[0].U1 = 0.0f;
        GlowToBackBufferVerts[0].V1 = 1.0f;
        GlowToBackBufferVerts[1].U1 = 1.0f;
        GlowToBackBufferVerts[1].V1 = 1.0f;
        GlowToBackBufferVerts[2].U1 = 1.0f;
        GlowToBackBufferVerts[2].V1 = 0.0f;
        GlowToBackBufferVerts[3].U1 = 0.0f;
        GlowToBackBufferVerts[3].V1 = 0.0f;
        for (long i = 0; i < 4; ++i) {
            BackBufferToTextureVerts[i].DiffuseColour = 0xffffffff;
            Glow256ToGlow64Verts[i].DiffuseColour = 0xffffffff;
            Glow64ToGlow16Verts[i].DiffuseColour = 0xffffffff;
            TextureToBackBufferVerts[i].DiffuseColour = 0xffffffff;
        }
    }
    ThresholdR = ThresholdG = ThresholdB = 1.33f;
    IntensityR = IntensityG = IntensityB = 0.275f;
    MotionBlur = 1.0f;
    UseEnvironmentTheme = true;
}