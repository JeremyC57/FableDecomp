// CLocalDetailCacheMap_00b6d600::CloseStaticMap() -- hand the memory page at +0x1d8 back to the GPU scratch-buffer texture manager embedded in the engine global 0x01436E98 at +0x3190 (FreeMemoryPage @ 0x00F37030) and clear it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6D600, length 48 bytes.
// Callees: CGPUScratchBufferTextureManager::FreeMemoryPage @ 0x00F37030. The chunk label is propagated; named by address. Shape: `this` in esi, the page loaded before the global, `ret`.
struct CGPUScratchBufferTextureManager {
    void FreeMemoryPage(void* page);               // 0x00F37030
};
struct SEngine_01436e98 {
    unsigned char                   _pad[0x3190];
    CGPUScratchBufferTextureManager m_scratchTextures;   // +0x3190
};
extern SEngine_01436e98* g_pEngine_01436e98;      // 0x01436E98

struct CLocalDetailCacheMap_00b6d600 {
    unsigned char _pad00[0x1d8];
    void*         m_pMemoryPage;                   // +0x1d8
    void CloseStaticMap();
};

void CLocalDetailCacheMap_00b6d600::CloseStaticMap()
{
    g_pEngine_01436e98->m_scratchTextures.FreeMemoryPage(m_pMemoryPage);
    m_pMemoryPage = 0;
}
