// CEngineScreenEffectDisplacementRenderer::Render(const CEngineComponentRenderParams&) -- during the two post-processing passes: when displacement content was drawn this frame, copy the back buffer, apply the displacement and show it, then release this frame's dynamic textures
// Leaf: Render
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B88ED0, length 123 bytes; virtual
// `?Render@CEngineScreenEffectDisplacementRenderer@@UAEXABVCEngineComponentRenderParams@@@Z` (`ret 4`).
// Callees: CDisplayManager::CopyBackBufferToTexture @ 0x009BF5E0 (global @ 0x01436E1C), ApplyDisplacement @ 0x00B87870,
// ShowDisplacementTexture @ 0x00B88BC0, the texture pool release @ 0x00F36F00 (engine global @ 0x01436E98 + 0x3190).
// Layout: engine/CEngineScreenEffectDisplacementRenderer.h -- BackBufferCopyTexture +0x0c, DisplacementTexture +0x10,
// DisplacementTextureHandle +0x14, HasContent +0x1c; the enabled flag is the retail-only byte at +0x08 (the header's
// `_pad_0x08`); the render pass id is the params' second word. The method is attached through a derived shell.
// Shape: the pass test is `pass == 0x20000000 || pass == 0x10000`; the tail is the inlined ReleaseDynamicTextures (landed
// out of line @ 0x00B86A30: pool formed first, `if (BackBufferCopyTexture) BackBufferCopyTexture = 0;`, then the
// displacement texture released and cleared).
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

class CEngineComponentRenderParams { public:
    long          Params;
    unsigned long m_pass;                          // +0x04
};

class CDisplayManager { public:
    CTexture* CopyBackBufferToTexture();           // 0x009BF5E0
};
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A

// NF46 (round 7, NF44's scratch.py): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
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

struct CEngineScreenEffectDisplacementRenderer_Render : CEngineScreenEffectDisplacementRenderer {
    void ApplyDisplacement();                      // 0x00B87870
    void ShowDisplacementTexture();                // 0x00B88BC0
    void ReleaseDynamicTextures()
    {
        CGPUScratchBufferTextureManager* pool = &NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
        if (BackBufferCopyTexture)
            BackBufferCopyTexture = 0;
        if (DisplacementTexture) {
            pool->EndTextureUse(DisplacementTextureHandle);
            DisplacementTexture = 0;
        }
    }
    void Render(const CEngineComponentRenderParams& params);
};

void CEngineScreenEffectDisplacementRenderer_Render::Render(const CEngineComponentRenderParams& params)
{
    if (_pad_0x08[0]) {
        unsigned long pass = params.m_pass;
        if (pass == 0x20000000 || pass == 0x10000) {
            if (HasContent) {
                BackBufferCopyTexture = NEngineInternal::GPDisplayManager->CopyBackBufferToTexture();
                ApplyDisplacement();
                ShowDisplacementTexture();
                HasContent = false;
            }
            ReleaseDynamicTextures();
        }
    }
}