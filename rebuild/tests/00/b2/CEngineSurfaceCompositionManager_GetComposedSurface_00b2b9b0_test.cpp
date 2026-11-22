// Standalone tests for baked and reusable composed surface selection.
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include "rebuild_abi.h"
#include "engine/C2DExtentsI.h"
#include "engine/GFSqr.h"

#ifdef _MSC_VER
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CCOMPOSITEBLENDDESC_ORIGINAL_CTOR_PASS26
#define FABLE_RETAIL_CCOMPOSITEBLENDDESC_ORIGINAL_IMPLICIT_COPY_PASS26
#include "engine/CCompositeBlendDesc.h"
#else
// Expose the same colour channels to the portable fixture as the Win32 packed union.
class CRGBColour {
public:
    CRGBColour(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
        : R(red), G(green), B(blue), A(alpha) {}
    unsigned char GetR() const { return R; }
    unsigned char GetG() const { return G; }
    unsigned char GetB() const { return B; }
    unsigned char GetA() const { return A; }
private:
    unsigned char R, G, B, A;
};

enum ECompositeBlendType { COMPOSITE_BLEND_ALPHA = 2, COMPOSITE_BLEND_SOLID = 3 };

struct CCompositeBlendDesc {
    CCompositeBlendDesc(ECompositeBlendType blend, unsigned long index,
                        const CRGBColour& diffuse, const CRGBColour& specular)
        : TextureIndex(index), BlendType(blend), DiffuseColour(diffuse), SpecularColour(specular) {}
    unsigned long TextureIndex;
    ECompositeBlendType BlendType;
    CRGBColour DiffuseColour, SpecularColour;
};
#endif

typedef std::string CCharString;

CCharString FABLE_FASTCALL GFIntToCharString(long value)
{
    char text[24];
    sprintf(text, "%ld", value);
    return text;
}

class CGraphicFrame {
public:
    CGraphicFrame() : State(0) {}
    bool IsInitialised() const { return State != 0; }
    int State;
};

class CGraphicInfoFrame {
public:
    CGraphicInfoFrame() : Width(0), Height(0), Result(false), Calls(0), WidthCalls(0), HeightCalls(0), GotMipmap(0), GotFlags(0) {}
    unsigned long GetWidth() const { ++WidthCalls; return Width; }
    unsigned long GetHeight() const { ++HeightCalls; return Height; }
    bool GetFrame(CGraphicFrame& dest, unsigned long level, unsigned long flags) const
    {
        ++Calls;
        GotMipmap = level;
        GotFlags = flags;
        if (Result)
            dest.State = 25;
        return Result;
    }
    unsigned long Width, Height;
    bool Result;
    mutable unsigned Calls, WidthCalls, HeightCalls;
    mutable unsigned long GotMipmap, GotFlags;
};

struct CEngineTextureMorph {
    CEngineTextureMorph() : OriginalTextureIndex(3), MorphID(8) {}
    long OriginalTextureIndex, MorphID;
    std::vector<CCompositeBlendDesc> Blends;
};

class CCompositeSurfaceCache {
public:
    CCompositeSurfaceCache() : MipmapLevel(2), BankFileIndex(0), LastUsedFrame(0), TimeStamp(4), Touches(0) {}
    void TouchResource() { ++Touches; }

    CEngineTextureMorph CurrentMorph, CachedMorph;
    CGraphicFrame GraphicFrame;
    unsigned long MipmapLevel, BankFileIndex, LastUsedFrame;
    double TimeStamp;
    unsigned Touches;
};

class CachePointer {
public:
    CachePointer() : Data(0) {}
    CCompositeSurfaceCache* Get() const { return Data; }
    CCompositeSurfaceCache* operator->() const { return Data; }
    CCompositeSurfaceCache& operator*() const { return *Data; }
    CCompositeSurfaceCache* Data;
};

class CCompositeSurfaceInfo {
public:
    CEngineTextureMorph& GetCurrentMorph() { return Cache->CurrentMorph; }
    CachePointer Cache;
};

class CompletedBank {
public:
    CompletedBank() : Size(0), Calls(0), LookupCalls(0), Index(0), LookupIndex(0), Frame(0) {}
    long GetSize() const { return Size; }
    unsigned long FindIndexBySymbol(const CCharString& name) const
    {
        ++LookupCalls;
        return name == "3-8" ? LookupIndex : 0;
    }
    const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long index, unsigned long frame) const
    {
        if (frame != 0)
            return 0;
        ++Calls;
        Index = index;
        return Frame;
    }

