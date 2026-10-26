// CEngineScreenEffectRadialBlurRenderer::ReleaseTextures_00b5bd30() -- when the textures are held (+0xc): release both (+0x14 and +0x18) to the engine's texture pool (+0x3190 of the global) and clear the two held flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5BD30, length 62 bytes.
// Callees: the pool release @ 0x00F36F00 twice (on the global @ 0x01436E98 + 0x3190, kept in edi; edi is saved only inside the branch). Shape: the first texture is
// loaded before the pool address is formed; the two clears are immediates. The chunk label "FinishWithDynamicTextures" is propagated; named by effect.
struct CTexturePool_00f36f00 {
    void Release(void* texture);                   // 0x00F36F00 (an empty `ret 4` in retail)
};

struct CEngineGlobal_01436e98 {
    unsigned char         _pad[0x3190];
    CTexturePool_00f36f00 m_texturePool;           // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

class CEngineScreenEffectRadialBlurRenderer { public:
    unsigned char _pad[0xc];
    long          m_held;                          // +0x0c
    long          m_heldB;                         // +0x10
    void*         m_pTextureA;                     // +0x14
    void*         m_pTextureB;                     // +0x18
    void ReleaseTextures_00b5bd30();
};

void CEngineScreenEffectRadialBlurRenderer::ReleaseTextures_00b5bd30()
{
    if (m_held) {
        CTexturePool_00f36f00* pool = &g_pEngine_01436e98->m_texturePool;
        pool->Release(m_pTextureA);
        pool->Release(m_pTextureB);
        m_held = 0;
        m_heldB = 0;
    }
}