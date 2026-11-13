// CEngineScreenEffectOutlineGlow::CEngineScreenEffectOutlineGlow(const CEngineInit& init) -- construct the outline glow effect: the component base, the saved render target, the six shader wrappers and the capture block; allocate a render-target scratch page with a 256x256 outline texture and two 128x128 blur textures (A8R8G8B8), fill the three screen quads (outline -> blur with a 1/512 half-texel offset, blur -> blur with 1/256, blur -> back buffer), look up the blur / copy-outline (the SIMPLE variant when init flag 0x40 is set) / apply-outline shaders, then the default parameters (scale 0.333, extrusion 4.0, one blur step, depth bias -0.002, white override colour, alpha ref 0x80)
// Leaf: ??0CEngineScreenEffectOutlineGlow@@QAE@ABVCEngineInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B891B0, 1819 bytes, thiscall, `ret 4` (returns this), no EH frame (the TU has no /EHsc).
// Identity (proven): the vtable stored after the CEngineComponent base constructor is 0x012A3B6C, RTTI .?AVCEngineScreenEffectOutlineGlow@@
//   (rtti-vtables.tsv: slots 4 / 9 / 10 = the landed Render / GetConsoleEnableFunctionName / PeekSceneFilterFlags 0x00B8B550..0x00B8B5A0);
//   the only caller is CEngine::Initialise 0x00B26360 (`push 1E4h ; call operator new ; ... ; call 0x00B891B0` at 0x00B26679..0x00B2668D,
//   0x1E4 = the class size); Ego_r.pdb / FableWin.pdb ??0CEngineScreenEffectOutlineGlow@@QAE@ABVCEngineInit@@@Z; the body reads
//   InitFlags +0x60 of the CEngineInit argument; the shader literals ("VSHADER_SCREEN_FX_GLOW_BLUR" 0x012A28B0,
//   "PSHADER_TEXTURE_COPY_OUTLINE_SIMPLE" 0x012A3B44 / "PSHADER_TEXTURE_COPY_OUTLINE" 0x012A3B24, "VSHADER_SCREEN_FX_COPY" 0x012A11E4,
//   "PSHADER_TEXTURE_APPLY_OUTLINE" 0x012A3B04) are the outline set, and the landed destructor 0x00B898D0 releases exactly these members.
// Callees (every relocation checked against retail and reloc_bindings.tsv): CEngineComponent() 0x00B59710, CBaseClass() 0x0099A2D0
//   (OldRenderTarget's base; vtables CRenderTarget 0x0129C800 / CSurface 0x0122F84C stored in place), CVertexShader() 0x00A62FB0 x3,
//   CPixelShader() 0x00A5EC00 x3, CPixelFormat::Initialise(_D3DFORMAT) 0x009E3830, CGPUScratchBufferTextureManager::AllocteMemoryPage
//   (sic, both PDBs) 0x00F37660 / AllocateTexture 0x00F37440 x3, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CShaderResource::GetVertexShader / GetPixelShader (const CCharString&) const 0x00A5D5F0 / 0x00A5D720 on the resource manager
//   0x01436E98's VShaderResources[13] (+0x2A28) and ShaderResourcePixel (+0x2A98). Inline: the CRenderTarget / CSurface / capture-block
//   constructors, CPixelFormat() (-1), CEngineResourceManager::GetGPUScratchBufferTextureManager (+0x3190) / GetVShaderResource /
//   GetPShaderResource, C2DExtentsI(long, long), CVertexShader::operator= / CPixelShader::operator= (nulls Data), the temporaries'
//   ~CVertexShader / ~CPixelShader and ~CShaderBase, CRGBColour::Initialise.
// Layout: hdr/engine/CEngineScreenEffectOutlineGlow.h opt-in FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS (Ego_r.pdb names and
//   offsets, which are retail's), hdr/engine/CRenderTarget.h (chunk 2008), hdr/engine/CGPUScratchBufferTextureManager.h (chunk 2018
//   _TEXTURES), hdr/engine/CRenderStateCaptureBlock.h (new _INLINE_CTOR opt-in: FableWin-only inline constructor).
// Global: 0x01436E98 as `g_pResourceManager` (the spelling reloc_bindings.tsv binds and the sibling CEngineScreenEffectGlowRenderer
//   constructor 0x00B5F090 uses; no PDB data symbol exists, see NOTES.md).
// Shape: the sibling glow / displacement constructors' shape: one statement per vertex field, quad by quad (positions, then UVs), the 12
//   diffuse colours as one `for (i < 4)` loop over the three quads (VC7.1 unrolls it; the -1 kept in ebp is the CCharString length too);
//   OutlineTexture is cleared between the first and second AllocateTexture (retail 0x00B892B6 store sits in the second call's pushes),
//   the two blur pointers after the third; the pixel-shader pair as an if / else on InitFlags & 0x40 (the second assignment of the
//   SIMPLE arm cross-jumps into the else arm's tail at 0x00B896E2); the override colour through CRGBColour::Initialise (R, G, B, A
//   byte stores straight into the member; assigning a CRGBColour temporary builds it on the frame and copies a dword).
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_CTOR
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderTarget.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "fable_string.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/CEngineInit.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"

