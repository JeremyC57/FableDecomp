// CNewFrontendGameComponent::InitialiseGraphics() -- fetch the front-end menu font, then pick the best texture formats from the display manager and create the "GBANK_FRONT_END" graphic bank (mipmaps, 2048x2048 limit, dither) into m_pFrontEndGraphicBank; returns true
#pragma optimize("s", on)
#pragma optimize("y", off)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042DDB3, 274 bytes, plain `ret` after `leave`.
// Pragma: size TU with a frame pointer (`optimize("s", on)` + `("y", off)`), as the landed frontend/main-game rows.
// Callees: CFontBank::GetFont(const CCharString&) 0x009E2C80 (bank 0x013B838C, name the global CCharString 0x013B875C,
//   counted pointer by hidden pointer), CCountedPointer<CFont>::operator= 0x0042FE34 / dtor 0x004057A0,
//   CGraphicDataBankInit() 0x00415B80, GFGetSystemManager 0x009A4EC0 (`__fastcall`; display manager at +0x60), the
//   display manager's format pickers 0x009BE830 / 0x009BE870 (compressed non-alpha / alpha), 0x009BE8B0 (compressed
//   boolean alpha), GetBestAlphaTextureFormat 0x009BE6C0, GetBestOpaqueTextureFormat 0x009BE610, GetBestSignedTextureFormat
//   0x009BE590, CPixelFormat::Initialise(_D3DFORMAT) 0x009E3830 (D3DFMT_DXT5), CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, the graphic bank manager's CreateAndInitialize 0x009F83D0 (manager 0x013B837C),
//   CCountedPointer<CGraphicDataBank>::operator= 0x004194BA / dtor 0x00419108.
// Layout: Ego_r.pdb CNewFrontendGameComponent: m_pMenuFont +0x10, m_pFrontEndGraphicBank +0x40; CGraphicDataBankInit (0x2c).
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
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
    bool GetBestCompressedNonAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);       // 0x009BE830
    bool GetBestCompressedAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);          // 0x009BE870
    bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);   // 0x009BE8B0
    bool GetBestAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);                    // 0x009BE6C0
    bool GetBestOpaqueTextureFormat(unsigned long bitDepth, CPixelFormat* out, bool flag);        // 0x009BE610
    bool GetBestSignedTextureFormat(unsigned long bitDepth, CPixelFormat* out);                   // 0x009BE590
};
struct CSystemManager {
    unsigned char    _pad00[0x60];
    CDisplayManager* DisplayManager;               // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

struct CFont;
struct CGraphicDataBank;
template <class T> class CCountedPointer {
public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer<T>& operator=(const CCountedPointer<T>& other);   // 0x0042FE34 (CFont) / 0x004194BA (CGraphicDataBank)
    ~CCountedPointer();                            // 0x004057A0 (CFont) / 0x00419108 (CGraphicDataBank)
};
struct CFontBank {
    CCountedPointer<CFont> GetFont(const CCharString& name);   // 0x009E2C80
};
struct CGraphicDataBankManager {
    CCountedPointer<CGraphicDataBank> CreateAndInitialize(const CCharString& name, const CGraphicDataBankInit& init, bool a, bool b);   // 0x009F83D0
};
extern CFontBank* g_pFontBank;                     // 0x013B838C
extern CGraphicDataBankManager* g_pGraphicDataBankManager;   // 0x013B837C
extern CCharString g_FrontEndMenuFontName;         // 0x013B875C

struct CNewFrontendGameComponent {
    unsigned char                     _pad00[0x10];
    CCountedPointer<CFont>            m_pMenuFont;              // +0x10
    unsigned char                     _pad18[0x40 - 0x18];
    CCountedPointer<CGraphicDataBank> m_pFrontEndGraphicBank;   // +0x40
    bool InitialiseGraphics();
};

bool CNewFrontendGameComponent::InitialiseGraphics()
{
    m_pMenuFont = g_pFontBank->GetFont(g_FrontEndMenuFontName);
    CGraphicDataBankInit init;
    CSystemManager* system;
    system = GFGetSystemManager();
    system->DisplayManager->GetBestCompressedNonAlphaTextureFormat(0x20, &init.NonAlphaFormat);
    system = GFGetSystemManager();
    system->DisplayManager->GetBestCompressedAlphaTextureFormat(0x20, &init.AlphaFormat);
    system = GFGetSystemManager();
    system->DisplayManager->GetBestCompressedBooleanAlphaTextureFormat(0x10, &init.BooleanAlphaFormat);
    system = GFGetSystemManager();
    system->DisplayManager->GetBestAlphaTextureFormat(0x20, &init.UncompressedAlphaFormat);
    system = GFGetSystemManager();
    system->DisplayManager->GetBestOpaqueTextureFormat(0x20, &init.UncompressedNonAlphaFormat, false);
    system = GFGetSystemManager();
    system->DisplayManager->GetBestSignedTextureFormat(0x10, &init.SignedFormat);
    init.InterpolatedAlphaFormat.Initialise(0x35545844);   // D3DFMT_DXT5
    init.GenerateMipmaps = true;
    init.MaxGraphicWidth = 0x800;
    init.MaxGraphicHeight = 0x800;
    init.AllowDither = true;
    {
        CCharString name("GBANK_FRONT_END", -1);
        m_pFrontEndGraphicBank = g_pGraphicDataBankManager->CreateAndInitialize(name, init, true, true);
    }
    return true;
}
