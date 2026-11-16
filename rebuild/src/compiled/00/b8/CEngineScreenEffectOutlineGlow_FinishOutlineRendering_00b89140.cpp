// CEngineScreenEffectOutlineGlow::FinishOutlineRendering() -- when the effect is active: close the outline render-state capture block, restore the display's previous render target and viewport, uninitialise the saved target, and if the effect was switched off meanwhile release the outline texture to the engine's texture pool
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B89140, length 110 bytes (+ int3 padding).
// Callees: CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 (global @ 0x013CB508), CDisplayManager::SetRenderTarget
// @ 0x009BF160 and SetViewport @ 0x009BEF80 (global @ 0x01436E1C), CRenderTarget::Uninitialise @ 0x00A10E30, the texture
// pool release @ 0x00F36F00 (engine global @ 0x01436E98 + 0x3190).
// Layout: engine/CEngineScreenEffectOutlineGlow.h -- OutlineTexture +0x0c, OutlineTextureHandle +0x18, OldRenderTarget
// +0x14c, OldViewport +0x170, OutlineRenderingCaptureBlock_Open +0x1e0; the active flag is the retail-only byte at +0x08
// (the header's `_pad_0x08`, read through an accessor). The method is attached through a derived shell.
// Shape: as the landed CEngineScreenEffectDisplacementRenderer::FinishDisplacementRendering @ 0x00B869A0: the render
// target address stays in edi for the uninitialise call, the active byte is re-read after it; the capture block is
// closed by clearing its Open byte before the manager call; the release pushes the handle before forming the pool.
#include "engine/CEngineScreenEffectOutlineGlow.h"

class CRenderStateManager { public:
    void RestoreCaptureBlock();                    // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508

class CRenderTarget { public:
    void Uninitialise();                           // 0x00A10E30
};

class CDisplayManager { public:
    void SetRenderTarget(CRenderTarget* target);   // 0x009BF160
    void SetViewport(void* viewport);              // 0x009BEF80
};
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A

// NF44 (round 6): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) and its +0x3190 member the CGPUScratchBufferTextureManager
// (engine/CEngineResourceManager.h GetGPUScratchBufferTextureManager(), engine/CGPUScratchBufferTextureManager.h, Ego_r.pdb member
// spellings); were the address-named CEngineGlobal_01436e98 / g_pEngine_01436e98 and local scratch-manager stand-ins.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDTEXTUREUSE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

struct CEngineScreenEffectOutlineGlow_Finish : CEngineScreenEffectOutlineGlow {
    unsigned char IsActive() const { return _pad_0x08[0]; }
    void FinishOutlineRendering();
};

void CEngineScreenEffectOutlineGlow_Finish::FinishOutlineRendering()
{
    if (IsActive()) {
        OutlineRenderingCaptureBlock_Open = false;
        g_pRenderStateManager->RestoreCaptureBlock();
        CRenderTarget* target = (CRenderTarget*)OldRenderTarget;
        NEngineInternal::GPDisplayManager->SetRenderTarget(target);
        NEngineInternal::GPDisplayManager->SetViewport(&OldViewport_TLX);
        target->Uninitialise();
        if (!IsActive()) {
            NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().EndTextureUse(OutlineTextureHandle);
            OutlineTexture = 0;
        }
    }
}