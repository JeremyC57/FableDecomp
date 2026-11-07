// CEngineSurfaceCompositionManager::UpdateDiskCache(const CArray<CEngineTextureMorph>&,bool,float,const std::set<long>*) -- composite every multi-texture morph into the intermediate disk-cache bank, within an optional time budget, then publish the bank
// Leaf: ?UpdateDiskCache@CEngineSurfaceCompositionManager@@QAEXABV?$CArray@VCEngineTextureMorph@@@@_NMPBV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2D9F0, 807 bytes (ends on a backward `jmp` after its `ret`: the loop's `continue` tail).
// Identity: Ego_r.pdb / FableWin.pdb ?UpdateDiskCache@CEngineSurfaceCompositionManager@@QAEXABV?$CArray@VCEngineTextureMorph@@@@_NMPBV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@@Z;
//   Codex pass 12 reached RELOCATION_MATCH in its own header context (pending/2026-11-02_codex_pass12/verified_pending/00b2d9f0); rebased here
//   onto our headers.
// Callees: GFGetTime 0x009E1BC0, CEngineResourceManager::GetGraphicBank (inline opt-in, GraphicBanks[0] at +0x954),
//   CGraphicDataBank::SetReduceSizeFrameDelay 0x009FD100 / SetUnloadFrameDelay 0x009FD0F0, CEngine::DeallocateShadowBuffer /
//   ReallocateShadowBuffer, CBankFile vtable slots 4 OpenFileReadWrite / 5 Close, BeginUpdateEntries 0x009CE8D0 / EndUpdateEntries 0x009CEA70,
//   GetEntryDataType 0x009CC210 (inline IsBump), vector<CCompositeBlendDesc>::operator= 0x006CADE0 (FABLE_RETAIL_STLP_VECTOR_ASSIGN, the
//   inline SetCurrentMorph), set<long> tree find 0x00B2EB20 (member template, owner decision D1), UpdateDiskCacheTexture 0x00B2D330,
//   ~CCompositeSurfaceCache 0x00B2B910, CopyIntermediateBank 0x00B2C8E0, PreloadCompletedBank 0x00B2C010.
// Globals: NEngineInternal::GPEngine 0x01436EA4, NEngineInternal::GPResourceManager 0x01436E98.
// Layout: this +0x8C IntermediateBankFile (CGraphicDataBank), +0x9B0 IntermediateDiskCacheFileName, +0x9BC GeneratedMorphs (map size
//   test), +0x10D5 RequiresCopy; CGraphicDataBank +0x1E0 GraphicList, +0x268 UnloadFrameDelay, +0x26C ReduceSizeFrameDelay.
//   Local models (header debt): engine/CBankFile.h is HOLD (flattened vptr) and CGraphicDataBank's bases are opaque in its header, so
//   the bank / graphic-info / surface-cache classes are modelled here as in the Codex source.
// Shape: the limit-set filter is a scoped `const long` key with a named const iterator compared `== end()` before `continue`
//   (key stored after the null test, find before end()); the CGraphicFrame ctor opt-in zeroes FrameData in member order inside the
//   expanded CCompositeSurfaceCache ctor.
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE
#define FABLE_RETAIL_STLP_SET_FIND
#define FABLE_RETAIL_STLP_SET
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CArray.h"
#include "engine/CBaseClass.h"
#include "engine/CResource.h"
#include "engine/CManagedTextureOwner.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CGraphicFrame.h"
#include "engine/CRGBColour.h"
#include "engine/CEngine.h"
#include "engine/CEngineResourceManager.h"
#include "fable_string.h"

double __fastcall GFGetTime();                                   // 0x009E1BC0 ?GFGetTime@@YINXZ

