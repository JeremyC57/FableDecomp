// CEngineWeatherRenderer::Render(const CEngineComponentRenderParams&) -- draw the weather: inside a render-state capture block with both shader kinds forced on and vertex constant layout 1, set the particle states (no z write, alpha test > 1, src-alpha / inv-src-alpha blending, wrap U / V and clamp W on stages 0..3), reset the world transform, draw rain and snow when active, then for mist turn z testing off, mirror every stage's UV modes and draw the mist; unbind the index and vertex buffers
// Leaf: ?Render@CEngineWeatherRenderer@@UAEXABVCEngineComponentRenderParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B555A0, 3832 bytes (0x00B555A0..0x00B56497; two `ret 4` tails 0x00B5647F / 0x00B56495), thiscall,
//   0xC-byte frame, no EH frame, no ebp frame. Start: vtable CEngineWeatherRenderer 0x012A2434 slot 4 (only reference); next function 0x00B564A0 area.
// Identity (three sources): Ego_r.pdb 0x009FB880 and FableWin.pdb 0x02D3B660 ?Render@CEngineWeatherRenderer@@UAEXABVCEngineComponentRenderParams@@@Z
//   (Ego_r order RenderMist / RenderRain / RenderSnow / Render = retail 0x00B54310 / 0x00B54890 / 0x00B54E30 / 0x00B555A0); rtti vtable slot 4.
// Callees: CShaderRenderManager::EnableVertexShaders 0x00987FE0 / EnablePixelShaders 0x00988110 / DisableVertexShaders 0x009880E0 /
//   DisablePixelShaders 0x00988190 / SetVertexShaderConstantLayout 0x0098B5E0 / ResetWorldTransform 0x00988290, RenderRain 0x00B54890,
//   RenderSnow 0x00B54E30, RenderMist 0x00B54310, CRenderStateManager::RestoreCaptureBlock 0x00A05840; the inline state setters (SetRenderState
//   default text), StartCaptureBlock, SelectIndexBuffer / SelectVertexBuffer (IDirect3DDevice9 SetIndices / SetStreamSource).
// Globals: NEngineInternal::GPRenderManager 0x01436E18, GPShaderRenderManager 0x01436E14, CShaderRenderManager singleton 0x013BC470,
//   capture-block state manager 0x013CB508, CRenderStateManager::D3DAlphaFuncModeTable 0x01396DD8 / D3DAlphaBlendModeTable 0x01396F68 /
//   D3DTextureUVModeTable 0x01396DC8. Members: RenderEnabled +0x08 (PeekRenderEnabled), Settings.Raining / Snowing / Mist +0x5D0 / 1 / 2.
// Scope objects in retail's constructor / destructor order: capture block, shader enables, vertex constant layout.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWEATHERRENDERER
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_BUFFERS
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDERRAIN
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDERSNOW
#define FABLE_RETAIL_CENGINEWEATHERRENDERER_RENDER
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL
#define FABLE_RETAIL_CRENDERMANAGERCORE
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14

void CEngineWeatherRenderer::Render(const CEngineComponentRenderParams& params)
{
    if (!PeekRenderEnabled())
        return;

    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    CScopedVertexShaderConstantLayout layout(1);

    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    NEngineInternal::GPRenderManager->SetAlphaRef(1);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeW(0, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeW(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeW(2, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(3, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(3, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeW(3, TEXTURE_UV_CLAMP);
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();

    if (Settings.Raining)
        RenderRain(params);
    if (Settings.Snowing)
        RenderSnow(params);
    if (Settings.Mist)
    {
        NEngineInternal::GPRenderManager->SetZBufferEnable(false);
        NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_MIRROR);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_MIRROR);
        NEngineInternal::GPRenderManager->SetTextureUVModeW(0, TEXTURE_UV_MIRROR);
        NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_MIRROR);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_MIRROR);
        NEngineInternal::GPRenderManager->SetTextureUVModeW(1, TEXTURE_UV_MIRROR);
        NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_MIRROR);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_MIRROR);
        NEngineInternal::GPRenderManager->SetTextureUVModeW(2, TEXTURE_UV_MIRROR);
        NEngineInternal::GPRenderManager->SetTextureUVModeU(3, TEXTURE_UV_MIRROR);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(3, TEXTURE_UV_MIRROR);
        NEngineInternal::GPRenderManager->SetTextureUVModeW(3, TEXTURE_UV_MIRROR);
        RenderMist(params);
    }
    NEngineInternal::GPRenderManager->SelectIndexBuffer(0, 0);
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, 0);
}
