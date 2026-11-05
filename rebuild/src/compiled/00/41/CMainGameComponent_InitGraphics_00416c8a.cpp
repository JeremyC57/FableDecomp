// CMainGameComponent::InitGraphics() -- pick the texture formats (compressed when the display supports it and the option is on), create the main graphic bank "GBANK_MAIN" into PMainGraphicBank and make it the graphic appearances' bank, with three loading-progress ticks; returns true
// Leaf: ?InitGraphics@CMainGameComponent@@QAE_NXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
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
enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };   // naming wave 25: the D3D format enum (Initialise takes it)
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString&, float, bool, bool); }   // 0x009E9F40   // 0x009E9F40
class CPixelFormat { public:
    _D3DFORMAT Format;
    void Initialise(_D3DFORMAT d3dFormat);               // 0x009E3830
};
class CGraphicDataBankInit { public:                      // 0x2c bytes
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
class CDisplayManager { public:
    bool IsSupportingCompressedTextures();                                                       // 0x009BE800
    bool GetBestCompressedNonAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);       // 0x009BE830
    bool GetBestCompressedAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);          // 0x009BE870
    bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);   // 0x009BE8B0
    bool GetBestOpaqueTextureFormat(unsigned long bitDepth, CPixelFormat* out, bool flag);        // 0x009BE610
    bool GetBestAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);                    // 0x009BE6C0
    bool GetBestBooleanAlphaTextureFormat(unsigned long bitDepth, CPixelFormat* out);             // 0x009BE750
    bool GetBestSignedTextureFormat(unsigned long bitDepth, CPixelFormat* out);                   // 0x009BE590
};
class CGraphicDataBank;
#include "engine/CCountedPointer.h"
struct CGraphicDataBankManager {
    CCountedPointer<CGraphicDataBank> CreateAndInitialize(const CCharString& name, const CGraphicDataBankInit& init, bool a, bool b);   // 0x009F83D0
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    static void __fastcall SetGraphicBank(CGraphicDataBank& bank);   // 0x004BBFC0
};
}
extern CDisplayManager* GPDisplayManager;         // 0x013B8390
class CGraphicBankManager;
extern CGraphicBankManager* PGraphicBankManager;   // 0x013B837C
extern unsigned long g_TextureBitDepth;            // 0x013B7D68
extern unsigned long g_MaxGraphicWidth;            // 0x013B7D64
extern unsigned long g_MaxGraphicHeight;           // 0x013B7D60
extern bool g_AllowCompressedTextures;             // 0x01375453
extern bool g_AllowDither;                         // 0x01375452

class CMainGameComponent { public:
    unsigned char                     _pad00[0x161c8];
    CCountedPointer<CGraphicDataBank> PMainGraphicBank;   // +0x161C8
    bool InitGraphics();
};

bool CMainGameComponent::InitGraphics()
{
    {
        CCharString text("Init Graphics", -1);
        NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
    }
    CGraphicDataBankInit init;
    if (GPDisplayManager->IsSupportingCompressedTextures() && g_AllowCompressedTextures) {
        GPDisplayManager->GetBestCompressedNonAlphaTextureFormat(g_TextureBitDepth, &init.NonAlphaFormat);
        GPDisplayManager->GetBestCompressedAlphaTextureFormat(g_TextureBitDepth, &init.AlphaFormat);
        GPDisplayManager->GetBestCompressedBooleanAlphaTextureFormat(g_TextureBitDepth, &init.BooleanAlphaFormat);
        init.InterpolatedAlphaFormat.Initialise(D3DFMT_DXT5);   // D3DFMT_DXT5
    } else {
        GPDisplayManager->GetBestOpaqueTextureFormat(g_TextureBitDepth, &init.NonAlphaFormat, false);
        GPDisplayManager->GetBestAlphaTextureFormat(g_TextureBitDepth, &init.AlphaFormat);
        GPDisplayManager->GetBestBooleanAlphaTextureFormat(g_TextureBitDepth, &init.BooleanAlphaFormat);
    }
    GPDisplayManager->GetBestAlphaTextureFormat(g_TextureBitDepth, &init.UncompressedAlphaFormat);
    GPDisplayManager->GetBestOpaqueTextureFormat(g_TextureBitDepth, &init.UncompressedNonAlphaFormat, false);
    GPDisplayManager->GetBestSignedTextureFormat(g_TextureBitDepth, &init.SignedFormat);
    init.MaxGraphicWidth = g_MaxGraphicWidth;
    init.MaxGraphicHeight = g_MaxGraphicHeight;
    init.GenerateMipmaps = true;
    init.AllowDither = g_AllowDither;
    {
        CCharString text("Opening Main Graphic Bank", -1);
        NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
    }
    {
        CCharString name("GBANK_MAIN", -1);
        PMainGraphicBank = ((CGraphicDataBankManager*)PGraphicBankManager)->CreateAndInitialize(name, init, true, true);
    }
    {
        CCharString text("Setting Main Graphic Bank", -1);
        NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
    }
    NGraphicAppearance::CTCGraphicAppearance::SetGraphicBank(*PMainGraphicBank);
    return true;
}