    long Size;
    mutable unsigned Calls, LookupCalls;
    mutable unsigned long Index;
    unsigned long LookupIndex;
    const CGraphicInfoFrame* Frame;
};

static unsigned ClockCalls;
double GFGetTime() { ++ClockCalls; return 6.0; }

class CEngineSurfaceCompositionManager {
public:
    CEngineSurfaceCompositionManager() : CurrentFrame(54), ReuseTolerance(0.45f),
        MaxCompositeTextureSize(200), IndexRight(false), PendingInvalidation(0) {}
    bool GetComposedSurface(CGraphicFrame&, CCompositeSurfaceInfo&, unsigned long, bool);
    const CGraphicInfoFrame* GetGraphicInfo(unsigned long index)
    {
        IndexRight = index == 3;
        if (PendingInvalidation)
            PendingInvalidation->Data = 0;
        return &Source;
    }
    void GetDimensionsAndAdjustMipmapLevel(C2DExtentsI&, const CGraphicInfoFrame*, unsigned long&);
    unsigned long FindIndexInCompletedBank(const CEngineTextureMorph&) const;
    bool IsNearlyIdentical(const CEngineTextureMorph&, const CEngineTextureMorph&, float);
    float CalcColourDifference(const CRGBColour&, const CRGBColour&);

    CompletedBank CompletedBankFile;
    CGraphicInfoFrame Source;
    unsigned long CurrentFrame;
    float ReuseTolerance;
    long MaxCompositeTextureSize;
    bool IndexRight;
    CachePointer* PendingInvalidation;
};

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

struct Scenario {
    Scenario()
    {
        Info.Cache.Data = &Cache;
        Cache.CachedMorph.MorphID = 12;
        Manager.Source.Width = 128;
        Manager.Source.Height = 64;
        Manager.CompletedBankFile.Frame = &DiskFrame;
        DiskFrame.Width = 16;
        Dest.State = 1;
        ClockCalls = 0;
    }

    bool Fetch(unsigned long mipmapLevel)
    {
        return Manager.GetComposedSurface(Dest, Info, mipmapLevel, false);
    }

    bool CheckDependencies() const
    {
        return ClockCalls == 1 && Manager.IndexRight && Manager.Source.WidthCalls && Manager.Source.HeightCalls;
    }

    CEngineSurfaceCompositionManager Manager;
    CCompositeSurfaceCache Cache;
    CCompositeSurfaceInfo Info;
    CGraphicInfoFrame DiskFrame;
    CGraphicFrame Dest;
};

