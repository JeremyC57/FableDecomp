// CMainGameComponent::InitGraphics() -- pick the texture formats (compressed when the display supports it and the option is on), create the main graphic bank "GBANK_MAIN" into PMainGraphicBank and make it the graphic appearances' bank, with three loading-progress ticks; returns true
// Retail 0x00416C8A, 494 bytes, plain `ret` after `leave`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, __fastcall DisplayProgress 0x009E9F40,
//   CGraphicDataBankInit() 0x00415B80 (manifest label "CNavigationLayer::CNavigationLayer" is wrong), the display
//   manager (0x013B8390) IsSupportingCompressedTextures 0x009BE800 and format pickers 0x009BE830/870/8B0 (compressed
//   non-alpha / alpha / boolean alpha), 0x009BE610 opaque, 0x009BE6C0 alpha, 0x009BE750 boolean alpha, 0x009BE590
//   signed, CPixelFormat::Initialise 0x009E3830 (D3DFMT_DXT5), CreateAndInitialize 0x009F83D0 (manager 0x013B837C),
//   CCountedPointer<CGraphicDataBank>::operator= 0x004194BA / dtor 0x00419108, __fastcall SetGraphicBank 0x004BBFC0
//   (manifest label "SetCurrentThingManager" is wrong; the graphic twin of SetMeshBank 0x004BBFD0).
// Globals: texture bit depth 0x013B7D68, maxima 0x013B7D64 / 0x013B7D60, compressed-textures option 0x01375453,
//   dither option 0x01375452.
// Layout: CGraphicDataBankInit (PDB, 0x2c) as the landed CNewFrontendGameComponent::InitialiseGraphics 0x0042DDB3;
//   CMainGameComponent PMainGraphicBank at +0x161C8 (PDB +0x161D0 less 8 bytes of narrower retail members).
// Shape: the frame-pointer size TU of the landed twin; the progress texts share one frame slot in their own blocks.
#pragma optimize("s", on)
#pragma optimize("y", off)
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
struct CPixelFormat {
    long Format;
    void Initialise(long d3dFormat);               // 0x009E3830
};
struct CGraphicDataBankInit {                      // 0x2c bytes
    CPixelFormat  NonAlphaFormat;                  // +0x00
    CPixelFormat  AlphaFormat;                     // +0x04
    CPixelFormat  InterpolatedAlphaFormat;         // +0x08
    CPixelFormat  BooleanAlphaFormat;              // +0x0c
    CPixelFormat  UncompressedNonAlphaFormat;      // +0x10
    CPixelFormat  UncompressedAlphaFormat;         // +0x14
    CPixelFormat  SignedFormat;                    // +0x18
    bool          GenerateMipmaps;                 // +0x1c
    unsigned long MaxGraphicWidth;                 // +0x20
    unsigned long MaxGraphicHeight;                // +0x24
    bool          AllowDither;                     // +0x28
    bool          AllowCompressedTextures;         // +0x29
    bool          AllowDXT1ForBooleanAlpha;        // +0x2a
    CGraphicDataBankInit();                        // 0x00415B80
};
struct CDisplayManager {
    bool IsSupportingCompressedTextures();                                                       // 0x009BE800
    bool GetBestCompressedNonAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);       // 0x009BE830
    bool GetBestCompressedAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);          // 0x009BE870
    bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);   // 0x009BE8B0
    bool GetBestOpaqueTextureFormat(unsigned long bitDepth, CPixelFormat* out, bool flag);        // 0x009BE610
    bool GetBestAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);                    // 0x009BE6C0
    bool GetBestBooleanAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);             // 0x009BE750
    bool GetBestSignedTextureFormat(unsigned long bitDepth, CPixelFormat* out);                   // 0x009BE590
};
struct CGraphicDataBank;
template <class T> class CCountedPointer {
public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer<T>& operator=(const CCountedPointer<T>& other);   // 0x004194BA
    ~CCountedPointer();                            // 0x00419108
    T& operator*() const { return *m_pObject; }
};
struct CGraphicDataBankManager {
    CCountedPointer<CGraphicDataBank> CreateAndInitialize(const CCharString& name, const CGraphicDataBankInit& init, bool a, bool b);   // 0x009F83D0
};
namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    static void __fastcall SetGraphicBank(CGraphicDataBank& bank);   // 0x004BBFC0
};
}
extern CDisplayManager* g_pDisplayManager;         // 0x013B8390
extern CGraphicDataBankManager* g_pGraphicDataBankManager;   // 0x013B837C
extern unsigned long g_TextureBitDepth;            // 0x013B7D68
extern unsigned long g_MaxGraphicWidth;            // 0x013B7D64
extern unsigned long g_MaxGraphicHeight;           // 0x013B7D60
extern bool g_AllowCompressedTextures;             // 0x01375453
extern bool g_AllowDither;                         // 0x01375452

struct CMainGameComponent {
    unsigned char                     _pad00[0x161c8];
    CCountedPointer<CGraphicDataBank> PMainGraphicBank;   // +0x161C8
    bool InitGraphics();
};

bool CMainGameComponent::InitGraphics()
{
    {
        CCharString text("Init Graphics", -1);
        DisplayProgress(text, -1.0f, false, false);
    }
    CGraphicDataBankInit init;
    if (g_pDisplayManager->IsSupportingCompressedTextures() && g_AllowCompressedTextures) {
        g_pDisplayManager->GetBestCompressedNonAlphaTextureFormat(g_TextureBitDepth, &init.NonAlphaFormat);
        g_pDisplayManager->GetBestCompressedAlphaTextureFormat(g_TextureBitDepth, &init.AlphaFormat);
        g_pDisplayManager->GetBestCompressedBooleanAlphaTextureFormat(g_TextureBitDepth, &init.BooleanAlphaFormat);
        init.InterpolatedAlphaFormat.Initialise(0x35545844);   // D3DFMT_DXT5
    } else {
        g_pDisplayManager->GetBestOpaqueTextureFormat(g_TextureBitDepth, &init.NonAlphaFormat, false);
        g_pDisplayManager->GetBestAlphaTextureFormat(g_TextureBitDepth, &init.AlphaFormat);
        g_pDisplayManager->GetBestBooleanAlphaTextureFormat(g_TextureBitDepth, &init.BooleanAlphaFormat);
    }
    g_pDisplayManager->GetBestAlphaTextureFormat(g_TextureBitDepth, &init.UncompressedAlphaFormat);
    g_pDisplayManager->GetBestOpaqueTextureFormat(g_TextureBitDepth, &init.UncompressedNonAlphaFormat, false);
    g_pDisplayManager->GetBestSignedTextureFormat(g_TextureBitDepth, &init.SignedFormat);
    init.MaxGraphicWidth = g_MaxGraphicWidth;
    init.MaxGraphicHeight = g_MaxGraphicHeight;
    init.GenerateMipmaps = true;
    init.AllowDither = g_AllowDither;
    {
        CCharString text("Opening Main Graphic Bank", -1);
        DisplayProgress(text, -1.0f, false, false);
    }
    {
        CCharString name("GBANK_MAIN", -1);
        PMainGraphicBank = g_pGraphicDataBankManager->CreateAndInitialize(name, init, true, true);
    }
    {
        CCharString text("Setting Main Graphic Bank", -1);
        DisplayProgress(text, -1.0f, false, false);
    }
    NGraphicAppearance::CTCGraphicAppearance::SetGraphicBank(*PMainGraphicBank);
    return true;
}