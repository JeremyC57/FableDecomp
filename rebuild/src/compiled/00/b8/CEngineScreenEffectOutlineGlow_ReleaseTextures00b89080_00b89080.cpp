// CEngineScreenEffectOutlineGlow::ReleaseTextures_00b89080() -- for each of the three held flags at +0xc / +0x10 / +0x14, release the matching texture (+0x18 / +0x1c / +0x20) to the engine's texture pool (+0x3190 of the global) and clear the flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B89080, length 94 bytes.
// Callees: the pool release @ 0x00F36F00 three times (on the global @ 0x01436E98 + 0x3190, kept in edi). Shape: the pool address is formed before the first test; each
// arm pushes its texture, calls, and clears its flag with an immediate. The chunk label "FinishWithDynamicTextures" is propagated; named by effect.
struct CTexturePool_00f36f00 {
    void Release(void* texture);                   // 0x00F36F00 (an empty `ret 4` in retail)
};

struct CEngineGlobal_01436e98 {
    unsigned char         _pad[0x3190];
    CTexturePool_00f36f00 m_texturePool;           // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

class CEngineScreenEffectOutlineGlow { public:
    unsigned char _pad[0xc];
    long          m_heldA;                         // +0x0c
    long          m_heldB;                         // +0x10
    long          m_heldC;                         // +0x14
    void*         m_pTextureA;                     // +0x18
    void*         m_pTextureB;                     // +0x1c
    void*         m_pTextureC;                     // +0x20
    void ReleaseTextures_00b89080();
};

void CEngineScreenEffectOutlineGlow::ReleaseTextures_00b89080()
{
    CTexturePool_00f36f00* pool = &g_pEngine_01436e98->m_texturePool;
    if (m_heldA) {
        pool->Release(m_pTextureA);
        m_heldA = 0;
    }
    if (m_heldB) {
        pool->Release(m_pTextureB);
        m_heldB = 0;
    }
    if (m_heldC) {
        pool->Release(m_pTextureC);
        m_heldC = 0;
    }
}