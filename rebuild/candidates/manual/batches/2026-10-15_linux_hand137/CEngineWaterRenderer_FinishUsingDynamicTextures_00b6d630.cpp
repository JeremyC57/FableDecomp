// CEngineWaterRenderer::FinishUsingDynamicTextures() -- hand the four dynamic water textures back to the engine's texture pool and clear their slots
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6D630, length 103 bytes (+ int3 padding).
// Callees: the texture pool release @ 0x00F36F00 four times (on the global @ 0x01436E98 + 0x3190, kept in edi).
// Layout: four (texture, slot) word pairs at +0x1dc/+0x1e0, +0x1e4/+0x1e8, +0x1ec/+0x1f0, +0x1f4/+0x1f8. The PDB layout
// (engine/CEngineWaterRenderer.h) puts the shader objects at +0x1d4/+0x1dc and (handle, CTexture*) pairs from +0x1e4,
// which does not line up with these retail accesses (the retail block sits 8 bytes lower and is (texture, slot)
// ordered), so the block is modelled locally.
// Shape: the pool address is formed once into a local (edi); each release is followed by its slot clear, which
// VC7.1 schedules before the NEXT push (the zero lives in ebx); the order is +0x1dc, +0x1ec, +0x1e4, +0x1f4.
struct CTexturePool_00f36f00 {
    void Release(void* texture);                   // 0x00F36F00
};

struct CEngineGlobal_01436e98 {
    unsigned char         _pad[0x3190];
    CTexturePool_00f36f00 m_texturePool;           // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CEngineWaterRenderer {
    unsigned char _pad[0x1dc];
    void*         m_pReflectionTexture;            // +0x1dc
    long          m_reflectionSlot;                // +0x1e0
    void*         m_pRefractionTexture;            // +0x1e4
    long          m_refractionSlot;                // +0x1e8
    void*         m_pSeaBumpTexture;               // +0x1ec
    long          m_seaBumpSlot;                   // +0x1f0
    void*         m_pWaterBumpTexture;             // +0x1f4
    long          m_waterBumpSlot;                 // +0x1f8
    void FinishUsingDynamicTextures();
};

void CEngineWaterRenderer::FinishUsingDynamicTextures()
{
    CTexturePool_00f36f00* pool = &g_pEngine_01436e98->m_texturePool;
    pool->Release(m_pReflectionTexture);
    m_reflectionSlot = 0;
    pool->Release(m_pSeaBumpTexture);
    m_seaBumpSlot = 0;
    pool->Release(m_pRefractionTexture);
    m_refractionSlot = 0;
    pool->Release(m_pWaterBumpTexture);
    m_waterBumpSlot = 0;
}