// ---- local models (engine/CBankFile.h is HOLD and flattens the vptr; CGraphicDataBank's bases are opaque in its header) ----
class CBankFile {
public:
    virtual void vslot0();
    virtual void vslot1();
    virtual void vslot2();
    virtual bool OpenFileReadOnly(const CWideString& file_name, unsigned long flags);                          // slot 3
    virtual void OpenFileReadWrite(const CWideString& file_name, unsigned long create, unsigned long flags);   // slot 4
    virtual void Close();                                                                                      // slot 5
    void BeginUpdateEntries();                                   // 0x009CE8D0 ?BeginUpdateEntries@CBankFile@@QAEXXZ
    void EndUpdateEntries();                                     // 0x009CEA70 ?EndUpdateEntries@CBankFile@@QAEXXZ
    unsigned long GetEntryDataType(unsigned long index) const;   // 0x009CC210 ?GetEntryDataType@CBankFile@@QBEKK@Z
    unsigned char _bankfile[0x10c];
};
class CBankFileAsync : public CBankFile {
public:
    unsigned char _async[0x164 - 0x110];
};
class CGraphicInfo;
class CGraphicDataBank : public CBankFileAsync {
public:
    unsigned long GetUnloadFrameDelay() const { return UnloadFrameDelay; }
    unsigned long GetReduceSizeFrameDelay() const { return ReduceSizeFrameDelay; }
    void SetUnloadFrameDelay(unsigned long delay);               // 0x009FD0F0
    void SetReduceSizeFrameDelay(unsigned long delay);           // 0x009FD100
    const CGraphicInfo* PeekGraphic(unsigned long index) const;
    unsigned char       _resourcebank[0x1e0 - 0x164];            // CResourceBank +0x164
    CArray<CGraphicInfo> GraphicList;                            // +0x1e0
    unsigned char       _pad_0x1ec[0x268 - 0x1ec];
    unsigned long       UnloadFrameDelay;                        // +0x268
    unsigned long       ReduceSizeFrameDelay;                    // +0x26c
    unsigned char       _pad_0x270[0x30c - 0x270];
};
class CGraphicInfo {
public:
    bool IsBump() const { return Bank->GetEntryDataType(Index) == 2; }
    unsigned char     Header[0x1c];                              // +0x00 CGraphicDataBank::CGraphicHeader
    long              TextureType;                               // +0x1c
    CGraphicDataBank* Bank;                                      // +0x20
    long              Index;                                     // +0x24
    void*             Frames;                                    // +0x28
};
inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}

enum ECompositeBlendType { };
class CCompositeBlendDesc {
public:
    unsigned long       TextureIndex;
    ECompositeBlendType BlendType;
    CRGBColour          DiffuseColour;
    CRGBColour          SpecularColour;
};
class CEngineTextureMorph {
public:
    CEngineTextureMorph() : OriginalTextureIndex(0), MorphID(0) { }
    long                             OriginalTextureIndex;
    long                             MorphID;
    std::vector<CCompositeBlendDesc> Blends;
};
class CManagedTextureData;
class CManagedTexture : public CBaseClass {
public:
    CManagedTexture() { }
    virtual ~CManagedTexture();
    bool Release();
    CIVCountedPointer<CManagedTextureData> Data;
};
class CEngineSurfaceCompositionManager;
class CCompositeSurfaceCache : public CManagedTextureOwner {
public:
    CCompositeSurfaceCache(CEngineSurfaceCompositionManager& owner) : Owner(owner), BankFileIndex(0), LastUsedFrame(0) { }
    virtual ~CCompositeSurfaceCache();                           // 0x00B2B910
    virtual void ReleaseTexture();
    void SetCurrentMorph(const CEngineTextureMorph& morph) { CurrentMorph = morph; }
    CEngineTextureMorph& GetCurrentMorph() { return CurrentMorph; }
    bool IsMultipleTextures() const { return CurrentMorph.Blends.size() > 1; }
private:
    CEngineSurfaceCompositionManager& Owner;
    CEngineTextureMorph CurrentMorph;
    CEngineTextureMorph CachedMorph;
    unsigned long       MipmapLevel;
    CGraphicFrame       GraphicFrame;
    CManagedTexture     ManagedTexture;
    unsigned long       BankFileIndex;
    unsigned long       LastUsedFrame;
    double              TimeStamp;
};

