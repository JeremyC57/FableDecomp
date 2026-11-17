// CEngineScreenEffectRadialBlurRenderer::CEngineScreenEffectRadialBlurRenderer(const CEngineInit& init) -- constructs the radial blur screen effect: supported only when the init flags carry 0x80 (otherwise the component is disabled); when supported, looks up the copy / blur / texture shaders, creates the textures and fills the four screen quads; then enables the environment parameters and resets them
// Leaf: ??0CEngineScreenEffectRadialBlurRenderer@@QAE@ABVCEngineInit@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5C460, length 1552 bytes (`ret 4`, no EH frame).
// Label correction: catalogued "CEnginePrimitiveManagerText::CEnginePrimitiveManagerText" (propagated); the body calls the
//   CEngineComponent ctor @ 0x00B59710 and stores vtable 0x012A2658 = CEngineScreenEffectRadialBlurRenderer (rtti-vtables.tsv).
// Callees: CEngineComponent() @ 0x00B59710, CVertexShader() @ 0x00A62FB0, CPixelShader() @ 0x00A5EC00, CCharString @ 0x0099EBF0 /
//   ~ @ 0x0099EAE0, GetVertexShader @ 0x00A5D5F0 / GetPixelShader @ 0x00A5D720, InitialiseTextures @ 0x00B5BF10,
//   ResetParameters @ 0x00B5BBC0.
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h offsets hold (0x22c bytes); restated locally with the real shader / vertex
//   types (model as the landed sibling 0x00B86C00).
// Shape: the per-quad X/Y/Z then U/V stores and a 4-iteration colour loop over the four quads; VC7.1 fully unrolls the loop and
//   schedules the stores exactly as retail.
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A); the vertex shader resources are
// CEngineResourceManager::VShaderResources[bank] (retail +0x29C0 + 8*bank, FableWin.pdb inline GetVShaderResource) and the pixel one
// ShaderResourcePixel (+0x2A98, GetPShaderResource). Shader classes from the shared headers (CShaderResource::GetVertexShader /
// GetPixelShader const, Ego_r.pdb / FableWin.pdb); were a local CEngineData / g_pEngineData stand-in and local shader models.
// agentNF57 (q10421): the local CEngineScreenEffectRadialBlurRenderer / CEngineInit / CTVertexColTex1 / C2DVector / CParameters models
//   replaced by the header's retail _MEMBERS branch (+ overlay opt-ins _INITIALISETEXTURES and _SETDEFAULTPARAMS) and engine/CEngineInit.h
//   (InitFlags +0x60). InitialiseTextures 0x00B5BF10 now binds the private PDB spelling, and the last call 0x00B5BBC0 is Ego_r.pdb
//   0x00A0DD80 / FableWin.pdb 0x02D1C070 ?SetDefaultParams@CEngineScreenEffectRadialBlurRenderer@@QAEXXZ (its body stores the
//   CurrentParameters defaults; was the unproven ResetParameters).
#define FABLE_RETAIL_CENGINECOMPONENT_PROTECTED
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADER
#include "fable_string.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_INITIALISETEXTURES
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_SETDEFAULTPARAMS
#include "engine/CEngineInit.h"
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

