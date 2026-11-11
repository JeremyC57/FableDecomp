// CEngineSurfaceCompositionManager::CEngineSurfaceCompositionManager() -- construct the bases and members, set the composition defaults, pick the texture formats, fetch the screen-copy vertex shader, build the three disk-cache file names, initialise the three graphic banks with the compressed formats and allocate the 4x4 opaque / alpha scratch texture handle tables (32..256 texels)
// Leaf: ??0CEngineSurfaceCompositionManager@@QAE@XZ
// Inline-budget: FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE (engine/GFGetNextPowerOfTwo.h opt-in, owner 2026-11-01 rule: GFGetTopBit is a
//   FableWin.pdb-only Lionhead helper whose retail out-of-line copy 0x0099FDB0 has no caller). c2 inline trace (tools/inl.sh, hook c2.dll
//   0x1078D1AA): with plain `inline` the two variable-argument GFGetTopBit(width) / GFGetTopBit(height) sites score 81 (c1 120, f 25)
//   against the default threshold 74, so the compile calls the out-of-line copy where retail expands both (0x00B2CF85..0x00B2D072);
//   `while` loops only move the score to 80. /d2inlT82 also matched (pass 1) and was dropped for the same forced spelling as row
//   0x00B2D330 of this TU; the constant GFGetTopBit(32) sites fold either way and every other decision is unchanged.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2CB40, 1637 bytes, thiscall, plain `ret` (no EH frame: the TU has no /EHsc), aligned frame
//   (`and esp,-8`). Sole caller CEngine::Initialise 0x00B26360 (`push 10DCh ; call operator new ; ... ; call 0x00B2CB40`).
// Identity (proven): Ego_r.pdb / FableWin.pdb ??0CEngineSurfaceCompositionManager@@QAE@XZ (Ego_r 0x009CE6D0, right after the dtor
//   0x009CE560 = retail 0x00B2D1B0, landed); the body stores both class vtables 0x012A1238 (CResourceBank part) / 0x012A1200
//   (CEngineComponent part, +0x7C) after the two base constructors. The catalog label is right.
// Layout: engine/CEngineSurfaceCompositionManager.h FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS (overlay hdr/): Ego_r names
//   at the retail offsets (the landed dtor's model).
// Callees: CResourceBank() 0x009FC5F0, CEngineComponent() 0x00B59710, malloc 0x00BFEA0E (STLport list head node 0x10 bytes, morph map
//   header node 0x2C bytes), CGraphicDataBank() 0x009FEA20 x3, CWideString() 0x0099AED0 x3, CTextureManager() 0x00A6A360,
//   CVertexShader() 0x00A62FB0, CDisplayManager::GetBestOpaqueTextureFormat 0x009BE610 / GetBestAlphaTextureFormat 0x009BE6C0 /
//   GetBestCompressedOpaqueTextureFormat 0x009BE830 / GetBestCompressedAlphaTextureFormat 0x009BE870 /
//   GetBestCompressedBooleanAlphaTextureFormat 0x009BE8B0 / GetBestSignedTextureFormat 0x009BE590 (all `CPixelFormat&`, Ego_r.pdb),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CShaderResource::GetVertexShader(const CCharString&) const
//   0x00A5D5F0, operator+(const CWideString&, const unsigned short*) 0x0099BF30, CWideString::operator= 0x0099B7D0, ~CWideString
//   0x0099B510, CGraphicDataBank::Initialise 0x009FD4E0, CBankFile::SetIsStaticMap 0x009D5230 (empty `ret 4`; established
//   descriptive name), CGPUScratchBufferTextureManager::AllocteMemoryPage 0x00F37660 / AllocateTexture 0x00F37440.
// Inline (expanded): the STLport list / map constructors, CPixelFormat() x4 (-1), the pair<long,long> array constructors (two
//   16-iteration loops), CEngineResourceManager::GetVShaderResource (bank 13 at +0x2A28) / PeekWriteableEngineCachePath (+0x2AD0) /
//   GetGPUScratchBufferTextureManager (+0x3190), CVertexShader::operator= and the temporary's ~CVertexShader, CGraphicDataBankInit()
//   (out of line at 0x00415B80 elsewhere), CGraphicDataBank::GetPixelFormat (PixelFormats[1] +0x28C, [0] +0x288), GFGetTopBit x4,
//   C2DExtentsI(long, long) x4.
// Globals: the display manager pointer 0x01436E1C (established spelling g_pDisplayManager_01436e1c), NEngineInternal::GPResourceManager
//   0x01436E98.
// Shape: (1) the shader resource is named before the call so its pointer is read before the implicit "VSHADER_SCREEN_FX_COPY"
//   CCharString temporary is built (retail 0x00B2CD06 / 0x00B2CD1D); (2) CompressedAlphaPixelFormat is copied before the opaque one
//   (0x00B2CF43); (3) the handle-table indices are GFGetTopBit(size) - GFGetTopBit(32): the constant site folds after c2's address
//   CSE, so each store keeps retail's `lea r,[y+x*4+1DFh] / [..-19h] / [..+1EFh]` forms with x / y live in ebp / edi and the scratch
//   manager in a frame slot (a literal `- 5` lets c2 share x*4+y in a register instead); (4) one page variable per table
//   (opaque_page / alpha_page): with a single reassigned `page` its IL weight 4 puts it ahead of the CCharString temporary in c2's
//   frame packer and the slots at [esp+14h] / [esp+1Ch] swap partners; (5) `<<= 1` loop steps (`shl`, where `*= 2` gives `add`).
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_CACHEPATHS
#define FABLE_RETAIL_CDISPLAYMANAGER_ALLTEXTUREFORMATS
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE
#include "engine/CEngineSurfaceCompositionManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "engine/CShaderResource.h"
#include "engine/CGraphicDataBankInit.h"
#include "engine/C2DExtentsI.h"
#include "engine/GFGetNextPowerOfTwo.h"

