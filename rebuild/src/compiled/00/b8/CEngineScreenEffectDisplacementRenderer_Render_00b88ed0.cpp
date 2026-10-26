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
extern CDisplayManager* g_pDisplayManager_01436e1c;   // 0x01436E1C

struct CTexturePool_00f36f00 {
    void Release(long texture);                    // 0x00F36F00
};
struct CEngineGlobal_01436e98 {
    unsigned char         _pad[0x3190];
    CTexturePool_00f36f00 m_texturePool;           // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CEngineScreenEffectDisplacementRenderer_Render : CEngineScreenEffectDisplacementRenderer {
    void ApplyDisplacement();                      // 0x00B87870
    void ShowDisplacementTexture();                // 0x00B88BC0
    void ReleaseDynamicTextures()
    {
        CTexturePool_00f36f00* pool = &g_pEngine_01436e98->m_texturePool;
        if (BackBufferCopyTexture)
            BackBufferCopyTexture = 0;
        if (DisplacementTexture) {
            pool->Release(DisplacementTextureHandle);
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
                BackBufferCopyTexture = g_pDisplayManager_01436e1c->CopyBackBufferToTexture();
                ApplyDisplacement();
                ShowDisplacementTexture();
                HasContent = false;
            }
            ReleaseDynamicTextures();
        }
    }
}