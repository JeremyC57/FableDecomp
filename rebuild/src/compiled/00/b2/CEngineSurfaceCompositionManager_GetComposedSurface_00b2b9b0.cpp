// CEngineSurfaceCompositionManager::GetComposedSurface -- fetch a baked or reusable composed texture frame.
// Leaf: ?GetComposedSurface@CEngineSurfaceCompositionManager@@QAE_NAAVCGraphicFrame@@AAVCCompositeSurfaceInfo@@K_N@Z
// Flags: none
// Inline-budget: use the shared original graphic-frame and bank lookup bodies expanded in retail.
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINESURFACECOMPOSITION_GETCOMPOSED_PASS26
#define FABLE_RETAIL_CBANKFILE_GETSIZE_PASS26
#define FABLE_RETAIL_CCOMPOSITESURFACEINFO_MORPH_PASS26
#define FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_GETFRAME_POINTER_GUARD_PASS22
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_TOUCHFRAME_POINTER_GUARD_PASS22
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CBANKFILE_ENTRIES
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_CALCCOLOURDIFFERENCE
#define FABLE_RETAIL_CRGBCOLOUR
#include "fable_string.h"
#include "engine/CEngineSurfaceCompositionManager.h"
#include "engine/CCompositeSurfaceInfo.h"
#include "engine/CGraphicInfo.h"
#include "engine/C2DExtentsI.h"
#include "engine/CResourceList.h"
#include "engine/GFSqr.h"
#include <math.h>


double FABLE_FASTCALL GFGetTime();

CCharString FABLE_FASTCALL GFIntToCharString(long value);

// FableWin 02D8B9F0 constructs the pair from the two original morph fields.
#if defined(_MSC_VER)
__declspec(noinline)
#endif
inline unsigned long CEngineSurfaceCompositionManager::FindIndexInCompletedBank(const CEngineTextureMorph& morph) const
{
    const std::pair<long, long> key(morph.OriginalTextureIndex, morph.MorphID);
    const CCharString name = GFIntToCharString(key.first) + "-" + GFIntToCharString(key.second);
    return CompletedBankFile.FindIndexBySymbol(name);
}

// Same-TU helper: native 0x00B2AEF0 and donor 0x02D8D590 prove the stores and scope.
#if defined(_MSC_VER)
__declspec(noinline)
#endif
inline void CEngineSurfaceCompositionManager::GetDimensionsAndAdjustMipmapLevel(
    C2DExtentsI& dimensions, const CGraphicInfoFrame* frame, unsigned long& mipmap_level)
{
    dimensions.X = frame->GetWidth() >> mipmap_level;
    dimensions.Y = frame->GetHeight() >> mipmap_level;
    while (dimensions.X > MaxCompositeTextureSize || dimensions.Y > MaxCompositeTextureSize) {
        dimensions.X /= 2;
        dimensions.Y /= 2;
        ++mipmap_level;
    }
}

// FableWin 02D8D800 evaluates R, G, B and A in that order through GFSqr.
#if defined(_MSC_VER)
__declspec(noinline)
#endif
inline float CEngineSurfaceCompositionManager::CalcColourDifference(const CRGBColour& a, const CRGBColour& b)
{
    return sqrt(GFSqr(static_cast<float>(a.GetR() - b.GetR())) +
                GFSqr(static_cast<float>(a.GetG() - b.GetG())) +
                GFSqr(static_cast<float>(a.GetB() - b.GetB())) +
                GFSqr(static_cast<float>(a.GetA() - b.GetA())));
}

// FableWin 02D8D640 retains the count, typed blend references and early mismatch.
#if defined(_MSC_VER)
__declspec(noinline)
#endif
inline bool CEngineSurfaceCompositionManager::IsNearlyIdentical(const CEngineTextureMorph& a,
                                                            const CEngineTextureMorph& b, float tolerance)
{
    if (a.Blends.size() != b.Blends.size())
        return false;
    const unsigned int count = a.Blends.size();
    for (unsigned int i = 0; i < count; ++i) {
        const CCompositeBlendDesc& a_blend = a.Blends[i];
        const CCompositeBlendDesc& b_blend = b.Blends[i];
        if (a_blend.TextureIndex != b_blend.TextureIndex ||
            CalcColourDifference(a_blend.DiffuseColour, b_blend.DiffuseColour) > tolerance ||
            CalcColourDifference(a_blend.SpecularColour, b_blend.SpecularColour) > tolerance)
            return false;
    }
    return true;
}

bool CEngineSurfaceCompositionManager::GetComposedSurface(CGraphicFrame& dest,
    CCompositeSurfaceInfo& surface_info, unsigned long mipmap_level, bool is_bump)
{
    CEngineTextureMorph& morph = surface_info.GetCurrentMorph();
    const CGraphicInfoFrame* base_graphic_info = GetGraphicInfo(morph.OriginalTextureIndex);
    double current_time = GFGetTime();
    C2DExtentsI dimensions;
    GetDimensionsAndAdjustMipmapLevel(dimensions, base_graphic_info, mipmap_level);
    unsigned long no_blends = morph.Blends.size();

    bool has_cached_texture = false;
    bool near_identical = false;
    if (surface_info.Cache.Get()) {
        CCompositeSurfaceCache& surface_cache = *surface_info.Cache;
        if (!surface_cache.BankFileIndex && CompletedBankFile.GetSize())
            surface_cache.BankFileIndex = FindIndexInCompletedBank(morph);
        if (surface_cache.BankFileIndex) {
            const CGraphicInfoFrame* graphic_info = CompletedBankFile.PeekGraphicInfoFrame(surface_cache.BankFileIndex, 0);
            unsigned long width = graphic_info->GetWidth();
            while (mipmap_level > 0 && width < base_graphic_info->GetWidth()) {
                --mipmap_level;
                width *= 2;
            }
            surface_cache.LastUsedFrame = CurrentFrame;
            return graphic_info->GetFrame(dest, mipmap_level, 0);
        }
        if (surface_cache.GraphicFrame.IsInitialised()) {
            has_cached_texture = true;
            double delta_time = current_time - surface_cache.TimeStamp;
            float tolerance = ReuseTolerance;
            if (IsNearlyIdentical(morph, surface_info.Cache->CachedMorph, tolerance)) {
                near_identical = true;
                if (mipmap_level <= surface_info.Cache->MipmapLevel)
                    surface_info.Cache->TouchResource();
                if (surface_info.Cache->MipmapLevel <= mipmap_level) {
                    dest = surface_info.Cache->GraphicFrame;
                    return true;
                }
            }
        }
    }
    return false;
}