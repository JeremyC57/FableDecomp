// CLocalDetailCacheMap_00b6d600::CloseStaticMap() -- hand the memory page at +0x1d8 back to the GPU scratch-buffer texture manager embedded in the engine global 0x01436E98 at +0x3190 (FreeMemoryPage @ 0x00F37030) and clear it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6D600, length 48 bytes.
// Callees: CGPUScratchBufferTextureManager::FreeMemoryPage @ 0x00F37030. The chunk label is propagated; named by address. Shape: `this` in esi, the page loaded before the global, `ret`.
// NF44 (round 6): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) and its +0x3190 member the CGPUScratchBufferTextureManager
// (engine/CEngineResourceManager.h GetGPUScratchBufferTextureManager(), engine/CGPUScratchBufferTextureManager.h, Ego_r.pdb member
// spellings); were the address-named SEngine_01436e98 / g_pEngine_01436e98 and local scratch-manager stand-ins.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDTEXTUREUSE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

struct CLocalDetailCacheMap_00b6d600 {
    unsigned char _pad00[0x1d8];
    long          m_pMemoryPage;                   // +0x1d8 (the page index FreeMemoryPage takes)
    void CloseStaticMap();
};

void CLocalDetailCacheMap_00b6d600::CloseStaticMap()
{
    NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().FreeMemoryPage(m_pMemoryPage);
    m_pMemoryPage = 0;
}