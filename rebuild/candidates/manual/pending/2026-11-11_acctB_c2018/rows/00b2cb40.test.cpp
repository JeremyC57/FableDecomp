// Behaviour test for 0x00B2CB40 CEngineSurfaceCompositionManager::CEngineSurfaceCompositionManager(): a host model of the
// constructor body (same statement order, same index arithmetic) over mock display / resource / scratch managers.
#include <cstdio>
#include <cstring>

static unsigned long GFGetTopBit(unsigned long value)
{
    if (value == 0)
        return (unsigned long)-1;
    if (value >= 0x80000000UL)
        return 31;
    ++value;
    long r = ((long)((1L << 16) - (long)value) >> 31) & 16;
    r += ((long)((1L << (r + 8)) - (long)value) >> 31) & 8;
    r += ((long)((1L << (r + 4)) - (long)value) >> 31) & 4;
    r += ((long)((1L << (r + 2)) - (long)value) >> 31) & 2;
    r += ((long)((1L << (r + 1)) - (long)value) >> 31) & 1;
    return r;
}

struct PixelFormat { long FormatIndex; PixelFormat() : FormatIndex(-1) { } };
struct Extents { long X, Y; Extents(long x, long y) : X(x), Y(y) { } };
struct HandlePair { long first, second; HandlePair() : first(0), second(0) { } };
struct BankInit {
    PixelFormat NonAlphaFormat, AlphaFormat, InterpolatedAlphaFormat, BooleanAlphaFormat, UncompressedNonAlphaFormat,
                UncompressedAlphaFormat, SignedFormat;
    bool GenerateMipmaps; unsigned long MaxGraphicWidth, MaxGraphicHeight; bool AllowDither, AllowCompressedTextures, AllowDXT1ForBooleanAlpha;
    BankInit() : GenerateMipmaps(false), MaxGraphicWidth((unsigned long)-1), MaxGraphicHeight((unsigned long)-1), AllowDither(true),
                 AllowCompressedTextures(false), AllowDXT1ForBooleanAlpha(true) { }
};
struct Bank {
    PixelFormat PixelFormats[11]; bool Initialised; bool StaticMap; unsigned long MaxW;
    Bank() : Initialised(false), StaticMap(false), MaxW(0) { }
    void Initialise(const BankInit& init) {
        Initialised = true; MaxW = init.MaxGraphicWidth;
        PixelFormats[0] = init.NonAlphaFormat; PixelFormats[1] = init.AlphaFormat;
    }
    void SetIsStaticMap(bool s) { StaticMap = s; }
    PixelFormat GetPixelFormat(int type) const { return PixelFormats[type]; }
};
// display manager: opaque 32-bit fails, everything else succeeds with a format id that encodes (kind, bit depth)
static int g_opaque_calls = 0;
struct DisplayManager {
    bool GetBestOpaqueTextureFormat(unsigned long bits, PixelFormat& f, bool) { ++g_opaque_calls; if (bits == 32 && g_opaque_calls == 1) return false; f.FormatIndex = 100 + (long)bits; return true; }
    bool GetBestAlphaTextureFormat(unsigned long bits, PixelFormat& f) { f.FormatIndex = 200 + (long)bits; return true; }
    bool GetBestCompressedOpaqueTextureFormat(unsigned long bits, PixelFormat& f) { f.FormatIndex = 300 + (long)bits; return true; }
    bool GetBestCompressedAlphaTextureFormat(unsigned long bits, PixelFormat& f) { f.FormatIndex = 400 + (long)bits; return true; }
    bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long bits, PixelFormat& f) { f.FormatIndex = 500 + (long)bits; return true; }
    bool GetBestSignedTextureFormat(unsigned long bits, PixelFormat& f) { f.FormatIndex = 600 + (long)bits; return true; }
};
struct Alloc { long page; long w, h, mips; long format; unsigned long usage; bool flag; };
struct Scratch {
    long pages; Alloc allocs[64]; long count;
    Scratch() : pages(0), count(0) { }
    long AllocteMemoryPage(long, bool) { return ++pages; }
    long AllocateTexture(long page, const Extents& e, long mips, const PixelFormat& f, unsigned long usage, bool flag) {
        Alloc a = { page, e.X, e.Y, mips, f.FormatIndex, usage, flag }; allocs[count] = a; return 1000 + count++;
    }
};
static DisplayManager g_display; static Scratch g_scratch; static char g_cache[32] = "cache\\";

