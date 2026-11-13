// CEngineSurfaceCompositionManager::UpdateDiskCacheTexture(CCompositeSurfaceCache& surface_cache, unsigned long mipmap_level, bool is_bump) -- refresh the intermediate disk-cache bank entry "<texture>-<morph>" for the cache's current morph: look the entry up (adding it with a default graphic-bank state block when missing), return false when a nearly identical morph was already generated; otherwise fit the original texture's frame below MaxCompositeTextureSize, pick the alpha / opaque scratch texture pair for that size, fetch every blend layer's frame at the mip level (false when one is not loaded that far), render the morph into the compressed scratch texture, remember the morph, level and time, write the texture into the bank and return true
// Leaf: ?UpdateDiskCacheTexture@CEngineSurfaceCompositionManager@@AAE_NAAVCCompositeSurfaceCache@@K_N@Z
// Inline-budget: four FABLE_FORCEINLINE Lionhead header helpers (owner 2026-11-01 rule: FableWin.pdb-only names whose retail
//   out-of-line copies have no caller), measured with the c2 inline trace (tools/inl.sh, default threshold 74): GFGetTopBit
//   (FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE; variable sites c1 120, score 125 at f 0.75), CGraphicInfoFrame::GetFrame (c1 188, score 142 at
//   f 3.75; retail expands it at 0x00B2D6DB..0x00B2D8A9), CGraphicFrameData::Release (c1 75, score 85 at f 0.5) and CGraphicFrame::operator=
//   (c1 101, score 115..127 at f 0.35..0.5). With all four forced the remaining decisions match retail (??_GCGraphicFrameData score 60, r=1).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2D330, 1723 bytes, thiscall, `ret 0Ch` (three stack words), no EH frame (the TU has no /EHsc),
//   0x48-byte frame.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?UpdateDiskCacheTexture@CEngineSurfaceCompositionManager@@AAE_NAAVCCompositeSurfaceCache@@K_N@Z
//   (private, bool(CCompositeSurfaceCache&, unsigned long, bool)); the body uses the three parameters as typed (BankFileIndex,
//   MipmapLevel, TimeStamp and the current morph of the cache; the level passed by reference to GetDimensionsAndAdjustMipmapLevel; the
//   bool masking the alpha choice and forwarded to RenderToTexture) and calls RenderToTexture 0x00B2B180 (0x00B2D911), the class's
//   only seven-parameter member. The catalog label is right.
// Pair-landing: this TU also defines RenderToTexture (row 0x00B2B180 of this chunk, same body as src/00b2b180.cpp) and a noinline
//   GetDimensionsAndAdjustMipmapLevel (landed row 0x00B2AEF0); retail compiled both earlier in the TU and c2 needs their bodies to
//   keep `dimensions` / `mipmap_level` unescaped (earlier probes without them missed retail's 0x48 frame and register assignment).
// Layout: engine/CEngineSurfaceCompositionManager.h MEMBERS opt-in (IntermediateBankFile, GeneratedMorphs, DiskCacheTolerance,
//   MaxCompositeTextureSize, OpaqueTextureHandles / AlphaTextureHandles); engine/CCompositeSurfaceCache.h, engine/CEngineTextureMorph.h,
//   engine/CGraphicInfo.h / CGraphicInfoFrame.h / CGraphicDataBank.h SURFACECOMPOSITION branches (overlay hdr/).
// Callees: CGraphicDataBank::FrameEnd 0x009FD660, CRenderManagerCore::WaitWhileGPUIsBusy 0x00A0AF80 x2, GFIntToCharString 0x0099F830 x2,
//   operator+(const CCharString&, const char*) 0x0099F600 / (const CCharString&, const CCharString&) 0x0099F570, ~CCharString 0x0099EAE0,
//   CBankFile::FindIndexBySymbol 0x009CCDC0 x2 / AddEntry 0x009CD620, the morph map's _M_find 0x00B2E630 / operator[] 0x00B2F5E0,
//   IsNearlyIdentical 0x00B2B050, operator new 0x00BFEA1A / delete 0x00BFE9BC, CCountedPointer<CGraphicBankStateBlock>(T*) 0x00B2E340,
//   CCharString(const char*, long) 0x0099EBF0, GetDimensionsAndAdjustMipmapLevel 0x00B2AEF0, CGPUScratchBufferTextureManager::
//   StartTextureUse 0x00F36EE0 / EndTextureUse 0x00F36F00, vector<CGraphicFrame>(size_t) 0x00B2EC80, GetGraphicInfo 0x00B2B790,
//   CGraphicInfoFrame::CFrameData::BeginLoading 0x009FDE80 / CFrameData(const CGraphicInfoFrame*) 0x009FE380,
//   InternalGetFrameForcedLoading 0x009FF6B0, CGraphicFrameData::Uninitialise 0x009FCBA0, RenderToTexture 0x00B2B180,
//   vector<CCompositeBlendDesc>::operator= 0x006CADE0, GFGetTime 0x009E1BC0, WriteTextureToBank 0x00B2BD80,
//   std::__destroy_aux<CGraphicFrame*> 0x00B2EFB0 (fastcall), free 0x00BFEA14.
// Inline (expanded): GetCurrentMorph, GetGraphicBank, PeekGraphic, PeekInfoFrame, the map find / end / iterator compare, the
//   CGraphicBankStateBlock constructor chain and its GetDefault virtual call (slot 1), CCountedPointer copy / release, IsAlpha,
//   GetGPUScratchBufferTextureManager, GFGetTopBit x4 (the constant GFGetTopBit(32) sites fold), GetFrame / operator= / UnInitialise /
//   Release (with ??_GCGraphicFrameData), GetCurrentLoadedMipmapLevel, the CArray<CGraphicFrame> destructor.
// Shape: (1) texture_index / morph_id are named before the key pair and the name string: retail loads both once (0x00B2D397), stores
//   the key (0x00B2D3A0) and passes them to the two GFIntToCharString calls from registers (edx, then `mov edx,esi` at 0x00B2D3B3);
//   (2) the handle pair is copied out of the alpha / opaque table by one ternary on the pair address (0x00B2D646..0x00B2D672) and its
//   second member named for StartTextureUse / EndTextureUse; (3) the layer loop goes through `morph` (the reference held from the top,
//   kept at [esp+18h]) while the RenderToTexture call and the map store re-read surface_cache.GetCurrentMorph().
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_CGRAPHICBANKSTATEBLOCK
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGERCORE_WAITWHILEGPUISBUSY
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_AUGMENTATION_PARTICLES
#define FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CTEXTURE_GENERATEMIPMAPS
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_COPYFROMSURFACE
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_DTOR
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR_B
#include "engine/Direct3D9Types.h"
#include "engine/CEngineSurfaceCompositionManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicInfoFrame.h"
#include "engine/CCompositeSurfaceCache.h"
#include "engine/CGraphicBankStateBlock.h"
#include "engine/C2DExtentsI.h"
#include "engine/CArray.h"
#include "engine/CRenderManager.h"
#include "engine/GFGetNextPowerOfTwo.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/C3DVector.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
#include "engine/CGraphicFrame.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CSurfaceCompositor.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                // 0x01436E18
CCharString FABLE_FASTCALL GFIntToCharString(long value);                            // 0x0099F830
double FABLE_FASTCALL GFGetTime();                                                   // 0x009E1BC0
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14

