#ifndef FABLE_ENGINE_CENGINESURFACECOMPOSITIONMANAGER_H
#define FABLE_ENGINE_CENGINESURFACECOMPOSITIONMANAGER_H
#ifdef FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
// Retail branch (chunk 731, 2026-11-03): RTTI .?AVCEngineSurfaceCompositionManager@@ (vtable 0x012A1200) : CResourceBank (+0x0), CEngineComponent (+0x7C); sizeof 0x10DC from
// CEngine::Initialise 0x00B26360 (`new` of 0x10DC bytes before the constructor call). Constructor ??0CEngineSurfaceCompositionManager@@QAE@XZ 0x00B2CB40 and
// the PeekSceneFilterFlags override (?PeekSceneFilterFlags@CEngineSurfaceCompositionManager@@UBEKXZ): Ego_r.pdb / FableWin.pdb. Own members are
// not modelled yet (one opaque block; the generated Ego_r model below lists their names).
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "rebuild_abi.h"
#include "engine/CResourceBank.h"
#include "engine/CEngineComponent.h"
#ifdef FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS
// Retail opt-in (chunk 2018, rows 0x00B2B180 RenderToTexture / 0x00B2CB40 the constructor / 0x00B2D330 UpdateDiskCacheTexture): the
// own members typed, Ego_r.pdb names and order at the retail offsets of the generated default below (and of the landed
// ~CEngineSurfaceCompositionManager 0x00B2D1B0 local model): the cache list (STLport list, node malloc(0x10) at 0x00B2CB76), the three
// CGraphicDataBank (ctor 0x009FEA20 x3), three CWideString (0x0099AED0 x3), the morph map (header node malloc(0x2C)), the texture
// manager (0x00A6A360), four CPixelFormat (-1 stores), the screen-copy vertex shader (0x00A62FB0), the two pair<long,long>[4][4]
// handle tables (16-iteration zero loops) and the tail scalars. Needs the CGraphicDataBank / CTextureManager / CBankFile opt-ins.
#ifndef FABLE_RETAIL_CGRAPHICDATABANK_SURFACECOMPOSITION
#define FABLE_RETAIL_CGRAPHICDATABANK_SURFACECOMPOSITION
#endif
#ifndef FABLE_RETAIL_CTEXTUREMANAGER_CLASS
#define FABLE_RETAIL_CTEXTUREMANAGER_CLASS
#endif
#ifndef FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXSHADER
#endif
#ifndef FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT
#endif
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CTextureManager.h"
#include "engine/CPixelFormat.h"
#include "engine/CVertexShader.h"
#include "engine/CEngineTextureMorph.h"
class CCompositeSurfaceCache;
class CGraphicInfoFrame;
class CTexture;
class C2DExtentsI;
class CGraphicFrame;
template <class T> class CArray;
#elif defined(FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_BANKS)
#include "engine/CGraphicDataBank.h"
#endif
class CEngineInit;
class CEngineSurfaceCompositionManager : public CResourceBank, public CEngineComponent {
public:
    CEngineSurfaceCompositionManager();        // 0x00B2CB40 ??0CEngineSurfaceCompositionManager@@QAE@XZ
    virtual unsigned long PeekSceneFilterFlags() const;
#if defined(FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS)
    virtual ~CEngineSurfaceCompositionManager();                         // 0x00B2D1B0 (landed)
    void ChangeSceneForTextureBank();                                    // 0x00B2AD80
private:
    // ?RenderToTexture@CEngineSurfaceCompositionManager@@AAEXAAVCTexture@@ABVC2DExtentsI@@_N2ABVCEngineTextureMorph@@ABV?$CArray@VCGraphicFrame@@@@J@Z
    // (Ego_r.pdb; private; retail `ret 1Ch`).
    void RenderToTexture(CTexture& texture, const C2DExtentsI& extents, bool alpha, bool unused_flag,
                         const CEngineTextureMorph& morph, const CArray<CGraphicFrame>& frames, long scratch_texture);   // 0x00B2B180
    // Ego_r.pdb / FableWin.pdb private members (AAE), the spellings UpdateDiskCacheTexture calls (NAME_FIXES.tsv for the landed ones):
    void GetDimensionsAndAdjustMipmapLevel(C2DExtentsI& dimensions, const CGraphicInfoFrame* frame, unsigned long& mipmap_level);   // 0x00B2AEF0
    bool IsNearlyIdentical(const CEngineTextureMorph& a, const CEngineTextureMorph& b, float tolerance);   // 0x00B2B050
    const CGraphicInfoFrame* GetGraphicInfo(unsigned long index);                                         // 0x00B2B790
    void WriteTextureToBank(CTexture& texture, CGraphicDataBank& bank, unsigned long index, bool alpha);  // 0x00B2BD80
    bool UpdateDiskCacheTexture(CCompositeSurfaceCache& surface_cache, unsigned long mipmap_level, bool is_bump);   // 0x00B2D330
public:
    std::list<CCountedPointer<CCompositeSurfaceCache> >      CachedSurfaces;                  // +0x88
    CGraphicDataBank                                         IntermediateBankFile;            // +0x8c
    CGraphicDataBank                                         CompletedBankFile;               // +0x398
    CGraphicDataBank                                         CustomTextureBankFile;           // +0x6a4
    CWideString                                              IntermediateDiskCacheFileName;   // +0x9b0
    CWideString                                              CompletedDiskCacheFileName;      // +0x9b4
    CWideString                                              CustomTextureBankFileName;       // +0x9b8
    std::map<std::pair<long, long>, CEngineTextureMorph>     GeneratedMorphs;                 // +0x9bc
    float                                                    DiskCacheTolerance;              // +0x9c8
    float                                                    MaxPreloadTime;                  // +0x9cc
    unsigned long                                            CurrentFrame;                    // +0x9d0
    CTextureManager                                          TextureManager;                  // +0x9d4
    CPixelFormat                                             OpaquePixelFormat;               // +0xfa8
    CPixelFormat                                             AlphaPixelFormat;                // +0xfac
    CPixelFormat                                             CompressedOpaquePixelFormat;     // +0xfb0
    CPixelFormat                                             CompressedAlphaPixelFormat;      // +0xfb4
    CVertexShader                                            ScreenCopyVertexShader;          // +0xfb8
    std::pair<long, long>                                    OpaqueTextureHandles[4][4];      // +0xfc0
    std::pair<long, long>                                    AlphaTextureHandles[4][4];       // +0x1040
    float                                                    ReuseTolerance;                  // +0x10c0
    float                                                    ToleranceFalloffPercent;         // +0x10c4
    long                                                     MaxCompositeTextureSize;         // +0x10c8
    unsigned long                                            MinRenderTargetSize;             // +0x10cc
    unsigned long                                            MinMipMapLevels;                 // +0x10d0
    bool                                                     Enable;                          // +0x10d4
    bool                                                     RequiresCopy;                    // +0x10d5
    unsigned long                                            CacheSizeBytes;                  // +0x10d8
#elif defined(FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_BANKS)
    // Chunk 767 (row 0x00B2AD80): Ego_r.pdb ?ChangeSceneForTextureBank@CEngineSurfaceCompositionManager@@QAEXXZ (`add ecx,398h ; jmp
    // 0x009FD0C0` = CompletedBankFile.ChangeScene()); the three CGraphicDataBank members (Ego_r names and order: CachedSurfaces list,
    // IntermediateBankFile, CompletedBankFile, CustomTextureBankFile), retail 0x30C bytes each from +0x8C.
    void ChangeSceneForTextureBank();                                    // 0x00B2AD80
private:
    unsigned char _pad_0x88[0x4];               // +0x88 CachedSurfaces (list<CCountedPointer<CCompositeSurfaceCache> >, not modelled)
    CGraphicDataBank IntermediateBankFile;      // +0x8c
    CGraphicDataBank CompletedBankFile;         // +0x398
    CGraphicDataBank CustomTextureBankFile;     // +0x6a4
    unsigned char _pad_0x9b0[0x72c];            // +0x9b0 members not modelled yet (Ego_r.pdb names; retail STLport sizes)
#else
private:
    unsigned char _pad_0x88[0x1054];            // +0x88 members not modelled yet (Ego_r.pdb names; retail STLport sizes)
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEngineSurfaceCompositionManager) == 0x10dc);
#else // generated flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineSurfaceCompositionManager = 0x129c.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineSurfaceCompositionManager = 0x10dc.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CWideStringData;

