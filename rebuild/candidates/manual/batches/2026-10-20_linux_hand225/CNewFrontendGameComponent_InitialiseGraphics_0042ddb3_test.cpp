// Behaviour test for 0x0042DDB3 CNewFrontendGameComponent::InitialiseGraphics: the menu font comes from the font bank,
// every format slot gets its picker's choice (DXT5 for interpolated alpha), mipmaps/2048x2048/dither are set and the
// "GBANK_FRONT_END" bank is created with that init; returns true.
#include <cstdio>
#include <cstring>

struct CPixelFormat { long Format; void Initialise(long f) { Format = f; } };
struct CGraphicDataBankInit {
    CPixelFormat NonAlphaFormat, AlphaFormat, InterpolatedAlphaFormat, BooleanAlphaFormat, UncompressedNonAlphaFormat, UncompressedAlphaFormat, SignedFormat;
    bool GenerateMipmaps; unsigned long MaxGraphicWidth, MaxGraphicHeight; bool AllowDither, AllowCompressedTextures, AllowDXT1ForBooleanAlpha;
    CGraphicDataBankInit() { CPixelFormat* f = &NonAlphaFormat; for (int i = 0; i < 7; ++i) f[i].Format = -1; GenerateMipmaps = false; MaxGraphicWidth = MaxGraphicHeight = (unsigned long)-1; AllowDither = false; AllowCompressedTextures = true; AllowDXT1ForBooleanAlpha = false; }
};
struct CDisplayManager {
    bool GetBestCompressedNonAlphaTextureFormat(unsigned long d, CPixelFormat* o) { o->Format = 100 + (long)d; return true; }
    bool GetBestCompressedAlphaTextureFormat(unsigned long d, CPixelFormat* o) { o->Format = 200 + (long)d; return true; }
    bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long d, CPixelFormat* o) { o->Format = 300 + (long)d; return true; }
    bool GetBestAlphaTextureFormat(unsigned long d, CPixelFormat* o) { o->Format = 400 + (long)d; return true; }
    bool GetBestOpaqueTextureFormat(unsigned long d, CPixelFormat* o, bool f) { o->Format = 500 + (long)d + (f ? 1 : 0); return true; }
    bool GetBestSignedTextureFormat(unsigned long d, CPixelFormat* o) { o->Format = 600 + (long)d; return true; }
};
static CDisplayManager g_display; static int g_systemCalls;
struct CSystemManager { CDisplayManager* DisplayManager; };
static CSystemManager g_system = { &g_display };
static CSystemManager* GFGetSystemManager() { ++g_systemCalls; return &g_system; }
static CGraphicDataBankInit g_created; static char g_bankName[32];
struct CNewFrontendGameComponent {
    int m_pMenuFont; int m_pFrontEndGraphicBank;
    bool InitialiseGraphics()
    {
        m_pMenuFont = 77;                                     // g_pFontBank->GetFont(g_FrontEndMenuFontName)
        CGraphicDataBankInit init;
        CSystemManager* system;
        system = GFGetSystemManager(); system->DisplayManager->GetBestCompressedNonAlphaTextureFormat(0x20, &init.NonAlphaFormat);
        system = GFGetSystemManager(); system->DisplayManager->GetBestCompressedAlphaTextureFormat(0x20, &init.AlphaFormat);
        system = GFGetSystemManager(); system->DisplayManager->GetBestCompressedBooleanAlphaTextureFormat(0x10, &init.BooleanAlphaFormat);
        system = GFGetSystemManager(); system->DisplayManager->GetBestAlphaTextureFormat(0x20, &init.UncompressedAlphaFormat);
        system = GFGetSystemManager(); system->DisplayManager->GetBestOpaqueTextureFormat(0x20, &init.UncompressedNonAlphaFormat, false);
        system = GFGetSystemManager(); system->DisplayManager->GetBestSignedTextureFormat(0x10, &init.SignedFormat);
        init.InterpolatedAlphaFormat.Initialise(0x35545844);
        init.GenerateMipmaps = true; init.MaxGraphicWidth = 0x800; init.MaxGraphicHeight = 0x800; init.AllowDither = true;
        { std::strcpy(g_bankName, "GBANK_FRONT_END"); g_created = init; m_pFrontEndGraphicBank = 1; }
        return true;
    }
};
int main()
{
    CNewFrontendGameComponent c = { 0, 0 };
    bool ok = c.InitialiseGraphics();
    const CGraphicDataBankInit& i = g_created;
    if (!ok || c.m_pMenuFont != 77 || c.m_pFrontEndGraphicBank != 1 || g_systemCalls != 6 || std::strcmp(g_bankName, "GBANK_FRONT_END")) { std::printf("0042ddb3_TEST FAIL\n"); return 1; }
    if (i.NonAlphaFormat.Format != 132 || i.AlphaFormat.Format != 232 || i.BooleanAlphaFormat.Format != 316 || i.UncompressedAlphaFormat.Format != 432
        || i.UncompressedNonAlphaFormat.Format != 532 || i.SignedFormat.Format != 616 || i.InterpolatedAlphaFormat.Format != 0x35545844) { std::printf("0042ddb3_TEST FAIL\n"); return 1; }
    if (!i.GenerateMipmaps || i.MaxGraphicWidth != 2048 || i.MaxGraphicHeight != 2048 || !i.AllowDither || !i.AllowCompressedTextures) { std::printf("0042ddb3_TEST FAIL\n"); return 1; }
    std::printf("0042ddb3_TEST PASS\n");
    return 0;
}
