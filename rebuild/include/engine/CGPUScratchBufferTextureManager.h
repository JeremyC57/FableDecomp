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
#ifdef FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ALLOCATEDEPTHBUFFERSURFACE
#include "engine/ESurfaceMultisampleType.h"
#endif
#ifdef FABLE_RETAIL_GPU_SCRATCH_RESOURCES_PASS29
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CArray.h"
#include "engine/CTexture.h"
#include "engine/ESurfaceMultisampleType.h"
#include "engine/CSurface.h"
class CGPUScratchBufferTextureManager : public CBaseClassNonCopyable {
#else
class CGPUScratchBufferTextureManager {
#endif
public:
#ifdef FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_CTOR
    // Retail opt-in (wfE7, row 0x00B3CB30): Ego_r.pdb / FableWin.pdb ??0CGPUScratchBufferTextureManager@@QAE@XZ (retail 0x00F37520) and
    // ?AddScratchPool@CGPUScratchBufferTextureManager@@QAEJJ@Z (retail 0x00F370C0), called by the CEngineResourceManager ctor. Declarations only.
    CGPUScratchBufferTextureManager();                                   // 0x00F37520
    long AddScratchPool(long size);                                      // 0x00F370C0
#endif
    bool BeginShutdown();                                    // 0x00F37170
#ifdef FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDSHUTDOWN
    // NF172 opt-in (row 0x00B3AFF0): Ego_r.pdb 0x00B3C1D0 / FableWin.pdb 0x0316CC20 ?EndShutdown@CGPUScratchBufferTextureManager@@QAE_NXZ
    // (retail 0x00F36D30, not a landed row: it re-creates the scratch textures, `mov al,1 ; ret` / `xor al,al ; ret`). Declaration only.
    bool EndShutdown();                                      // 0x00F36D30
#endif
#ifdef FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
    long AllocteMemoryPage(long size, bool is_render_target);                                            // 0x00F37660
    long AllocateTexture(long page, const C2DExtentsI& extents, long mipmap_levels, const CPixelFormat& format,
                         unsigned long usage, bool is_compressed);                                          // 0x00F37440
    CTexture* StartTextureUse(long handle);                                                             // 0x00F36EE0
    void EndTextureUse(long handle);                                                                    // 0x00F36F00
#endif
#ifdef FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ALLOCATEDEPTHBUFFERSURFACE
    // Opt-in (NF46 round 7, row 0x00BABB50 CEnginePrimitiveManagerRepeatedZSprites ctor): Ego_r.pdb / FableWin.pdb
    // ?AllocateDepthBufferSurface@CGPUScratchBufferTextureManager@@QAEJJABVC2DExtentsI@@ABVCPixelFormat@@W4ESurfaceMultisampleType@@@Z
    // (retail 0x00F37390). Declaration only.
    long AllocateDepthBufferSurface(long page, const class C2DExtentsI& extents, const class CPixelFormat& format,
                                    ESurfaceMultisampleType multisample_type);                          // 0x00F37390
#endif
#if defined(FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE) && !defined(FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES)
    // Opt-in (bwf2, row 0x00B72180): Ego_r.pdb 0x00B3C370 ?StartTextureUse@CGPUScratchBufferTextureManager@@QAEPAVCTexture@@J@Z
    // (retail 0x00F36EE0: returns the handle's entry +4 = the texture; 0x00F36F10 returns +8 = StartSurfaceUse). The same declaration as
    // the _TEXTURES opt-in above (chunk 2018), so it is skipped when both are defined.
    class CTexture* StartTextureUse(long handle);                        // 0x00F36EE0
#endif
#ifdef FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
    // Opt-in (NF44 round 6, the CEngineScreenEffect* InitialiseTextures / SetEffectQuality rows 0x00B5BFD0 ..): Ego_r.pdb
    // ?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z (retail 0x00F37030). Declaration only.
    void FreeMemoryPage(long page);                                      // 0x00F37030
#endif
#if defined(FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDTEXTUREUSE) && !defined(FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES)
    // Opt-in (NF44 round 6, rows 0x00B5BD30 ..): Ego_r.pdb ?EndTextureUse@CGPUScratchBufferTextureManager@@QAEXJ@Z (retail 0x00F36F00, an
    // empty `ret 4`). The same declaration as the _TEXTURES opt-in above, so it is skipped when both are defined.
    void EndTextureUse(long handle);                                     // 0x00F36F00
#endif
#ifdef FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_SURFACEUSE
    // Opt-in (NF44 round 6, row 0x00BAC0F0 RenderNonsortedPrimitives): Ego_r.pdb ?StartSurfaceUse@CGPUScratchBufferTextureManager@@QAEPAVCSurface@@J@Z
    // (retail 0x00F36F10, the entry's +8) and ?EndSurfaceUse@CGPUScratchBufferTextureManager@@QAEXJ@Z (retail 0x00F36F30). Declarations only.
    class CSurface* StartSurfaceUse(long handle);                        // 0x00F36F10
    void EndSurfaceUse(long handle);                                     // 0x00F36F30
#endif
private:
#ifdef FABLE_RETAIL_GPU_SCRATCH_RESOURCES_PASS29
    enum EResourceType { RESOURCE_TYPE_NOT_ALLOCATED, RESOURCE_TYPE_TEXTURE, RESOURCE_TYPE_DEPTH_BUFFER_SURFACE };
    struct CResourceEntry {
        EResourceType ResourceType;
        CTexture* Texture;
        CSurface* Surface;
        long PageIndex;
        C2DExtentsI Dimensions;
        long NoLevels;
        CPixelFormat PixelFormat;
        ESurfaceMultisampleType MultiSampleType;
        unsigned long UseageFlags;
        ESurfacePool MemoryPool;
    };
    struct CResourcePage;
    struct CScratchPool;
    CArray<CResourceEntry> Resources;
    CArray<CResourcePage> MemoryPages;
    CArray<CScratchPool> ScratchPools;
    CArray<long> FreeTexturePageSlots, FreeTextureSlots;
    bool ShutdownFlag;
#else
    unsigned char Unmodeled_0x00[0x44];
#endif
};
FABLE_STATIC_ASSERT_AT(36, sizeof(CGPUScratchBufferTextureManager) == 0x44);
#endif // FABLE_ENGINE_CGPUSCRATCHBUFFERTEXTUREMANAGER_H