extern CDisplayManager* g_pDisplayManager_01436e1c;                      // 0x01436E1C (established spelling)
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

CEngineSurfaceCompositionManager::CEngineSurfaceCompositionManager()
{
    ReuseTolerance = 10.0f;
    ToleranceFalloffPercent = 10.0f;
    MinRenderTargetSize = 64;
    MinMipMapLevels = 5;
    MaxCompositeTextureSize = 256;
    if (!g_pDisplayManager_01436e1c->GetBestOpaqueTextureFormat(32, OpaquePixelFormat, false))
        g_pDisplayManager_01436e1c->GetBestOpaqueTextureFormat(16, OpaquePixelFormat, false);
    g_pDisplayManager_01436e1c->GetBestAlphaTextureFormat(32, AlphaPixelFormat);
    const CShaderResource* shader_resource = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)13);
    ScreenCopyVertexShader = shader_resource->GetVertexShader("VSHADER_SCREEN_FX_COPY");
    CompletedDiskCacheFileName = NEngineInternal::GPResourceManager->PeekWriteableEngineCachePath() + L"comfront.dat";
    IntermediateDiskCacheFileName = NEngineInternal::GPResourceManager->PeekWriteableEngineCachePath() + L"comback.dat";
    CustomTextureBankFileName = NEngineInternal::GPResourceManager->PeekWriteableEngineCachePath() + L"comuser.dat";
    DiskCacheTolerance = 0.01f;
    MaxPreloadTime = 5.0f;
    CurrentFrame = 0;
    CGraphicDataBankInit init;
    g_pDisplayManager_01436e1c->GetBestCompressedOpaqueTextureFormat(32, init.NonAlphaFormat);
    g_pDisplayManager_01436e1c->GetBestCompressedAlphaTextureFormat(32, init.AlphaFormat);
    g_pDisplayManager_01436e1c->GetBestCompressedBooleanAlphaTextureFormat(16, init.BooleanAlphaFormat);
    g_pDisplayManager_01436e1c->GetBestAlphaTextureFormat(32, init.UncompressedAlphaFormat);
    g_pDisplayManager_01436e1c->GetBestOpaqueTextureFormat(32, init.UncompressedNonAlphaFormat, false);
    g_pDisplayManager_01436e1c->GetBestSignedTextureFormat(16, init.SignedFormat);
    init.MaxGraphicWidth = MaxCompositeTextureSize;
    init.MaxGraphicHeight = MaxCompositeTextureSize;
    init.GenerateMipmaps = false;
    init.AllowDither = false;
    IntermediateBankFile.Initialise(init);
    IntermediateBankFile.SetIsStaticMap(true);
    CompletedBankFile.Initialise(init);
    CompletedBankFile.SetIsStaticMap(true);
    CustomTextureBankFile.Initialise(init);
    CustomTextureBankFile.SetIsStaticMap(true);
    CompressedAlphaPixelFormat = CompletedBankFile.GetPixelFormat(CGraphicDataBank::TEXTURE_TYPE_ALPHA);
    CompressedOpaquePixelFormat = CompletedBankFile.GetPixelFormat(CGraphicDataBank::TEXTURE_TYPE_OPAQUE);
    CGPUScratchBufferTextureManager& scratch = NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    for (long width = 32; width <= 256; width <<= 1) {
        for (long height = 32; height <= 256; height <<= 1) {
            unsigned long x = GFGetTopBit(width) - GFGetTopBit(32);
            unsigned long y = GFGetTopBit(height) - GFGetTopBit(32);
            long opaque_page = scratch.AllocteMemoryPage(0, true);
            OpaqueTextureHandles[x][y].first = scratch.AllocateTexture(opaque_page, C2DExtentsI(width, height), 1, OpaquePixelFormat, 1, false);
            OpaqueTextureHandles[x][y].second = scratch.AllocateTexture(opaque_page, C2DExtentsI(width, height), -1, CompressedOpaquePixelFormat, 0, true);
            long alpha_page = scratch.AllocteMemoryPage(0, true);
            AlphaTextureHandles[x][y].first = scratch.AllocateTexture(alpha_page, C2DExtentsI(width, height), 1, AlphaPixelFormat, 1, false);
            AlphaTextureHandles[x][y].second = scratch.AllocateTexture(alpha_page, C2DExtentsI(width, height), -1, CompressedAlphaPixelFormat, 0, true);
        }
    }
    Enable = true;
}
