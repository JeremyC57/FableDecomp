// CEngineStateBlockAlpha::Apply() -- the alpha-blend engine state block: src-alpha / inv-src-alpha blending on, stage 0 modulates texture by diffuse (colour and alpha), stage 1 off.
// Leaf: ?Apply@CEngineStateBlockAlpha@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x00B573A0, 1208 bytes, plain `ret` (virtual slot 0 of vtable 0x012A0E8C; `this` unused).
// RTTI .?AVCEngineStateBlockAlpha@@ : CStateBlockFunctionBase (engine/CEngineStateBlocks.h FABLE_RETAIL_CENGINESTATEBLOCKS).
// Body: eleven inline CRenderStateManager setters (engine/CRenderStateManager.h FABLE_RETAIL_CRENDERSTATEMANAGER) on the render
// manager singleton NEngineInternal::GPRenderManager 0x01436E18, which retail re-reads after every changed state (the stores may
// alias it). Each setter maps the engine enum through its private static D3D table: D3DAlphaBlendModeTable 0x01396F68 (+0x10 SRCALPHA,
// +0x14 INVSRCALPHA), D3DTextureOperationModeTable 0x01396E70 (+0x0C MODULATE, +0 DISABLE), D3DTextureArgumentModeTable 0x01396EE8
// (+8 TEXTURE, +4 DIFFUSE). States: DeviceState (+0x2818) SourceAlphaBlendMode, DestAlphaBlendMode, AlphaBlendEnable, then stage 0
// TextureOperation / TextureAlphaOperation / TextureArgument1 / TextureArgument2 / TextureAlphaArgument1 / TextureAlphaArgument2, then
// stage 1 TextureOperation / TextureAlphaOperation.
#define FABLE_RETAIL_CENGINESTATEBLOCKS
#include "engine/CRenderManager.h"
#include "engine/CEngineStateBlocks.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

void CEngineStateBlockAlpha::Apply()
{
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetTextureOperation(0, TEXTURE_OP_MODULATE);
    NEngineInternal::GPRenderManager->SetTextureAlphaOperation(0, TEXTURE_OP_MODULATE);
    NEngineInternal::GPRenderManager->SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);
    NEngineInternal::GPRenderManager->SetTextureArgument2(0, TEXTURE_ARG_DIFFUSE);
    NEngineInternal::GPRenderManager->SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);
    NEngineInternal::GPRenderManager->SetTextureAlphaArgument2(0, TEXTURE_ARG_DIFFUSE);
    NEngineInternal::GPRenderManager->SetTextureOperation(1, TEXTURE_OP_DISABLE);
    NEngineInternal::GPRenderManager->SetTextureAlphaOperation(1, TEXTURE_OP_DISABLE);
}