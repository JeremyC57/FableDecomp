// Behaviour test for 0x00B2D330 CEngineSurfaceCompositionManager::UpdateDiskCacheTexture: a host model of the body (same statement order,
// early returns and arithmetic) over recording mocks: the "<texture>-<morph>" bank symbol, the nearly-identical early exit, the new bank
// entry, the dimension fit below MaxCompositeTextureSize (mip level raised), the alpha / opaque handle table pick by GFGetTopBit, the
// per-layer frame fetch with its two failure exits, and the final render / remember / write / end-use sequence.
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <utility>
#include <vector>

struct Log { int frameEnds, waits, renders, writes, startUses, endUses, adds; long started, ended; long renderHandle; long renderX, renderY;
             bool renderAlpha; unsigned long writeIndex; bool writeAlpha; };
static Log g;

static unsigned long GFGetTopBit(unsigned long value)
{
    if (value == 0) return (unsigned long)-1;
    unsigned long bit = 0;
    while (value >>= 1) ++bit;
    return bit;
}

struct Blend { unsigned long TextureIndex; };
struct Morph { long OriginalTextureIndex, MorphID; std::vector<Blend> Blends; float Weight; };
struct Cache { Morph Current; unsigned long BankFileIndex, MipmapLevel; double TimeStamp; const Morph& GetCurrentMorph() const { return Current; } };
struct InfoFrame { unsigned long Width, Height; bool Alpha; bool Loadable; long LoadedLevel; };

struct Bank {
    std::vector<std::string> Symbols;
    unsigned long FindIndexBySymbol(const std::string& s) const {
        for (unsigned long i = 0; i < Symbols.size(); ++i) if (Symbols[i] == s) return i + 1;
        return 0;
    }
    void AddEntry(const std::string& s) { Symbols.push_back(s); ++g.adds; }
};

struct Manager {
    long MaxCompositeTextureSize;
    float DiskCacheTolerance;
    Bank IntermediateBankFile;
    std::map<std::pair<long, long>, Morph> GeneratedMorphs;
    std::pair<long, long> OpaqueTextureHandles[4][4], AlphaTextureHandles[4][4];
    InfoFrame Original;
    std::vector<InfoFrame> Layers;

    static bool IsNearlyIdentical(const Morph& a, const Morph& b, float tolerance) { float d = a.Weight - b.Weight; return (d < 0 ? -d : d) <= tolerance; }
    void GetDimensionsAndAdjustMipmapLevel(long& x, long& y, const InfoFrame* frame, unsigned long& mipmap_level) const {
        x = frame->Width >> mipmap_level; y = frame->Height >> mipmap_level;
        while (x > MaxCompositeTextureSize || y > MaxCompositeTextureSize) { x /= 2; y /= 2; ++mipmap_level; }
    }
    bool UpdateDiskCacheTexture(Cache& surface_cache, unsigned long mipmap_level, bool is_bump)
    {
        const Morph& morph = surface_cache.GetCurrentMorph();
        const InfoFrame* info_frame = &Original;
        ++g.frameEnds;
        ++g.waits;
        long texture_index = morph.OriginalTextureIndex;
        long morph_id = morph.MorphID;
        std::pair<long, long> key(texture_index, morph_id);
        char buf[64]; std::sprintf(buf, "%ld-%ld", texture_index, morph_id);
        std::string name(buf);
        unsigned long bank_index = IntermediateBankFile.FindIndexBySymbol(name);
        surface_cache.BankFileIndex = bank_index;
        std::map<std::pair<long, long>, Morph>::iterator it = GeneratedMorphs.find(key);
        if (it != GeneratedMorphs.end() && IsNearlyIdentical(morph, it->second, DiskCacheTolerance))
            return false;
        if (!bank_index) {
            IntermediateBankFile.AddEntry(name);
            surface_cache.BankFileIndex = IntermediateBankFile.FindIndexBySymbol(name);
        }
        long dx, dy;
        GetDimensionsAndAdjustMipmapLevel(dx, dy, info_frame, mipmap_level);
        bool alpha = info_frame->Alpha && !is_bump;
        unsigned long x = GFGetTopBit(dx) - GFGetTopBit(32);
        unsigned long y = GFGetTopBit(dy) - GFGetTopBit(32);
        std::pair<long, long> handles = alpha ? AlphaTextureHandles[x][y] : OpaqueTextureHandles[x][y];
        long compressed_handle = handles.second;
        ++g.startUses; g.started = compressed_handle;
        unsigned long layer_count = morph.Blends.size();
        for (unsigned long i = 0; i < layer_count; ++i) {
            const InfoFrame* layer_frame = &Layers[morph.Blends[i].TextureIndex];
            if (!layer_frame->Loadable)
                return false;
            if ((unsigned long)layer_frame->LoadedLevel > mipmap_level)
                return false;
        }
        ++g.renders; g.renderHandle = handles.first; g.renderX = dx; g.renderY = dy; g.renderAlpha = alpha;
        GeneratedMorphs[key] = surface_cache.GetCurrentMorph();
        surface_cache.MipmapLevel = mipmap_level;
        surface_cache.TimeStamp = 12.5;
        ++g.waits;
        ++g.writes; g.writeIndex = surface_cache.BankFileIndex; g.writeAlpha = alpha;
        ++g.endUses; g.ended = compressed_handle;
        return true;
    }
};

