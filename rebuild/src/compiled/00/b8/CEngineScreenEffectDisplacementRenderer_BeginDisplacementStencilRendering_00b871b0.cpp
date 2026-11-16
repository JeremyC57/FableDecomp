// CEngineScreenEffectDisplacementRenderer::BeginDisplacementStencilRendering() -- open the stencil-rendering capture block and set the eleven stencil/z render states the displacement mask pass needs
// Leaf: BeginDisplacementStencilRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B871B0, length 1232 bytes (body 0x4c8 + int3 padding), plain `ret`, no calls.
// Shape: StencilRenderingCaptureBlock (+0x144, engine header) open byte = 1, then the render-state manager @ 0x013CB508
//   capture push (stack of 8-byte entries at +8, count +0x2808, capture mask +0x2814 shifted left, capture block +0x3a24;
//   the same inline OpenCaptureBlock as the landed CEngineScreenEffectOutlineGlow::BeginOutlineRendering 0x00B89A20), then
//   ten inlined render-core SetState calls on the core @ 0x01436E18 (the landed CEngineStateBlockDiffuse2X::Apply 0x00B58FB0
//   helper: capture stack +0x08, dirty list +0x2008, counts +0x2808/+0x280C, capture mask +0x2814, 0x14-byte tracked states),
//   the last one (+0x2840 <- 0) only when the global byte @ 0x0143B8CC is clear.
//   Values: +0x2868<-1, +0x2854<-0, +0x2b88<-1, +0x2bd8<-[0x01396E34], +0x2b9c/+0x2bb0<-[0x01396F38], +0x2bc4<-[0x01396F40],
//   +0x2bec/+0x2c14/+0x2c00<-1.
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_STENCILRENDERINGCAPTUREBLOCK
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_CAPTURE_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_DISPLACEMENT_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#include "fable_compiler.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h): the local CRenderManagerCoreView / SetState
//   model and g_renderStateValue_<addr> table entries are the shared inline CRenderStateManager setters (repo opt-ins STENCIL_INLINE
//   / DISPLACEMENT_INLINE / OUTLINE_INLINE): +0x2868 ZBufferEnable, +0x2854 ZBufferWriteEnable, +0x2B88 StencilEnable, +0x2BD8
//   StencilFunc [ALWAYS], +0x2B9C / +0x2BB0 StencilOpFail / StencilOpZFail [KEEP], +0x2BC4 StencilOpPass [REPLACE], +0x2BEC
//   StencilRef, +0x2C14 StencilWriteMask, +0x2C00 StencilMask, +0x2840 ColourWriteEnable(false). The capture push on 0x013CB508 is
//   StencilRenderingCaptureBlock.Capture() (CRenderStateCaptureBlock::StateManager, engine/CRenderStateCaptureBlock.h
//   CAPTURE_INLINE: Open = true, then the inline StartCaptureBlock), through the overlay opt-in that types the +0x144 member (was a
//   local CRenderStateManager model and g_pRenderStateManager).
#include "engine/CRenderManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
extern unsigned char g_skipStencilState_0143b8cc;      // 0x0143B8CC

struct CEngineScreenEffectDisplacementRenderer_Stencil : CEngineScreenEffectDisplacementRenderer {
    void BeginDisplacementStencilRendering();
};

void CEngineScreenEffectDisplacementRenderer_Stencil::BeginDisplacementStencilRendering()
{
    StencilRenderingCaptureBlock.Capture();
    NEngineInternal::GPRenderManager->SetZBufferEnable(true);
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetStencilEnable(true);
    NEngineInternal::GPRenderManager->SetStencilFunc(STENCIL_FUNC_ALWAYS);
    NEngineInternal::GPRenderManager->SetStencilOpFail(STENCIL_OP_MODE_KEEP);
    NEngineInternal::GPRenderManager->SetStencilOpZFail(STENCIL_OP_MODE_KEEP);
    NEngineInternal::GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_REPLACE);
    NEngineInternal::GPRenderManager->SetStencilRef(1);
    NEngineInternal::GPRenderManager->SetStencilWriteMask(1);
    NEngineInternal::GPRenderManager->SetStencilMask(1);
    if (!g_skipStencilState_0143b8cc)
        NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
}