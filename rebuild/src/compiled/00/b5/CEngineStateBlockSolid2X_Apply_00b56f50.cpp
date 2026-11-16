// CEngineStateBlockSolid2X::Apply() -- set the 10 tracked render states of the Solid2X state block on the render core
// Leaf: ?Apply@CEngineStateBlockSolid2X@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B56F50, length 1104 bytes, plain `ret` (virtual, `this` unused).
// RTTI class: engine/CEngineStateBlocks.h FABLE_RETAIL_CENGINESTATEBLOCKS (CStateBlockFunctionBase); twin of the landed
//   CEngineStateBlockAlpha::Apply 0x00B573A0 (same body shape, different setter constants).
// Body: 10 inline CRenderStateManager setters (engine/CRenderStateManager.h FABLE_RETAIL_CRENDERSTATEMANAGER) on the render manager
//   singleton NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A;
//   CRenderManager : CRenderManager2D : CRenderManagerCore : CRenderStateManager, all at offset 0), re-read after every changed state.
//   Each setter maps the engine enum through its private static D3D table (D3DAlphaBlendModeTable 0x01396F68,
//   D3DTextureOperationModeTable 0x01396E70, D3DTextureArgumentModeTable 0x01396EE8). States in retail order: SourceAlphaBlendMode=ALPHA_BLEND_ONE, DestAlphaBlendMode=ALPHA_BLEND_ZERO, TextureOperation[0]=TEXTURE_OP_MODULATE_2X, TextureAlphaOperation[0]=TEXTURE_OP_MODULATE, TextureArgument1[0]=TEXTURE_ARG_TEXTURE, TextureArgument2[0]=TEXTURE_ARG_DIFFUSE, TextureAlphaArgument1[0]=TEXTURE_ARG_TEXTURE, TextureAlphaArgument2[0]=TEXTURE_ARG_DIFFUSE, TextureOperation[1]=TEXTURE_OP_DISABLE, TextureAlphaOperation[1]=TEXTURE_OP_DISABLE.
// NF45 (round 7): was a local CRenderManagerCoreView / CTrackedRenderState model with a local SetState and address-named
//   g_renderStateDefault_<addr> table entries; now the shared header's DeviceState setters.
#define FABLE_RETAIL_CENGINESTATEBLOCKS
#include "engine/CRenderManager.h"
#include "engine/CEngineStateBlocks.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

void CEngineStateBlockSolid2X::Apply()
{
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ZERO);
    NEngineInternal::GPRenderManager->SetTextureOperation(0, TEXTURE_OP_MODULATE_2X);
    NEngineInternal::GPRenderManager->SetTextureAlphaOperation(0, TEXTURE_OP_MODULATE);
    NEngineInternal::GPRenderManager->SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);
    NEngineInternal::GPRenderManager->SetTextureArgument2(0, TEXTURE_ARG_DIFFUSE);
    NEngineInternal::GPRenderManager->SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);
    NEngineInternal::GPRenderManager->SetTextureAlphaArgument2(0, TEXTURE_ARG_DIFFUSE);
    NEngineInternal::GPRenderManager->SetTextureOperation(1, TEXTURE_OP_DISABLE);
    NEngineInternal::GPRenderManager->SetTextureAlphaOperation(1, TEXTURE_OP_DISABLE);
}
