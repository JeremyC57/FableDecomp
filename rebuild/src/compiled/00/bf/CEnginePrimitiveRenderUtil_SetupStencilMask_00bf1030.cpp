// CEnginePrimitiveRenderUtil::SetupStencilMask(CEngineMeshEffect* effect, unsigned long flags, ERenderPassType pass) -- for the opaque pass (1), or the alpha pass (2) of a mesh without flag 4, set the five stencil tracked render states: enable 1, func/ref from the globals 0x01396E34/0x01396F40, write mask 0xe (| the effect's stencil bit << 2 when flag 0x20) and the stencil reference (2 when flag 0x40, else 0)
// Leaf: SetupStencilMask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF1030, length 624 bytes (body 0x26e), `ret 0xc` (`this` unused).
// Render core @ 0x01436E18: the model of the landed CEngineStateBlockDiffuseOnly::Apply 0x00B58DE0 (capture stack +0x08,
//   dirty list +0x2008, counts +0x2808/+0x280C, capture mask +0x2814, 0x14-byte tracked states) with the `__forceinline`
//   SetState(state, value, core), core LAST. States: +0x2b88, +0x2bd8, +0x2bc4, +0x2c14, +0x2bec.
// Mesh effect: CEngineMeshEffectOutlineBase (Ego_r.pdb) keeps StencilBit at +0x18; the stencil reference lives in the dead `pass`
//   argument slot, as retail.
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h); the local CRenderManagerCoreView / SetState
//   model and the g_renderStateDefault_<addr> table entries are the shared inline CRenderStateManager stencil setters (repo opt-ins
//   STENCIL_INLINE / OUTLINE_INLINE). DeviceState offsets: +0x2B88 StencilEnable, +0x2BD8 StencilFunc (D3DStencilFuncModeTable
//   0x01396E18 [ALWAYS]), +0x2BC4 StencilOpPass (D3DStencilOperationModeTable 0x01396F38 [REPLACE]), +0x2C14 StencilWriteMask,
//   +0x2BEC StencilRef: the old model had the +0x2BC4 / +0x2BEC labels swapped, so the flag-0x40 value is the stencil reference.
#define FABLE_RETAIL_CRENDERSTATEMANAGER_STENCIL_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#include "engine/CRenderManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

class CEngineMeshEffect { public:
    unsigned char _pad00[0x18];
    unsigned char StencilBit;                      // +0x18 (CEngineMeshEffectOutlineBase)
};
enum ERenderPassType { RENDER_PASS_OPAQUE = 1, RENDER_PASS_ALPHA = 2 };

class CEnginePrimitiveRenderUtil {
public:
    void SetupStencilMask(CEngineMeshEffect* effect, unsigned long flags, ERenderPassType pass);
};

void CEnginePrimitiveRenderUtil::SetupStencilMask(CEngineMeshEffect* effect, unsigned long flags, ERenderPassType pass)
{
    if ((pass == RENDER_PASS_ALPHA && (flags & 4) == 0) || pass == RENDER_PASS_OPAQUE) {
        unsigned long writeMask = 0xe;
        unsigned long stencilRef = 0;
        if (flags & 0x40)
            stencilRef = 2;
        if (flags & 0x20)
            writeMask = (1 << (effect->StencilBit + 2)) | 0xe;
        NEngineInternal::GPRenderManager->SetStencilEnable(true);
        NEngineInternal::GPRenderManager->SetStencilFunc(STENCIL_FUNC_ALWAYS);
        NEngineInternal::GPRenderManager->SetStencilOpPass(STENCIL_OP_MODE_REPLACE);
        NEngineInternal::GPRenderManager->SetStencilWriteMask(writeMask);
        NEngineInternal::GPRenderManager->SetStencilRef(stencilRef);
    }
}