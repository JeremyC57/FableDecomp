// CEngineScreenEffectDisplacementRenderer::CEngineScreenEffectDisplacementRenderer(const CEngineInit& init) -- construct the screen displacement effect: the component base, the saved render target, the four shader wrappers and the capture block; the effect (and its 2D/3D switches) is supported only when the init flags carry both 0x80 and 8 (otherwise the component is disabled); when supported, create the textures, fill the three screen quads (back buffer -> texture, texture -> back buffer with a half-texel offset, displacement -> back buffer on the lower-left quarter) and look up the copy and displacement shaders; then the clear size 128 x 128 and displace amount 0.25
// Leaf: ??0CEngineScreenEffectDisplacementRenderer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86C00, length 1184 bytes, `ret 4`, no EH frame.
// Label correction: catalogued "CEnginePrimitiveManager2DSprites::CEnginePrimitiveManager2DSprites" (propagated); the vtable
//   stored is 0x012A3A94 = CEngineScreenEffectDisplacementRenderer (rtti-vtables.tsv), InitialiseTextures 0x00B86A70 is that
//   class's, and the landed destructor 0x00B870A0 releases exactly these members.
// Callees: CEngineComponent::CEngineComponent @ 0x00B59710 (manifest label "CNavigationLayer" propagated), CBaseClass::CBaseClass
//   @ 0x0099A2D0, CVertexShader::CVertexShader @ 0x00A62FB0, CPixelShader::CPixelShader @ 0x00A5EC00, InitialiseTextures
//   @ 0x00B86A70, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CShaderResource::GetVertexShader
//   @ 0x00A5D5F0 / GetPixelShader @ 0x00A5D720 (engine data 0x01436E98: vertex resource +0x2A28, pixel resource +0x2A98).
// Layout: the landed destructor 0x00B870A0's retail view of engine/CEngineScreenEffectDisplacementRenderer.h (0x1ac bytes);
//   the shader wrappers and the lookup/assign/temporary-release shape are the landed CEnginePrimitiveManager2DSprites ctor
//   0x00BAD040 (the literal converts implicitly to the `const CCharString&` parameter; the length -1 is a default argument).
// Shape: the twelve vertex colours are ONE `for (i < 4)` loop over the three quads -- VC7.1 unrolls it fully, which is what
//   interleaves the colour stores (TextureToBackBuffer, BackBufferToTexture, Displacement per vertex) at the end and lets the
//   scheduler group the other constant stores by register; per-vertex Set()/member stores give another store order.
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A); the vertex shader resources are
// CEngineResourceManager::VShaderResources[bank] (retail +0x29C0 + 8*bank, FableWin.pdb inline GetVShaderResource) and the pixel one
// ShaderResourcePixel (+0x2A98, GetPShaderResource). Shader classes from the shared headers (CShaderResource::GetVertexShader /
// GetPixelShader const, Ego_r.pdb / FableWin.pdb); were a local CEngineData / g_pEngineData stand-in and local shader models.
// NF65 (round 7): ported onto engine/CEngineScreenEffectDisplacementRenderer.h (_MEMBERS + _INITIALISETEXTURES + _FINISHSTENCIL; overlay opt-in
//   _OLDRENDERTARGET types OldRenderTarget on engine/CRenderTarget.h), engine/CEngineInit.h (InitFlags +0x60), CTVertexColTex1.h,
//   CRenderStateCaptureBlock.h, CSurface.h; the local CSurface / CRenderTarget / CEngineInit / CTVertexColTex1 / CRenderStateCaptureBlock /
//   class models are gone. InitialiseTextures now binds the private ?InitialiseTextures@CEngineScreenEffectDisplacementRenderer@@AAE_NXZ
//   (q10423; was QAE).
#define FABLE_RETAIL_CENGINECOMPONENT_PROTECTED
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_INITIALISETEXTURES
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_FINISHSTENCIL
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_OLDRENDERTARGET
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_CTOR
#include "fable_string.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
#include "engine/CEngineInit.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

CEngineScreenEffectDisplacementRenderer::CEngineScreenEffectDisplacementRenderer(const CEngineInit& init)
{
    if ((init.InitFlags & 0x80) && (init.InitFlags & 8)) {
        EffectSupported = true;
        Enable2DDisplacement = true;
        Enable3DDisplacement = true;
    } else {
        EffectSupported = false;
        Enable2DDisplacement = false;
        Enable3DDisplacement = false;
        RenderEnabled = false;
    }
    DynamicTexturePageHandle = -1;
    EnableDisplacementTextureView = false;
    DisplacementUsesShadowZBuffer = true;
    if (EffectSupported) {
        InitialiseTextures();
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
        DisplacementToBackBufferVerts[0].X = -1.0f;
        DisplacementToBackBufferVerts[0].Y = -1.0f;
        DisplacementToBackBufferVerts[0].Z = 0.0f;
        DisplacementToBackBufferVerts[1].X = 0.0f;
        DisplacementToBackBufferVerts[1].Y = -1.0f;
        DisplacementToBackBufferVerts[1].Z = 0.0f;
        DisplacementToBackBufferVerts[2].X = 0.0f;
        DisplacementToBackBufferVerts[2].Y = 0.0f;
        DisplacementToBackBufferVerts[2].Z = 0.0f;
        DisplacementToBackBufferVerts[3].X = -1.0f;
        DisplacementToBackBufferVerts[3].Y = 0.0f;
        DisplacementToBackBufferVerts[3].Z = 0.0f;
        DisplacementToBackBufferVerts[0].U1 = 0.0f;
        DisplacementToBackBufferVerts[0].V1 = 1.0f;
        DisplacementToBackBufferVerts[1].U1 = 1.0f;
        DisplacementToBackBufferVerts[1].V1 = 1.0f;
        DisplacementToBackBufferVerts[2].U1 = 1.0f;
        DisplacementToBackBufferVerts[2].V1 = 0.0f;
        DisplacementToBackBufferVerts[3].U1 = 0.0f;
        DisplacementToBackBufferVerts[3].V1 = 0.0f;
        for (long i = 0; i < 4; ++i) {
            TextureToBackBufferVerts[i].DiffuseColour = 0xffffffff;
            BackBufferToTextureVerts[i].DiffuseColour = 0xffffffff;
            DisplacementToBackBufferVerts[i].DiffuseColour = 0xffffffff;
        }
        ScreenCopyVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_SCREEN_FX_COPY");
        ScreenCopyPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_ONLY");
        DisplacementVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_SCREEN_FX_DISPLACEMENT");
        DisplacementPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SCREEN_FX_DISPLACEMENT");
    }
    ClearX = 0x80;
    ClearY = 0x80;
    HasContent = false;
    DisplaceAmount = 0.25f;
}