// Same-TU body (retail compiles it earlier in this TU, called out of line at 0x00B2D51F; the landed row 0x00B2AEF0): visible so that c2
// knows it keeps neither the dimensions' nor the level's address.
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CEngineSurfaceCompositionManager::GetDimensionsAndAdjustMipmapLevel(C2DExtentsI& dimensions, const CGraphicInfoFrame* frame, unsigned long& mipmap_level)
{
    dimensions.X = frame->GetWidth() >> mipmap_level;
    dimensions.Y = frame->GetHeight() >> mipmap_level;
    while (dimensions.X > MaxCompositeTextureSize || dimensions.Y > MaxCompositeTextureSize) {
        dimensions.X /= 2;
        dimensions.Y /= 2;
        ++mipmap_level;
    }
}

void CEngineSurfaceCompositionManager::RenderToTexture(CTexture& texture, const C2DExtentsI& extents, bool alpha, bool unused_flag,
                                                       const CEngineTextureMorph& morph, const CArray<CGraphicFrame>& frames,
                                                       long scratch_texture)
{
    CGPUScratchBufferTextureManager& scratch = NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    CTexture* render_texture = scratch.StartTextureUse(scratch_texture);
    CSurface render_surface = render_texture->GetSurfaceLevel(0);
    CScopedShadersEnable shaders_enable(true, false);
    CScopedVertexShaderConstantLayout constant_layout(3);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(ScreenCopyVertexShader, false);
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    NEngineInternal::GPRenderManager->SetSpecularEnable(false);
    C2DBoxI box(0, 0, extents.X, extents.Y);
    unsigned long size = extents.X < extents.Y ? extents.X : extents.Y;
    long level = 0;
    while (size >= MinRenderTargetSize) {
        CSurface level_surface = texture.GetSurfaceLevel(level);
        C2DBoxF quad;
        quad.TLX = -1.0f;
        quad.TLY = 1.0f - (float)(box.BRY * 2) / extents.Y;
        quad.BRX = (float)(box.BRX * 2) / extents.X - 1.0f;
        quad.BRY = 1.0f;
        CSurfaceCompositor compositor(render_surface);
        compositor.StartComposition(alpha);
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        unsigned long count = morph.Blends.size();
        for (unsigned long i = 0; i < count; ++i) {
            const CCompositeBlendDesc& blend = morph.Blends[i];
            const CTexture* layer_texture = frames[i].GetTexture();
            float u = 0.25f / box.BRX;
            float v = 0.25f / box.BRY;
            CSurfaceCompositor::CVertexType vertices[4];
            vertices[0].SetPosition(C3DVector(quad.TLX, quad.BRY, 0.5f));
            vertices[0].SetU1V1(u, v);
            vertices[0].DiffuseColour = blend.DiffuseColour.GetD3DCOLOR();
            vertices[1].SetPosition(C3DVector(quad.BRX, quad.BRY, 0.5f));
            vertices[1].SetU1V1(u + 1.0f, v);
            vertices[1].DiffuseColour = blend.DiffuseColour.GetD3DCOLOR();
            vertices[2].SetPosition(C3DVector(quad.BRX, quad.TLY, 0.5f));
            vertices[2].SetU1V1(u + 1.0f, v + 1.0f);
            vertices[2].DiffuseColour = blend.DiffuseColour.GetD3DCOLOR();
            vertices[3].SetPosition(C3DVector(quad.TLX, quad.TLY, 0.5f));
            vertices[3].SetU1V1(u, v + 1.0f);
            vertices[3].DiffuseColour = blend.DiffuseColour.GetD3DCOLOR();
            compositor.CompositeQuad(*layer_texture, vertices, blend.BlendType);
        }
        compositor.EndComposition();
        level_surface.CopyFromSurface(&box, render_surface, &box, (unsigned long)-1);
        box.BRX /= 2;
        box.BRY /= 2;
        size >>= 1;
        ++level;
    }
    NEngineInternal::GPRenderManager->SetFogEnable(true);
    texture.GenerateMipmaps(5, level - 1);
    scratch.EndTextureUse(scratch_texture);
}

