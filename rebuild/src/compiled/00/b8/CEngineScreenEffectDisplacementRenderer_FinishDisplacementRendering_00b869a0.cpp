// CEngineScreenEffectDisplacementRenderer::FinishDisplacementRendering() -- when active (+8): restore the display's render target (+0xf0) and viewport (+0x114), uninitialise the render target, and when the active flag has dropped meanwhile release the texture at +0x14 to the engine's texture pool (+0x3190 of the global) and clear +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B869A0, length 92 bytes.
// Callees: CDisplayManager::SetRenderTarget @ 0x009BF160 and SetViewport @ 0x009BEF80 (on the global @ 0x01436E1C, the members by address), CRenderTarget::Uninitialise
// @ 0x00A10E30 (on +0xf0), the pool release @ 0x00F36F00 (on the global @ 0x01436E98 + 0x3190, the texture; an empty `ret 4` in retail). Shape: the render target
// address stays in edi for the uninitialise call, the flag byte is re-read after it (`jne` skips the release), the +0x10 clear is an immediate.
struct CRenderTarget {
    unsigned char _body[0x24];
    void Uninitialise();                           // 0x00A10E30
};

struct CDisplayManager {
    void SetRenderTarget(CRenderTarget* target);   // 0x009BF160
    void SetViewport(void* viewport);              // 0x009BEF80
};
extern CDisplayManager* g_pDisplayManager_01436e1c;   // 0x01436E1C

struct CTexturePool_00f36f00 {
    void Release(void* texture);                   // 0x00F36F00
};

struct CEngineGlobal_01436e98 {
    unsigned char         _pad[0x3190];
    CTexturePool_00f36f00 m_texturePool;           // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CEngineScreenEffectDisplacementRenderer {
    unsigned char _pad[8];
    bool          m_active;                        // +0x08
    unsigned char _pad09[7];
    long          m_textureState;                  // +0x10
    void*         m_pTexture;                      // +0x14
    unsigned char _pad18[0xd8];
    CRenderTarget m_renderTarget;                  // +0xf0
    unsigned char m_viewport[0x10];                // +0x114
    void FinishDisplacementRendering();
};

void CEngineScreenEffectDisplacementRenderer::FinishDisplacementRendering()
{
    if (m_active) {
        g_pDisplayManager_01436e1c->SetRenderTarget(&m_renderTarget);
        g_pDisplayManager_01436e1c->SetViewport(m_viewport);
        m_renderTarget.Uninitialise();
        if (!m_active) {
            g_pEngine_01436e98->m_texturePool.Release(m_pTexture);
            m_textureState = 0;
        }
    }
}