#pragma pack(push, 1)
class CEngineSurfaceCompositionManager {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    unsigned char    _pad_0x0c[0x4];                         // +0x0c
    unsigned char    _pad_0x10[0x4];                         // +0x10
    unsigned char    _pad_0x14[0x4];                         // +0x14
    unsigned char    _pad_0x18[0x4];                         // +0x18
    unsigned char    _pad_0x1c[0x4];                         // +0x1c
    unsigned char    _pad_0x20[0x4];                         // +0x20
    unsigned char    _pad_0x24[0x4];                         // +0x24
    unsigned char    _pad_0x28[0x4];                         // +0x28
    unsigned char    _pad_0x2c[0x4];                         // +0x2c
    unsigned char    _pad_0x30[0x4];                         // +0x30
    unsigned char    _pad_0x34[0x4];                         // +0x34
    unsigned char    _pad_0x38[0x4];                         // +0x38
    unsigned char    _pad_0x3c[0x4];                         // +0x3c
    unsigned char    _pad_0x40[0x4];                         // +0x40
    unsigned char    _pad_0x44[0x4];                         // +0x44
    unsigned char    _pad_0x48[0x4];                         // +0x48
    unsigned char    _pad_0x4c[0x4];                         // +0x4c
    unsigned char    _pad_0x50[0x4];                         // +0x50
    unsigned char    _pad_0x54[0x4];                         // +0x54
    unsigned char    _pad_0x58[0x4];                         // +0x58
    unsigned char    _pad_0x5c[0x4];                         // +0x5c
    unsigned char    _pad_0x60[0x4];                         // +0x60
    unsigned char    _pad_0x64[0x4];                         // +0x64
    unsigned char    _pad_0x68[0x4];                         // +0x68
    unsigned char    _pad_0x6c[0x4];                         // +0x6c
    unsigned char    _pad_0x70[0x4];                         // +0x70
    unsigned char    _pad_0x74[0x4];                         // +0x74
    unsigned char    _pad_0x78[0x4];                         // +0x78
    unsigned char    _pad_0x7c[0x4];                         // +0x7c
    unsigned char    _pad_0x80[0x4];                         // +0x80
    unsigned char    _pad_0x84[0x4];                         // +0x84
    unsigned char    CachedSurfaces[0x4];                    // +0x88 list<CCountedPointer<CCompositeSurfaceCache>,std::allocator<CCountedPointer<CCompositeSurfaceCache>_>_>
    unsigned char    IntermediateBankFile[0x30c];            // +0x8c CGraphicDataBank
    unsigned char    CompletedBankFile[0x30c];               // +0x398 CGraphicDataBank
    unsigned char    CustomTextureBankFile[0x30c];           // +0x6a4 CGraphicDataBank
    CWideStringData* IntermediateDiskCacheFileName_PStringData; // +0x9b0 CWideString::PStringData
    CWideStringData* CompletedDiskCacheFileName_PStringData; // +0x9b4 CWideString::PStringData
    CWideStringData* CustomTextureBankFileName_PStringData;  // +0x9b8 CWideString::PStringData
    unsigned char    GeneratedMorphs[0xc];                   // +0x9bc map<std::pair<long,long>,CEngineTextureMorph,std::less<std::pair<long,long>_>,std::allocator<std::pair<std::pair<long,long>_const_,CEngineTextureMorph>_>_>
    float            DiskCacheTolerance;                     // +0x9c8
    float            MaxPreloadTime;                         // +0x9cc
    unsigned long    CurrentFrame;                           // +0x9d0
    unsigned char    TextureManager[0x5d4];                  // +0x9d4 CTextureManager
    long             OpaquePixelFormat_FormatIndex;          // +0xfa8 CPixelFormat::FormatIndex
    long             AlphaPixelFormat_FormatIndex;           // +0xfac CPixelFormat::FormatIndex
    long             CompressedOpaquePixelFormat_FormatIndex; // +0xfb0 CPixelFormat::FormatIndex
    long             CompressedAlphaPixelFormat_FormatIndex; // +0xfb4 CPixelFormat::FormatIndex
    unsigned char    ScreenCopyVertexShader[0x8];            // +0xfb8 CVertexShader
    unsigned char    OpaqueTextureHandles[0x80];             // +0xfc0 pair<long,long>[4][4]
    unsigned char    AlphaTextureHandles[0x80];              // +0x1040 pair<long,long>[4][4]
    float            ReuseTolerance;                         // +0x10c0
    float            ToleranceFalloffPercent;                // +0x10c4
    long             MaxCompositeTextureSize;                // +0x10c8
    unsigned long    MinRenderTargetSize;                    // +0x10cc
    unsigned long    MinMipMapLevels;                        // +0x10d0
    bool             Enable;                                 // +0x10d4
    bool             RequiresCopy;                           // +0x10d5
    unsigned char    _pad_0x10d6[0x2];                       // +0x10d6
    unsigned long    CacheSizeBytes;                         // +0x10d8
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineSurfaceCompositionManager) == 0x10dc);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, CachedSurfaces) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, IntermediateBankFile) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, CompletedBankFile) == 0x398);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, CustomTextureBankFile) == 0x6a4);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, IntermediateDiskCacheFileName_PStringData) == 0x9b0);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, CompletedDiskCacheFileName_PStringData) == 0x9b4);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, CustomTextureBankFileName_PStringData) == 0x9b8);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, GeneratedMorphs) == 0x9bc);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, DiskCacheTolerance) == 0x9c8);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, MaxPreloadTime) == 0x9cc);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, CurrentFrame) == 0x9d0);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, TextureManager) == 0x9d4);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, OpaquePixelFormat_FormatIndex) == 0xfa8);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, AlphaPixelFormat_FormatIndex) == 0xfac);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, CompressedOpaquePixelFormat_FormatIndex) == 0xfb0);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, CompressedAlphaPixelFormat_FormatIndex) == 0xfb4);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, ScreenCopyVertexShader) == 0xfb8);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, OpaqueTextureHandles) == 0xfc0);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, AlphaTextureHandles) == 0x1040);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, ReuseTolerance) == 0x10c0);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, ToleranceFalloffPercent) == 0x10c4);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, MaxCompositeTextureSize) == 0x10c8);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, MinRenderTargetSize) == 0x10cc);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, MinMipMapLevels) == 0x10d0);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, Enable) == 0x10d4);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, RequiresCopy) == 0x10d5);
FABLE_STATIC_ASSERT(offsetof(CEngineSurfaceCompositionManager, CacheSizeBytes) == 0x10d8);

#endif // FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
#endif // FABLE_ENGINE_CENGINESURFACECOMPOSITIONMANAGER_H