bool CEngineSurfaceCompositionManager::UpdateDiskCacheTexture(CCompositeSurfaceCache& surface_cache, unsigned long mipmap_level, bool is_bump)
{
    const CEngineTextureMorph& morph = surface_cache.GetCurrentMorph();
    const CGraphicInfoFrame* info_frame = NEngineInternal::GPResourceManager->GetGraphicBank((EGraphicBankNo)0)
        ->PeekGraphic(morph.OriginalTextureIndex)->PeekInfoFrame(0);
    NEngineInternal::GPResourceManager->GetGraphicBank((EGraphicBankNo)0)->FrameEnd();
    NEngineInternal::GPRenderManager->WaitWhileGPUIsBusy();
    long texture_index = morph.OriginalTextureIndex;
    long morph_id = morph.MorphID;
    std::pair<long, long> key(texture_index, morph_id);
    CCharString name = GFIntToCharString(texture_index) + "-" + GFIntToCharString(morph_id);
    unsigned long bank_index = IntermediateBankFile.FindIndexBySymbol(name);
    surface_cache.BankFileIndex = bank_index;
    std::map<std::pair<long, long>, CEngineTextureMorph>::iterator it = GeneratedMorphs.find(key);
    if (it != GeneratedMorphs.end() && IsNearlyIdentical(morph, it->second, DiskCacheTolerance))
        return false;
    if (!bank_index) {
        CCountedPointer<CGraphicBankStateBlock> state_block(new CGraphicBankStateBlock);
        state_block->GetDefault(*state_block);
        IntermediateBankFile.AddEntry(name, 0, "", state_block);
        surface_cache.BankFileIndex = IntermediateBankFile.FindIndexBySymbol(name);
    }
    C2DExtentsI dimensions;
    GetDimensionsAndAdjustMipmapLevel(dimensions, info_frame, mipmap_level);
    bool alpha = info_frame->IsAlpha() && !is_bump;
    CGPUScratchBufferTextureManager& scratch = NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    unsigned long x = GFGetTopBit(dimensions.X) - GFGetTopBit(32);
    unsigned long y = GFGetTopBit(dimensions.Y) - GFGetTopBit(32);
    std::pair<long, long> handles = alpha ? AlphaTextureHandles[x][y] : OpaqueTextureHandles[x][y];
    long compressed_handle = handles.second;
    CTexture* compressed_texture = scratch.StartTextureUse(compressed_handle);
    unsigned long layer_count = morph.Blends.size();
    CArray<CGraphicFrame> frames(layer_count);
    for (unsigned long i = 0; i < layer_count; ++i) {
        const CGraphicInfoFrame* layer_frame = GetGraphicInfo(morph.Blends[i].TextureIndex);
        if (!layer_frame->GetFrame(frames[i], mipmap_level, 0x19))
            return false;
        if ((unsigned long)layer_frame->GetCurrentLoadedMipmapLevel() > mipmap_level)
            return false;
    }
    RenderToTexture(*compressed_texture, dimensions, alpha, is_bump, surface_cache.GetCurrentMorph(), frames, handles.first);
    GeneratedMorphs[key] = surface_cache.GetCurrentMorph();
    surface_cache.MipmapLevel = mipmap_level;
    surface_cache.TimeStamp = GFGetTime();
    NEngineInternal::GPRenderManager->WaitWhileGPUIsBusy();
    WriteTextureToBank(*compressed_texture, IntermediateBankFile, surface_cache.BankFileIndex, alpha);
    scratch.EndTextureUse(compressed_handle);
    return true;
}