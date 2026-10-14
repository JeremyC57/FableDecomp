// CEngineScreenEffectDisplacementRenderer::ReleaseDynamicTextures_00b86a30() -- clear the handle at +0xc when held; when the texture state at +0x10 is set release the texture at +0x14 to the engine's texture pool (+0x3190 of the global) and clear the state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86A30, length 54 bytes.
// Callees: the pool release @ 0x00F36F00 (on the global @ 0x01436E98 + 0x3190; an empty `ret 4` in retail). Shape: the pool address is formed first (`add ecx,3190h`)
// and kept in ecx across both tests; the two clears are immediates. The chunk label "FinishWithDynamicTextures" is propagated; named by effect.
struct CTexturePool_00f36f00 {
    void Release(void* texture);                   // 0x00F36F00
};

struct CEngineGlobal_01436e98 {
    unsigned char         _pad[0x3190];
    CTexturePool_00f36f00 m_texturePool;           // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CEngineScreenEffectDisplacementRenderer {
    unsigned char _pad[0xc];
    void*         m_pHandle;                       // +0x0c
    long          m_textureState;                  // +0x10
    void*         m_pTexture;                      // +0x14
    void ReleaseDynamicTextures_00b86a30();
};

void CEngineScreenEffectDisplacementRenderer::ReleaseDynamicTextures_00b86a30()
{
    CTexturePool_00f36f00* pool = &g_pEngine_01436e98->m_texturePool;
    if (m_pHandle)
        m_pHandle = 0;
    if (m_textureState) {
        pool->Release(m_pTexture);
        m_textureState = 0;
    }
}
