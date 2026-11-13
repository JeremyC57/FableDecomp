#ifndef FABLE_ENGINE_CGPUSCRATCHBUFFERTEXTUREMANAGER_H
#define FABLE_ENGINE_CGPUSCRATCHBUFFERTEXTUREMANAGER_H
// New header (chunk 767, row 0x00B3AFE0 CEngineResourceManager::OnReleaseDefaultPoolResources). The resource manager's
// GPUScratchBufferTextureManager member (retail +0x3190, 0x44 bytes: engine/CEngineResourceManager.h). Members are not modelled yet
// (one opaque block); only the members rows call are declared, with their Ego_r.pdb / FableWin.pdb spellings:
// ?BeginShutdown@CGPUScratchBufferTextureManager@@QAE_NXZ (retail 0x00F37170, landed).
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
// Retail opt-in (chunk 2018, rows 0x00B2B180 / 0x00B2CB40 / 0x00B2D330 CEngineSurfaceCompositionManager): Ego_r.pdb / FableWin.pdb
// spellings. AllocteMemoryPage (sic, both PDBs) 0x00F37660, AllocateTexture 0x00F37440; StartTextureUse 0x00F36EE0 returns the
// entry's texture (+4 of the 0x2C-byte entry; Ego_r donor order StartResourceUse 0x00B3C350 / EndResourceUse / StartTextureUse
// 0x00B3C370 / EndTextureUse / StartSurfaceUse 0x00B3C3A0 / EndSurfaceUse = retail ret-4 0x00F36EC0 / 0x00F36ED0 / 0x00F36EE0 /
// ret-4 0x00F36F00 / 0x00F36F10 (+8) / ...); EndTextureUse is the empty `ret 4` 0x00F36F00. The landed bindings
// StartSurfaceUse / EndSurfaceUse at those two addresses are wrong (NAME_FIXES.tsv).
class C2DExtentsI; class CPixelFormat; class CTexture; class CSurface;
#endif
class CGPUScratchBufferTextureManager {
public:
    bool BeginShutdown();                                    // 0x00F37170
#ifdef FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
    long AllocteMemoryPage(long size, bool is_render_target);                                            // 0x00F37660
    long AllocateTexture(long page, const C2DExtentsI& extents, long mipmap_levels, const CPixelFormat& format,
                         unsigned long usage, bool is_compressed);                                          // 0x00F37440
    CTexture* StartTextureUse(long handle);                                                             // 0x00F36EE0
    void EndTextureUse(long handle);                                                                    // 0x00F36F00
#endif
private:
    unsigned char Unmodeled_0x00[0x44];
};
FABLE_STATIC_ASSERT(sizeof(CGPUScratchBufferTextureManager) == 0x44);
#endif // FABLE_ENGINE_CGPUSCRATCHBUFFERTEXTUREMANAGER_H
