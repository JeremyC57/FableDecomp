#include <cstdio>
#include <cstring>
// Behaviour test for CMainGameComponent::InitGraphics (0x00416C8A): the compressed / uncompressed format choice, the
// shared uncompressed pickers, the init fields, the bank creation and the three progress ticks.
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CPixelFormat { public: long Format; void Initialise(long f) { Format = f; } };
class CGraphicDataBankInit { public:
    CPixelFormat NonAlphaFormat, AlphaFormat, InterpolatedAlphaFormat, BooleanAlphaFormat,
                 UncompressedNonAlphaFormat, UncompressedAlphaFormat, SignedFormat;
    bool GenerateMipmaps; unsigned int MaxGraphicWidth, MaxGraphicHeight; bool AllowDither;
    CGraphicDataBankInit()
    {
        NonAlphaFormat.Format = AlphaFormat.Format = InterpolatedAlphaFormat.Format = BooleanAlphaFormat.Format = -1;
        UncompressedNonAlphaFormat.Format = UncompressedAlphaFormat.Format = SignedFormat.Format = -1;
        GenerateMipmaps = false; MaxGraphicWidth = MaxGraphicHeight = 0xffffffffu; AllowDither = true;
    }
};
class CDisplayManager { public:
    bool Compressed;
    bool IsSupportingCompressedTextures() { return Compressed; }
    void CNA(unsigned int d, CPixelFormat* o) { o->Format = 100 + d; }
    void CA(unsigned int d, CPixelFormat* o) { o->Format = 200 + d; }
    void CBA(unsigned int d, CPixelFormat* o) { o->Format = 300 + d; }
    void Opaque(unsigned int d, CPixelFormat* o, bool) { o->Format = 400 + d; }
    void Alpha(unsigned int d, CPixelFormat* o) { o->Format = 500 + d; }
    void BoolAlpha(unsigned int d, CPixelFormat* o) { o->Format = 600 + d; }
    void Signed(unsigned int d, CPixelFormat* o) { o->Format = 700 + d; }
};
static CDisplayManager g_dm;
static CDisplayManager* g_pDisplayManager = &g_dm;
static unsigned int g_TextureBitDepth = 32, g_MaxGraphicWidth = 1024, g_MaxGraphicHeight = 512;
static bool g_AllowCompressedTextures = true, g_AllowDither = false;
static CGraphicDataBankInit g_seen;
static int g_bank = 7;
static int* g_appearanceBank = 0;
struct CMain {
    int* PMainGraphicBank;
    bool InitGraphics()
    {
        Log("Init Graphics");
        CGraphicDataBankInit init;
        if (g_pDisplayManager->IsSupportingCompressedTextures() && g_AllowCompressedTextures) {
            g_pDisplayManager->CNA(g_TextureBitDepth, &init.NonAlphaFormat);
            g_pDisplayManager->CA(g_TextureBitDepth, &init.AlphaFormat);
            g_pDisplayManager->CBA(g_TextureBitDepth, &init.BooleanAlphaFormat);
            init.InterpolatedAlphaFormat.Initialise(0x35545844);
        } else {
            g_pDisplayManager->Opaque(g_TextureBitDepth, &init.NonAlphaFormat, false);
            g_pDisplayManager->Alpha(g_TextureBitDepth, &init.AlphaFormat);
            g_pDisplayManager->BoolAlpha(g_TextureBitDepth, &init.BooleanAlphaFormat);
        }
        g_pDisplayManager->Alpha(g_TextureBitDepth, &init.UncompressedAlphaFormat);
        g_pDisplayManager->Opaque(g_TextureBitDepth, &init.UncompressedNonAlphaFormat, false);
        g_pDisplayManager->Signed(g_TextureBitDepth, &init.SignedFormat);
        init.MaxGraphicWidth = g_MaxGraphicWidth;
        init.MaxGraphicHeight = g_MaxGraphicHeight;
        init.GenerateMipmaps = true;
        init.AllowDither = g_AllowDither;
        Log("Opening Main Graphic Bank");
        g_seen = init;
        PMainGraphicBank = &g_bank;
        Log("Setting Main Graphic Bank");
        g_appearanceBank = PMainGraphicBank;
        return true;
    }
};
static int Fail() { std::printf("00416c8a_TEST FAIL\n"); return 1; }
int main()
{
    CMain m; m.PMainGraphicBank = 0;
    g_dm.Compressed = true;
    if (!m.InitGraphics()) return Fail();
    if (g_seen.NonAlphaFormat.Format != 132 || g_seen.AlphaFormat.Format != 232 || g_seen.BooleanAlphaFormat.Format != 332
        || g_seen.InterpolatedAlphaFormat.Format != 0x35545844) return Fail();
    if (g_seen.UncompressedAlphaFormat.Format != 532 || g_seen.UncompressedNonAlphaFormat.Format != 432
        || g_seen.SignedFormat.Format != 732) return Fail();
    if (!g_seen.GenerateMipmaps || g_seen.MaxGraphicWidth != 1024 || g_seen.MaxGraphicHeight != 512 || g_seen.AllowDither)
        return Fail();
    if (g_appearanceBank != &g_bank) return Fail();
    if (std::strcmp(g_log, "Init Graphics;Opening Main Graphic Bank;Setting Main Graphic Bank;") != 0) return Fail();
    g_AllowCompressedTextures = false;
    m.InitGraphics();
    if (g_seen.NonAlphaFormat.Format != 432 || g_seen.AlphaFormat.Format != 532 || g_seen.BooleanAlphaFormat.Format != 632
        || g_seen.InterpolatedAlphaFormat.Format != -1) return Fail();
    std::printf("00416c8a_TEST PASS\n");
    return 0;
}