namespace NEngineInternal {
extern CEngine* GPEngine;                                        // 0x01436EA4
extern CEngineResourceManager* GPResourceManager;                // 0x01436E98
}

class CEngineSurfaceCompositionManager {
public:
    void UpdateDiskCache(const CArray<CEngineTextureMorph>& all_morphs, bool use_timeout, float timeout, const std::set<long>* limit_to_textures);
private:
    bool UpdateDiskCacheTexture(CCompositeSurfaceCache& surface_cache, unsigned long flags, bool is_bump);   // 0x00B2D330
    void CopyIntermediateBank();                                 // 0x00B2C8E0
    void PreloadCompletedBank();                                 // 0x00B2C010
    unsigned char    _bases[0x8c];
    CGraphicDataBank IntermediateBankFile;                       // +0x8c
    unsigned char    _pad_0x398[0x9b0 - 0x398];
    CWideString      IntermediateDiskCacheFileName;              // +0x9b0
    unsigned char    _pad_0x9b4[0x9bc - 0x9b4];
    std::map<long, long> GeneratedMorphs;                        // +0x9bc
    unsigned char    _pad_0x9c8[0x10d5 - 0x9c8];
    bool             RequiresCopy;                               // +0x10d5
    unsigned char    _pad_0x10d6[0x10dc - 0x10d6];
};

void CEngineSurfaceCompositionManager::UpdateDiskCache(const CArray<CEngineTextureMorph>& all_morphs, bool use_timeout, float timeout, const std::set<long>* limit_to_textures)
{
    double start_t = GFGetTime();
    long update_count = 0;
    bool finish_later = false;
    CGraphicDataBank& graphic_bank = *NEngineInternal::GPResourceManager->GetGraphicBank((EGraphicBankNo)0);
    unsigned long old_texture_resize_delay = graphic_bank.GetReduceSizeFrameDelay();
    unsigned long old_texture_unload_delay = graphic_bank.GetUnloadFrameDelay();
    graphic_bank.SetReduceSizeFrameDelay(1);
    graphic_bank.SetUnloadFrameDelay(1);
    NEngineInternal::GPEngine->DeallocateShadowBuffer();
    unsigned long open_flags = 0;
    if (GeneratedMorphs.size() == 0) open_flags = 1;
    IntermediateBankFile.OpenFileReadWrite(IntermediateDiskCacheFileName, open_flags, 0x1000);
    IntermediateBankFile.BeginUpdateEntries();
    for (long c_morph = 0; c_morph < all_morphs.size(); ++c_morph) {
        CCompositeSurfaceCache surface_cache(*this);
        surface_cache.SetCurrentMorph(all_morphs[c_morph]);
        unsigned long graphic_index = surface_cache.GetCurrentMorph().OriginalTextureIndex;
        {
            const long filter_key = static_cast<long>(graphic_index);
            if (limit_to_textures) {
                std::set<long>::iterator it = limit_to_textures->find(filter_key);
                if (it == limit_to_textures->end()) continue;
            }
        }
        if (surface_cache.IsMultipleTextures() && graphic_index) {
            const CGraphicInfo* graphic_info = NEngineInternal::GPResourceManager->GetGraphicBank((EGraphicBankNo)0)->PeekGraphic(graphic_index);
            if (UpdateDiskCacheTexture(surface_cache, 0, graphic_info->IsBump())) {
                RequiresCopy = true;
                ++update_count;
            }
            double elapsed = GFGetTime() - start_t;
            if (use_timeout && elapsed > timeout) { finish_later = true; break; }
        }
    }
    IntermediateBankFile.EndUpdateEntries();
    IntermediateBankFile.Close();
    NEngineInternal::GPEngine->ReallocateShadowBuffer();
    graphic_bank.SetReduceSizeFrameDelay(old_texture_resize_delay);
    graphic_bank.SetUnloadFrameDelay(old_texture_unload_delay);
    if (update_count) { double delta_t = GFGetTime() - start_t; }
    if (RequiresCopy && !finish_later) {
        CopyIntermediateBank();
        RequiresCopy = false;
        PreloadCompletedBank();
    }
}