struct Manager {
    Bank IntermediateBankFile, CompletedBankFile, CustomTextureBankFile;
    char IntermediateDiskCacheFileName[64], CompletedDiskCacheFileName[64], CustomTextureBankFileName[64];
    float DiskCacheTolerance, MaxPreloadTime; unsigned long CurrentFrame;
    PixelFormat OpaquePixelFormat, AlphaPixelFormat, CompressedOpaquePixelFormat, CompressedAlphaPixelFormat;
    HandlePair OpaqueTextureHandles[4][4], AlphaTextureHandles[4][4];
    float ReuseTolerance, ToleranceFalloffPercent; long MaxCompositeTextureSize; unsigned long MinRenderTargetSize, MinMipMapLevels;
    bool Enable;
    Manager()
    {
        ReuseTolerance = 10.0f;
        ToleranceFalloffPercent = 10.0f;
        MinRenderTargetSize = 64;
        MinMipMapLevels = 5;
        MaxCompositeTextureSize = 256;
        if (!g_display.GetBestOpaqueTextureFormat(32, OpaquePixelFormat, false))
            g_display.GetBestOpaqueTextureFormat(16, OpaquePixelFormat, false);
        g_display.GetBestAlphaTextureFormat(32, AlphaPixelFormat);
        std::strcpy(CompletedDiskCacheFileName, g_cache); std::strcat(CompletedDiskCacheFileName, "comfront.dat");
        std::strcpy(IntermediateDiskCacheFileName, g_cache); std::strcat(IntermediateDiskCacheFileName, "comback.dat");
        std::strcpy(CustomTextureBankFileName, g_cache); std::strcat(CustomTextureBankFileName, "comuser.dat");
        DiskCacheTolerance = 0.01f;
        MaxPreloadTime = 5.0f;
        CurrentFrame = 0;
        BankInit init;
        g_display.GetBestCompressedOpaqueTextureFormat(32, init.NonAlphaFormat);
        g_display.GetBestCompressedAlphaTextureFormat(32, init.AlphaFormat);
        g_display.GetBestCompressedBooleanAlphaTextureFormat(16, init.BooleanAlphaFormat);
        g_display.GetBestAlphaTextureFormat(32, init.UncompressedAlphaFormat);
        g_display.GetBestOpaqueTextureFormat(32, init.UncompressedNonAlphaFormat, false);
        g_display.GetBestSignedTextureFormat(16, init.SignedFormat);
        init.MaxGraphicWidth = MaxCompositeTextureSize;
        init.MaxGraphicHeight = MaxCompositeTextureSize;
        init.GenerateMipmaps = false;
        init.AllowDither = false;
        IntermediateBankFile.Initialise(init); IntermediateBankFile.SetIsStaticMap(true);
        CompletedBankFile.Initialise(init); CompletedBankFile.SetIsStaticMap(true);
        CustomTextureBankFile.Initialise(init); CustomTextureBankFile.SetIsStaticMap(true);
        CompressedAlphaPixelFormat = CompletedBankFile.GetPixelFormat(1);
        CompressedOpaquePixelFormat = CompletedBankFile.GetPixelFormat(0);
        Scratch& scratch = g_scratch;
        for (long width = 32; width <= 256; width <<= 1) {
            for (long height = 32; height <= 256; height <<= 1) {
                unsigned long x = GFGetTopBit(width) - GFGetTopBit(32);
                unsigned long y = GFGetTopBit(height) - GFGetTopBit(32);
                long opaque_page = scratch.AllocteMemoryPage(0, true);
                OpaqueTextureHandles[x][y].first = scratch.AllocateTexture(opaque_page, Extents(width, height), 1, OpaquePixelFormat, 1, false);
                OpaqueTextureHandles[x][y].second = scratch.AllocateTexture(opaque_page, Extents(width, height), -1, CompressedOpaquePixelFormat, 0, true);
                long alpha_page = scratch.AllocteMemoryPage(0, true);
                AlphaTextureHandles[x][y].first = scratch.AllocateTexture(alpha_page, Extents(width, height), 1, AlphaPixelFormat, 1, false);
                AlphaTextureHandles[x][y].second = scratch.AllocateTexture(alpha_page, Extents(width, height), -1, CompressedAlphaPixelFormat, 0, true);
            }
        }
        Enable = true;
    }
};

int main()
{
    bool ok = true;
    ok = ok && GFGetTopBit(32) == 5 && GFGetTopBit(256) == 8 && GFGetTopBit(0) == (unsigned long)-1 && GFGetTopBit(0x80000000UL) == 31 && GFGetTopBit(100) == 6;
    Manager m;
    ok = ok && m.Enable && m.OpaquePixelFormat.FormatIndex == 116 && m.AlphaPixelFormat.FormatIndex == 232;
    ok = ok && m.CompressedOpaquePixelFormat.FormatIndex == 332 && m.CompressedAlphaPixelFormat.FormatIndex == 432;
    ok = ok && m.CompletedBankFile.Initialised && m.CustomTextureBankFile.StaticMap && m.IntermediateBankFile.MaxW == 256;
    ok = ok && std::strcmp(m.CompletedDiskCacheFileName, "cache\\comfront.dat") == 0 && std::strcmp(m.IntermediateDiskCacheFileName, "cache\\comback.dat") == 0;
    ok = ok && g_scratch.count == 64 && g_scratch.pages == 32;
    for (int i = 0; i < 4 && ok; ++i)
        for (int j = 0; j < 4 && ok; ++j) {
            const Alloc& a = g_scratch.allocs[m.OpaqueTextureHandles[i][j].first - 1000];
            const Alloc& b = g_scratch.allocs[m.OpaqueTextureHandles[i][j].second - 1000];
            const Alloc& c = g_scratch.allocs[m.AlphaTextureHandles[i][j].first - 1000];
            const Alloc& d = g_scratch.allocs[m.AlphaTextureHandles[i][j].second - 1000];
            ok = a.w == (32L << i) && a.h == (32L << j) && a.mips == 1 && a.format == 116 && a.usage == 1 && !a.flag
              && b.page == a.page && b.mips == -1 && b.format == 332 && b.usage == 0 && b.flag && b.w == a.w && b.h == a.h
              && c.page == a.page + 1 && c.format == 232 && d.page == c.page && d.format == 432 && d.flag;
        }
    if (!ok) { std::printf("00b2cb40_TEST FAIL\n"); return 1; }
    std::printf("00b2cb40_TEST PASS\n");
    return 0;
}