extern CEngineResourceManager* g_pResourceManager;    // 0x01436E98

CEngineScreenEffectOutlineGlow::CEngineScreenEffectOutlineGlow(const CEngineInit& init)
{
    OutlineGlowScaleFactor = 0.333f;
    CPixelFormat format;
    format.Initialise(D3DFMT_A8R8G8B8);
    CGPUScratchBufferTextureManager& scratch = g_pResourceManager->GetGPUScratchBufferTextureManager();
    DynamicTexturePageHandle = scratch.AllocteMemoryPage(0, true);
    OutlineTextureHandle = scratch.AllocateTexture(DynamicTexturePageHandle, C2DExtentsI(256, 256), 1, format, 1, false);
    OutlineTexture = 0;
    Blur1TextureHandle = scratch.AllocateTexture(DynamicTexturePageHandle, C2DExtentsI(128, 128), 1, format, 1, false);
    Blur2TextureHandle = scratch.AllocateTexture(DynamicTexturePageHandle, C2DExtentsI(128, 128), 1, format, 1, false);
    Blur1Texture = 0;
    Blur2Texture = 0;
    OutlineToBlurVerts[0].X = -1.0f;
    OutlineToBlurVerts[0].Y = -1.0f;
    OutlineToBlurVerts[0].Z = 0.0f;
    OutlineToBlurVerts[1].X = 1.0f;
    OutlineToBlurVerts[1].Y = -1.0f;
    OutlineToBlurVerts[1].Z = 0.0f;
    OutlineToBlurVerts[2].X = 1.0f;
    OutlineToBlurVerts[2].Y = 1.0f;
    OutlineToBlurVerts[2].Z = 0.0f;
    OutlineToBlurVerts[3].X = -1.0f;
    OutlineToBlurVerts[3].Y = 1.0f;
    OutlineToBlurVerts[3].Z = 0.0f;
    OutlineToBlurVerts[0].U1 = 0.001953125f;
    OutlineToBlurVerts[0].V1 = 1.001953125f;
    OutlineToBlurVerts[1].U1 = 1.001953125f;
    OutlineToBlurVerts[1].V1 = 1.001953125f;
    OutlineToBlurVerts[2].U1 = 1.001953125f;
    OutlineToBlurVerts[2].V1 = 0.001953125f;
    OutlineToBlurVerts[3].U1 = 0.001953125f;
    OutlineToBlurVerts[3].V1 = 0.001953125f;
    BlurToBlurVerts[0].X = -1.0f;
    BlurToBlurVerts[0].Y = -1.0f;
    BlurToBlurVerts[0].Z = 0.0f;
    BlurToBlurVerts[1].X = 1.0f;
    BlurToBlurVerts[1].Y = -1.0f;
    BlurToBlurVerts[1].Z = 0.0f;
    BlurToBlurVerts[2].X = 1.0f;
    BlurToBlurVerts[2].Y = 1.0f;
    BlurToBlurVerts[2].Z = 0.0f;
    BlurToBlurVerts[3].X = -1.0f;
    BlurToBlurVerts[3].Y = 1.0f;
    BlurToBlurVerts[3].Z = 0.0f;
    BlurToBlurVerts[0].U1 = 0.00390625f;
    BlurToBlurVerts[0].V1 = 1.00390625f;
    BlurToBlurVerts[1].U1 = 1.00390625f;
    BlurToBlurVerts[1].V1 = 1.00390625f;
    BlurToBlurVerts[2].U1 = 1.00390625f;
    BlurToBlurVerts[2].V1 = 0.00390625f;
    BlurToBlurVerts[3].U1 = 0.00390625f;
    BlurToBlurVerts[3].V1 = 0.00390625f;
    BlurToBackBufferVerts[0].X = -1.0f;
    BlurToBackBufferVerts[0].Y = -1.0f;
    BlurToBackBufferVerts[0].Z = 0.0f;
    BlurToBackBufferVerts[1].X = 1.0f;
    BlurToBackBufferVerts[1].Y = -1.0f;
    BlurToBackBufferVerts[1].Z = 0.0f;
    BlurToBackBufferVerts[2].X = 1.0f;
    BlurToBackBufferVerts[2].Y = 1.0f;
    BlurToBackBufferVerts[2].Z = 0.0f;
    BlurToBackBufferVerts[3].X = -1.0f;
    BlurToBackBufferVerts[3].Y = 1.0f;
    BlurToBackBufferVerts[3].Z = 0.0f;
    BlurToBackBufferVerts[0].U1 = 0.0f;
    BlurToBackBufferVerts[0].V1 = 1.0f;
    BlurToBackBufferVerts[1].U1 = 1.0f;
    BlurToBackBufferVerts[1].V1 = 1.0f;
    BlurToBackBufferVerts[2].U1 = 1.0f;
    BlurToBackBufferVerts[2].V1 = 0.0f;
    BlurToBackBufferVerts[3].U1 = 0.0f;
    BlurToBackBufferVerts[3].V1 = 0.0f;
    for (long i = 0; i < 4; ++i) {
        OutlineToBlurVerts[i].DiffuseColour = 0xffffffff;
        BlurToBlurVerts[i].DiffuseColour = 0xffffffff;
        BlurToBackBufferVerts[i].DiffuseColour = 0xffffffff;
    }
    OutlineToBlurVertexShader = g_pResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_SCREEN_FX_GLOW_BLUR");
    BlurToBlurVertexShader = g_pResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_SCREEN_FX_GLOW_BLUR");
    if (init.InitFlags & 0x40) {
        OutlineToBlurPixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_COPY_OUTLINE_SIMPLE");
        BlurToBlurPixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_COPY_OUTLINE_SIMPLE");
    } else {
        OutlineToBlurPixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_COPY_OUTLINE");
        BlurToBlurPixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_COPY_OUTLINE");
    }
    BlurToBackBufferVertexShader = g_pResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_SCREEN_FX_COPY");
    BlurToBackBufferPixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_APPLY_OUTLINE");
    ParametersOverridden = false;
    OverriddenParamsWidthWorldSpace = 0.0f;
    OverriddenParamsWidthScreenSpace = 0.0f;
    OverriddenParamsColour.Initialise(255, 255, 255, 255);
    OverriddenParamsTextureAlphaRef = 0x80;
    OverriddenParamsTextureAlphaBias = 0;
    EnableDrawDebugTextures = false;
    OuterStencilExtraNormalExtrusion = 4.0f;
    BlurStepCount = 1;
    DepthBufferBias = -0.002f;
}