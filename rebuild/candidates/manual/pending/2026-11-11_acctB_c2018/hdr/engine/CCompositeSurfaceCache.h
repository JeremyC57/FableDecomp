#ifndef FABLE_ENGINE_CCOMPOSITESURFACECACHE_H
#define FABLE_ENGINE_CCOMPOSITESURFACECACHE_H
// New header (chunk 2018, row 0x00B2D330 CEngineSurfaceCompositionManager::UpdateDiskCacheTexture). RTTI .?AVCCompositeSurfaceCache@@ :
// CManagedTextureOwner : CResource : CIVCountedPointeeBase (rtti_bases.tsv); Ego_r.pdb layout (0x68 retail: the STLport vector makes
// CEngineTextureMorph 0x14): Owner +0x1C, CurrentMorph +0x20, CachedMorph +0x34, MipmapLevel +0x48, GraphicFrame +0x4C, ManagedTexture
// +0x50, BankFileIndex +0x58, LastUsedFrame +0x5C, TimeStamp +0x60 -- the offsets the landed ctor 0x00B2C400, ~CCompositeSurfaceCache
// 0x00B2B910 and ~CEngineSurfaceCompositionManager 0x00B2D1B0 models use. Functions: Ego_r.pdb / FableWin.pdb
// ?GetCurrentMorph@CCompositeSurfaceCache@@QAEAAVCEngineTextureMorph@@XZ (+ the const twin), inline; the composition manager reads and
// writes BankFileIndex / MipmapLevel / TimeStamp in place (0x00B2D412, 0x00B2D944, 0x00B2D94C), so it is a friend here. ManagedTexture
// is not modelled. Needs FABLE_RETAIL_DEF_CLASS_BASE.
#include "rebuild_abi.h"
#include "engine/CManagedTextureOwner.h"
#include "engine/CEngineTextureMorph.h"
#include "engine/CGraphicFrame.h"
class CEngineSurfaceCompositionManager;
class CCompositeSurfaceCache : public CManagedTextureOwner {
    friend class CEngineSurfaceCompositionManager;
public:
    CCompositeSurfaceCache(CEngineSurfaceCompositionManager& owner);    // 0x00B2C400
    virtual ~CCompositeSurfaceCache();                                   // 0x00B2B910
    CEngineTextureMorph& GetCurrentMorph() { return CurrentMorph; }
    const CEngineTextureMorph& GetCurrentMorph() const { return CurrentMorph; }
private:
    CEngineSurfaceCompositionManager* Owner;                 // +0x1C
    CEngineTextureMorph               CurrentMorph;          // +0x20
    CEngineTextureMorph               CachedMorph;           // +0x34
    unsigned long                     MipmapLevel;           // +0x48
    CGraphicFrame                     GraphicFrame;          // +0x4C
    unsigned char                     ManagedTexture[0x8];   // +0x50 CManagedTexture (not modelled)
    unsigned long                     BankFileIndex;         // +0x58
    unsigned long                     LastUsedFrame;         // +0x5C
    double                            TimeStamp;             // +0x60
};
FABLE_STATIC_ASSERT(sizeof(CCompositeSurfaceCache) == 0x68);
#endif // FABLE_ENGINE_CCOMPOSITESURFACECACHE_H