int main()
{
    static Manager m;
    m.MaxCompositeTextureSize = 256; m.DiskCacheTolerance = 0.01f;
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) {
        m.OpaqueTextureHandles[i][j] = std::make_pair(100 + i * 10 + j, 200 + i * 10 + j);
        m.AlphaTextureHandles[i][j] = std::make_pair(300 + i * 10 + j, 400 + i * 10 + j);
    }
    m.IntermediateBankFile.Symbols.push_back("1-1");
    InfoFrame original = { 1024, 512, true, true, 0 }; m.Original = original;
    InfoFrame l0 = { 64, 64, false, true, 0 }, l1 = { 64, 64, false, true, 2 }, l2 = { 64, 64, false, false, 0 };
    m.Layers.push_back(l0); m.Layers.push_back(l1); m.Layers.push_back(l2);
    bool ok = true;

    // 1: new morph 7-3, two loadable layers; 1024x512 >> 1 = 512x256 -> halve once: 256x128, level 2; alpha table [3][2]
    Cache c; c.Current.OriginalTextureIndex = 7; c.Current.MorphID = 3; c.Current.Weight = 0.5f;
    Blend b0 = { 0 }, b1 = { 1 }; c.Current.Blends.push_back(b0); c.Current.Blends.push_back(b1);
    c.BankFileIndex = 99; c.MipmapLevel = 0; c.TimeStamp = 0;
    ok = ok && m.UpdateDiskCacheTexture(c, 1, false);
    ok = ok && g.adds == 1 && m.IntermediateBankFile.Symbols.back() == "7-3" && c.BankFileIndex == 2;
    ok = ok && g.renders == 1 && g.renderX == 256 && g.renderY == 128 && g.renderAlpha && g.renderHandle == 332 && g.started == 432 && g.ended == 432;
    ok = ok && c.MipmapLevel == 2 && c.TimeStamp == 12.5 && g.writes == 1 && g.writeIndex == 2 && g.writeAlpha && g.waits == 2;
    ok = ok && m.GeneratedMorphs.size() == 1;

    // 2: same morph, nearly identical -> false before any bank add or scratch use
    ok = ok && !m.UpdateDiskCacheTexture(c, 1, false) && g.adds == 1 && g.startUses == 1 && c.BankFileIndex == 2;

    // 3: changed weight, bump -> opaque table, existing entry reused (no add)
    c.Current.Weight = 0.9f;
    ok = ok && m.UpdateDiskCacheTexture(c, 2, true) && g.adds == 1 && g.renderHandle == 132 && !g.writeAlpha && c.MipmapLevel == 2;

    // 4: a 128x64 original needs no fit (level 0); the layer is loaded only from level 2 -> false after the bank lookup
    Cache d; d.Current.OriginalTextureIndex = 1; d.Current.MorphID = 1; d.Current.Weight = 0; d.Current.Blends.push_back(b1);
    InfoFrame small = { 128, 64, false, true, 0 }; m.Original = small;
    ok = ok && !m.UpdateDiskCacheTexture(d, 0, false) && d.BankFileIndex == 1 && g.adds == 1 && g.renders == 2;

    // 5: an unloadable layer -> false, scratch texture use started and not ended (as retail)
    Blend b2 = { 2 };
    Cache e; e.Current.OriginalTextureIndex = 5; e.Current.MorphID = 0; e.Current.Weight = 0; e.Current.Blends.push_back(b0); e.Current.Blends.push_back(b2);
    ok = ok && !m.UpdateDiskCacheTexture(e, 1, false) && g.adds == 2 && e.BankFileIndex == 3 && g.startUses == 4 && g.endUses == 2 && g.renders == 2;
    if (!ok) { std::printf("00b2d330_TEST FAIL\n"); return 1; }
    std::printf("00b2d330_TEST PASS\n");
    return 0;
}