// CNewFrontendGameComponent::InitialiseGraphics() -- fetch the front-end menu font, then pick the best texture formats from the display manager and create the "GBANK_FRONT_END" graphic bank (mipmaps, 2048x2048 limit, dither) into m_pFrontEndGraphicBank; returns true
// Leaf: ?InitialiseGraphics@CNewFrontendGameComponent@@QAE_NXZ
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013B875C DEFAULT_FRONT_END_FONT (anonymous namespace; initializer 0x0121A2BF), was g_FrontEndMenuFontName.
enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };   // naming wave 25: the D3D format enum (Initialise takes it)
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
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
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
    bool GetBestCompressedOpaqueTextureFormat(unsigned long bitDepth, CPixelFormat& out);       // 0x009BE830
    bool GetBestCompressedAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& out);          // 0x009BE870
    bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& out);   // 0x009BE8B0
    bool GetBestAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& out);                    // 0x009BE6C0
    bool GetBestOpaqueTextureFormat(unsigned long bitDepth, CPixelFormat& out, bool flag);        // 0x009BE610
    bool GetBestSignedTextureFormat(unsigned long bitDepth, CPixelFormat& out);                   // 0x009BE590
};
class CSystemManager { public:
    unsigned char    _pad00[0x60];
    CDisplayManager* DisplayManager;               // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

// NF221 (family q12226 / q12402): the counted pointers are the shared engine/CCountedPointer.h (was the local CCountedPointer_0042fe34
//   stand-in with a CFont pointee): m_pMenuFont is CCountedPointer<const CFontBank> and m_pFrontEndGraphicBank CCountedPointer<CGraphicDataBank>
//   (Ego_r.pdb CNewFrontendGameComponent members). The menu font comes from CFontManager::GetFont (engine/CFontManager.h; FableWin.pdb /
//   Ego_r.pdb ?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z, the self leaf of 0x009E2C80; was a local
//   CFontBank::GetFont stand-in), assigned through the converting member template 0x0042FE34
//   (??$?4VCFontBank@@@?$CCountedPointer@$$CBVCFontBank@@@@QAEAAV0@ABV?$CCountedPointer@VCFontBank@@@@@Z, FableWin.pdb) and the temporary
//   released by ReleaseData<CFontBank> 0x004057A0; the bank comes from CGraphicBankManager::OpenGraphicBank (FableWin.pdb / Ego_r.pdb, the
//   self leaf of 0x009F83D0; was a local CGraphicDataBankManager::CreateAndInitialize stand-in; no header for the class: local declaration
//   as the landed 0x0041E3F6 / 0x005BFF07), assigned by ??4<CGraphicDataBank> 0x004194BA and released by ReleaseData<CGraphicDataBank> 0x00419108.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY
#include "engine/CCountedPointer.h"
#include "engine/CFontManager.h"
class CFontBank;
class CGraphicDataBank;
class CGraphicBankManager { public:
    CCountedPointer<CGraphicDataBank> OpenGraphicBank(const CCharString& name, const CGraphicDataBankInit& init, bool a, bool b);   // 0x009F83D0
};
extern CFontManager* PFontManager;                     // 0x013B838C
extern CGraphicBankManager* PGraphicBankManager;   // 0x013B837C
namespace { extern CCharString DEFAULT_FRONT_END_FONT; }         // 0x013B875C

class CNewFrontendGameComponent { public:
    unsigned char                     _pad00[0x10];
    CCountedPointer<const CFontBank>  m_pMenuFont;              // +0x10
    unsigned char                     _pad18[0x40 - 0x18];
    CCountedPointer<CGraphicDataBank> m_pFrontEndGraphicBank;   // +0x40
    bool InitialiseGraphics();
};

bool CNewFrontendGameComponent::InitialiseGraphics()
{
    m_pMenuFont = PFontManager->GetFont(DEFAULT_FRONT_END_FONT);
    CGraphicDataBankInit init;
    CSystemManager* system;
    system = GFGetSystemManager();
    system->DisplayManager->GetBestCompressedOpaqueTextureFormat(0x20, init.NonAlphaFormat);
    system = GFGetSystemManager();
    system->DisplayManager->GetBestCompressedAlphaTextureFormat(0x20, init.AlphaFormat);
    system = GFGetSystemManager();
    system->DisplayManager->GetBestCompressedBooleanAlphaTextureFormat(0x10, init.BooleanAlphaFormat);
    system = GFGetSystemManager();
    system->DisplayManager->GetBestAlphaTextureFormat(0x20, init.UncompressedAlphaFormat);
    system = GFGetSystemManager();
    system->DisplayManager->GetBestOpaqueTextureFormat(0x20, init.UncompressedNonAlphaFormat, false);
    system = GFGetSystemManager();
    system->DisplayManager->GetBestSignedTextureFormat(0x10, init.SignedFormat);
    init.InterpolatedAlphaFormat.Initialise(D3DFMT_DXT5);   // D3DFMT_DXT5
    init.GenerateMipmaps = true;
    init.MaxGraphicWidth = 0x800;
    init.MaxGraphicHeight = 0x800;
    init.AllowDither = true;
    {
        CCharString name("GBANK_FRONT_END", -1);
        m_pFrontEndGraphicBank = PGraphicBankManager->OpenGraphicBank(name, init, true, true);
    }
    return true;
}