#ifndef FABLE_ENGINE_CGRAPHICDATABANKINIT_H
#define FABLE_ENGINE_CGRAPHICDATABANKINIT_H
// New header (chunk 2018, row 0x00B2CB40 CEngineSurfaceCompositionManager::CEngineSurfaceCompositionManager). Ego_r.pdb
// CGraphicDataBankInit (0x2C bytes, public members in this order; struct_layouts_egor.tsv) and ??0CGraphicDataBankInit@@QAE@XZ
// (Ego_r.pdb / FableWin.pdb). The constructor is inline: out of line at 0x00415B80 (seven formats and both maxima -1, GenerateMipmaps
// false, AllowDither true, AllowCompressedTextures false, AllowDXT1ForBooleanAlpha true), expanded in place at 0x00B2CE25..0x00B2CE86.
// The landed CMainGameComponent::InitGraphics 0x00416C8A / CNewFrontendGameComponent::InitialiseGraphics 0x0042DDB3 model it locally.
#ifndef FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT
#endif
#include "rebuild_abi.h"
#include "engine/CPixelFormat.h"
class CGraphicDataBankInit {
public:
#ifdef FABLE_RETAIL_CGRAPHICDATABANKINIT_CTOR_DECL_ONLY
    // NF223 per-TU opt-in (2026-11-22, rows 0x0041E3F6 NUISystem::CManager::CManager / 0x005BFF07 CTCInventoryBase::CTCInventoryBase):
    // those size TUs call the out-of-line 0x00415B80, and the inline body below would emit a pick-any copy that differs from it (the
    // constant bytes stored directly instead of through dl / cl). Declaration only; the retail body is the #else one.
    CGraphicDataBankInit();
#else
    CGraphicDataBankInit()
        : GenerateMipmaps(false), MaxGraphicWidth((unsigned long)-1), MaxGraphicHeight((unsigned long)-1), AllowDither(true),
          AllowCompressedTextures(false), AllowDXT1ForBooleanAlpha(true) { }
#endif
    CPixelFormat  NonAlphaFormat;                            // +0x00
    CPixelFormat  AlphaFormat;                               // +0x04
    CPixelFormat  InterpolatedAlphaFormat;                   // +0x08
    CPixelFormat  BooleanAlphaFormat;                        // +0x0C
    CPixelFormat  UncompressedNonAlphaFormat;                // +0x10
    CPixelFormat  UncompressedAlphaFormat;                   // +0x14
    CPixelFormat  SignedFormat;                              // +0x18
    bool          GenerateMipmaps;                           // +0x1C
    unsigned long MaxGraphicWidth;                           // +0x20
    unsigned long MaxGraphicHeight;                          // +0x24
    bool          AllowDither;                               // +0x28
    bool          AllowCompressedTextures;                   // +0x29
    bool          AllowDXT1ForBooleanAlpha;                  // +0x2A
};
FABLE_STATIC_ASSERT_AT(32, sizeof(CGraphicDataBankInit) == 0x2c);
#endif // FABLE_ENGINE_CGRAPHICDATABANKINIT_H
