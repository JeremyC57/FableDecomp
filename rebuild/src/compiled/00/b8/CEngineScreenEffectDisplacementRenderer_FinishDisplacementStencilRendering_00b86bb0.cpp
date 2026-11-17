// CEngineScreenEffectDisplacementRenderer::FinishDisplacementStencilRendering() -- end the stencil pass: restore the render states captured in StencilRenderingCaptureBlock (+0x144)
// Leaf: ?FinishDisplacementStencilRendering@CEngineScreenEffectDisplacementRenderer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86BB0, 18 bytes.
// Retail bytes:  mov byte ptr [ecx+144h],0 ; mov ecx,[013CB508h] ; jmp CRenderStateManager::RestoreCaptureBlock
// Identity (NF56, round 7, queue q10419): Ego_r.pdb 0x00A2D730 / FableWin.pdb 0x02DBA380
//   ?FinishDisplacementStencilRendering@CEngineScreenEffectDisplacementRenderer@@QAEXXZ: the +0x144 member is Ego_r.pdb's
//   StencilRenderingCaptureBlock (engine/CEngineScreenEffectDisplacementRenderer.h, sizeof 0x1AC), and the body is its inline
//   CRenderStateCaptureBlock::Restore (engine/CRenderStateCaptureBlock.h, FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE + _RESTORE_OPEN_FIRST:
//   Open cleared, then the tail call ?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ 0x00A05840 on the private static StateManager
//   0x013CB508). Was the unattributed stand-in CCaptureUser_Retail::ClearFlagAndRestoreCaptureBlock (NF45 / NF53 kept the owner
//   descriptive); the owner now sits on its shared header (overlay opt-in _FINISHSTENCIL types the member).
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_FINISHSTENCIL
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_RESTORE_OPEN_FIRST
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

void CEngineScreenEffectDisplacementRenderer::FinishDisplacementStencilRendering()
{
    StencilRenderingCaptureBlock.Restore();
}