int main()
{
    {
        Scenario test;
        test.Manager.PendingInvalidation = &test.Info.Cache;
        if (test.Fetch(0) || test.Dest.State != 1 || !test.CheckDependencies())
            return 1;
    }
    {
        Scenario test;
        if (test.Fetch(0) || test.Manager.CompletedBankFile.LookupCalls != 0 ||
            test.Cache.Touches != 0 || test.Dest.State != 1 || !test.CheckDependencies())
            return 2;
    }
    {
        Scenario test;
        test.Manager.CompletedBankFile.Size = 8;
        test.Manager.CompletedBankFile.LookupIndex = 0;
        test.Cache.GraphicFrame.State = 19;
        const CRGBColour black(0, 0, 0, 0);
        test.Cache.CurrentMorph.Blends.push_back(CCompositeBlendDesc(COMPOSITE_BLEND_ALPHA, 4, black, black));
        if (test.Fetch(2) || test.Manager.CompletedBankFile.LookupCalls != 1 ||
            test.Cache.Touches != 0 || test.Dest.State != 1)
            return 3;
    }
    {
        Scenario test;
        test.Manager.CompletedBankFile.Size = 8;
        test.Manager.CompletedBankFile.LookupIndex = 5;
        test.DiskFrame.Result = true;
        if (!test.Fetch(4) || test.Manager.CompletedBankFile.LookupCalls != 1 || !test.CheckDependencies() ||
            test.Cache.BankFileIndex != 5 || test.Cache.LastUsedFrame != 54 || test.Dest.State != 25 ||
            test.DiskFrame.Calls != 1 || test.DiskFrame.GotMipmap != 1 ||
            test.DiskFrame.GotFlags != 0 || test.Manager.CompletedBankFile.Index != 5)
            return 4;
    }
    {
        Scenario test;
        test.Cache.BankFileIndex = 7;
        test.DiskFrame.Width = 128;
        if (test.Fetch(2) || test.Cache.LastUsedFrame != 54 || test.Dest.State != 1 ||
            test.Manager.CompletedBankFile.LookupCalls != 0 || test.DiskFrame.Calls != 1 ||
            test.DiskFrame.GotMipmap != 2 || test.Manager.CompletedBankFile.Index != 7)
            return 5;
    }
    {
        Scenario test;
        test.Cache.GraphicFrame.State = 19;
        if (test.Fetch(1) || test.Cache.Touches != 1 || test.Dest.State != 1)
            return 6;
    }
    {
        Scenario test;
        test.Cache.GraphicFrame.State = 19;
        if (!test.Fetch(2) || test.Cache.Touches != 1 || test.Dest.State != 19)
            return 7;
    }
    {
        Scenario test;
        test.Cache.GraphicFrame.State = 19;
        if (!test.Fetch(3) || test.Cache.Touches != 0 || test.Dest.State != 19)
            return 8;
    }
    {
        Scenario test;
        C2DExtentsI dimensions;
        test.Manager.Source.Width = 256;
        test.Manager.Source.Height = 512;
        unsigned long mipmap = 0;
        test.Manager.GetDimensionsAndAdjustMipmapLevel(dimensions, &test.Manager.Source, mipmap);
        if (dimensions.X != 64 || dimensions.Y != 128 || mipmap != 2)
            return 9;
        test.Manager.CompletedBankFile.LookupIndex = 5;
        if (test.Manager.FindIndexInCompletedBank(test.Cache.CurrentMorph) != 5)
            return 10;
        test.Cache.CurrentMorph.MorphID = 9;
        if (test.Manager.FindIndexInCompletedBank(test.Cache.CurrentMorph) != 0)
            return 11;
    }
    {
        Scenario test;
        CEngineTextureMorph& current = test.Cache.CurrentMorph;
        CEngineTextureMorph& cached = test.Cache.CachedMorph;
        const CRGBColour black(0, 0, 0, 0);
        const CRGBColour red_green(3, 4, 0, 0);
        const CRGBColour blue_alpha(0, 0, 3, 4);
        if (test.Manager.CalcColourDifference(black, red_green) != 5.0f ||
            test.Manager.CalcColourDifference(black, blue_alpha) != 5.0f)
            return 12;
        current.Blends.push_back(CCompositeBlendDesc(COMPOSITE_BLEND_ALPHA, 4, black, black));
        if (test.Manager.IsNearlyIdentical(current, cached, 5.0f))
            return 13;
        cached.Blends.push_back(CCompositeBlendDesc(COMPOSITE_BLEND_SOLID, 6, black, black));
        if (test.Manager.IsNearlyIdentical(current, cached, 5.0f))
            return 14;
        cached.Blends[0].TextureIndex = 4;
        cached.Blends[0].DiffuseColour = red_green;
        if (test.Manager.IsNearlyIdentical(current, cached, 4.0f) ||
            !test.Manager.IsNearlyIdentical(current, cached, 5.0f))
            return 15;
        cached.Blends[0].DiffuseColour = black;
        cached.Blends[0].SpecularColour = blue_alpha;
        if (test.Manager.IsNearlyIdentical(current, cached, 4.0f) ||
            !test.Manager.IsNearlyIdentical(current, cached, 5.0f))
            return 16;
    }
    puts("00b2b9b0_TEST PASS");
}