CEngineScreenEffectRadialBlurRenderer::CEngineScreenEffectRadialBlurRenderer(const CEngineInit& init)
{
    if (init.InitFlags & 0x80) {
        EffectSupported = true;
    } else {
        EffectSupported = false;
        RenderEnabled = false;
    }
    if (EffectSupported) {
        ScreenCopyVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)24)->GetVertexShader("VSHADER_SCREEN_FX_RADIAL_BLUR_BBCOPY");
        ScreenCopyPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SCREEN_FX_RADIAL_BLUR_BBCOPY");
        RadialBlurVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)24)->GetVertexShader("VSHADER_SCREEN_FX_RADIAL_BLUR");
        RadialBlurPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SCREEN_FX_RADIAL_BLUR");
        TextureOnlyPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_ONLY");
        TextureDiffusePixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE");
    }
    ShowDebug = 0;
    BlurTexture256_1 = 0;
    BlurTexture256_2 = 0;
    DynamicTexturePageHandle = -1;
    if (EffectSupported) {
        InitialiseTextures();
        if (EffectSupported) {
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
            BackBufferToTextureVerts[0].U1 = 0.0f;
            BackBufferToTextureVerts[0].V1 = 1.0f;
            BackBufferToTextureVerts[1].U1 = 1.0f;
            BackBufferToTextureVerts[1].V1 = 1.0f;
            BackBufferToTextureVerts[2].U1 = 1.0f;
            BackBufferToTextureVerts[2].V1 = 0.0f;
            BackBufferToTextureVerts[3].U1 = 0.0f;
            BackBufferToTextureVerts[3].V1 = 0.0f;
            Blur256ToBlur256Verts[0].X = -1.0f;
            Blur256ToBlur256Verts[0].Y = -1.0f;
            Blur256ToBlur256Verts[0].Z = 0.0f;
            Blur256ToBlur256Verts[1].X = 1.0f;
            Blur256ToBlur256Verts[1].Y = -1.0f;
            Blur256ToBlur256Verts[1].Z = 0.0f;
            Blur256ToBlur256Verts[2].X = 1.0f;
            Blur256ToBlur256Verts[2].Y = 1.0f;
            Blur256ToBlur256Verts[2].Z = 0.0f;
            Blur256ToBlur256Verts[3].X = -1.0f;
            Blur256ToBlur256Verts[3].Y = 1.0f;
            Blur256ToBlur256Verts[3].Z = 0.0f;
            Blur256ToBlur256Verts[0].U1 = -0.5f;
            Blur256ToBlur256Verts[0].V1 = 0.5f;
            Blur256ToBlur256Verts[1].U1 = 0.5f;
            Blur256ToBlur256Verts[1].V1 = 0.5f;
            Blur256ToBlur256Verts[2].U1 = 0.5f;
            Blur256ToBlur256Verts[2].V1 = -0.5f;
            Blur256ToBlur256Verts[3].U1 = -0.5f;
            Blur256ToBlur256Verts[3].V1 = -0.5f;
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
            TextureToBackBufferVerts[0].U1 = 0.001953125f;
            TextureToBackBufferVerts[0].V1 = 1.001953125f;
            TextureToBackBufferVerts[1].U1 = 1.001953125f;
            TextureToBackBufferVerts[1].V1 = 1.001953125f;
            TextureToBackBufferVerts[2].U1 = 1.001953125f;
            TextureToBackBufferVerts[2].V1 = 0.001953125f;
            TextureToBackBufferVerts[3].U1 = 0.001953125f;
            TextureToBackBufferVerts[3].V1 = 0.001953125f;
            BlurToBackBufferVerts[0].X = -1.0f;
            BlurToBackBufferVerts[0].Y = -1.0f;
            BlurToBackBufferVerts[0].Z = 0.0f;
            BlurToBackBufferVerts[1].X = 0.0f;
            BlurToBackBufferVerts[1].Y = -1.0f;
            BlurToBackBufferVerts[1].Z = 0.0f;
            BlurToBackBufferVerts[2].X = 0.0f;
            BlurToBackBufferVerts[2].Y = 0.0f;
            BlurToBackBufferVerts[2].Z = 0.0f;
            BlurToBackBufferVerts[3].X = -1.0f;
            BlurToBackBufferVerts[3].Y = 0.0f;
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
                BackBufferToTextureVerts[i].DiffuseColour = 0xffffffff;
                Blur256ToBlur256Verts[i].DiffuseColour = 0xffffffff;
                TextureToBackBufferVerts[i].DiffuseColour = 0xffffffff;
                BlurToBackBufferVerts[i].DiffuseColour = 0xffffffff;
            }
        }
    }
    UseEnvironmentParameters = true;
    SetDefaultParams();
}