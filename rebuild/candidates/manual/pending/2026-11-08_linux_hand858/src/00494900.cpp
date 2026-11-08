// CFrontendGameComponent::Output() -- draw one frame of the debug front end: keep the font manager from loading glyphs asynchronously, load the splash.tga backdrop into SplashScreen the first time, draw it full screen, then in the television-safe window list the start menu (mode 1), the world (10), save (11) or profile (12) list with the highlighted line in yellow, and flush, end the render and swap screens
// Leaf: ?Output@CFrontendGameComponent@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00494900, 3336 bytes incl. the 5 case pointers + 12-byte mode index after the `ret`
//   (0x004955E8 / 0x004955FC: mode 1 / 10 / 11 / 12, every other mode to the flush), plain `ret`, aligned frame.
// Identity (two sources): Ego_r.pdb ?Output@CFrontendGameComponent@@AAEXXZ (private, void()), and the landed
//   CFrontendGameComponent::Run 0x00496CF0 calls Input 0x004963A0 then 0x00494900 once per frame.
// Callees (Ego_r.pdb / FableWin.pdb spellings): GFGetSystemManager 0x009A4EC0 (the font manager at retail +0x84, as the landed
//   ~CText 0x0054F6E0 reads it) and FableWin's inline CFontManager Get/SetLoadFontGlyphsAsynchronously (+0x38, the landed
//   CFontManager ctor 0x009E2E30); CDisplayManager GetVVisibleTelevisionScreenArea 0x009BECE0 / GetBestCompressedOpaqueTextureFormat
//   0x009BE830 / inline GetBackBuffer (+0x2C) / BeginRender 0x009BEF20 / EndRender 0x009BEF50 / SwapScreens 0x009BEEB0;
//   CSurface::GetDimensions 0x009F2F90 / LoadFromBitmapFileToPreallocatedSurface 0x009F2DD0 / Uninitialise 0x009F2E20 (inline
//   virtual dtor, vtable 0x0122F84C); CTexture::GetNextValidDimensions 0x009FA620 / Initialise 0x009FA280 / GetSurfaceLevel
//   0x009F9E00; NResourceDirectoryNames::GetMiscDir 0x0041A0A0; CRenderManagerCore::SetVWindow 0x00A0B560; CRenderManager2D
//   Draw2DTexture(box) 0x009DC6E0 / Draw2DText 0x009DD8F0 / Render2DDrawList 0x009DA9F0; CFontBank slot 2 GetFontMaxHeight with
//   GFAYToVY 0x009E1DD0 (inline GetVFontMaxHeight); CCharString::ToWideString 0x0099E480; GFIntToWideString 0x00405F00; the
//   CWideString operator+ trio 0x0099BE70 / 0x0099BF30 / 0x0099BFF0, CWideString(const wchar_t*) 0x0099B6B0, ~CWideString 0x0099B510.
// Globals: GPDisplayManager 0x013B8390, PRenderManager 0x013B8384, VIRTUAL_SCREEN_WIDTH / VIRTUAL_SCREEN_HEIGHT 0x013961E8 /
//   0x013961F0, the version string 0x013B85E0 (landed spelling FableFullBuildVersion; probably Ego_r GVersionString, see NOTES),
//   g_WorldToLoad 0x013B8668 (landed GFParseCommandLine spelling).
// Layout: Ego_r.pdb CFrontendGameComponent with the retail offsets of the landed siblings (MainFont +0x1E04, ProfileNames +0x1E0C,
//   SaveNames +0x1E1C, WorldNames +0x1E30, CursorHighlight +0x1E44, SplashScreen +0x1E58); no engine/ header exists.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DTEXTUREBOX
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_FRONTEND
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CTEXTURE_NEXTVALIDDIMENSIONS
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DExtentsI.h"
#include "engine/CRGBColour.h"
#include "engine/CCountedPointer.h"
#include "engine/CFontBank.h"
#include "engine/CPixelFormat.h"
#include "engine/CSurface.h"
#include "engine/CTexture.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CGameComponent.h"
#include "engine/CSystemManager.h"   // retail branch: PFontManager at +0x84, inline GetFontManager; GFGetSystemManager 0x009A4EC0

class CFontManager {                     // no shared header: the FableWin inline Get/SetLoadFontGlyphsAsynchronously only
public:
    unsigned char _pad00[0x38];
    bool          LoadFontGlyphsAsynchronously;                          // +0x38
    bool GetLoadFontGlyphsAsynchronously() const { return LoadFontGlyphsAsynchronously; }
    void SetLoadFontGlyphsAsynchronously(bool load) { LoadFontGlyphsAsynchronously = load; }
};
namespace NResourceDirectoryNames { CWideString __fastcall GetMiscDir(); }   // 0x0041A0A0
CWideString __fastcall GFIntToWideString(long value);                    // 0x00405F00
extern CDisplayManager* GPDisplayManager;                                // 0x013B8390
extern CRenderManager* PRenderManager;                                   // 0x013B8384
extern double VIRTUAL_SCREEN_WIDTH;                                      // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                                     // 0x013961F0
extern CCharString FableFullBuildVersion;                                // 0x013B85E0
extern CWideString g_WorldToLoad;                                        // 0x013B8668

namespace NGameComponentHelpers { enum ECursorMovement { }; }
enum EGameMode { };

class CFrontendGameComponent : public CGameComponent {
public:
    virtual ~CFrontendGameComponent();
private:
    void Output();

    bool          CreateGame;                                            // +0x10
    long          CreateGameJoystickDeviceNumber;                        // +0x14
    bool          CreateEditor;                                          // +0x18
    bool          CreateReleaseFrontEnd;                                 // +0x19
    EGameMode     Mode;                                                  // +0x1c
    long          NoConnections;                                         // +0x20
    long          NoSessions;                                            // +0x24
    unsigned char Connections[64];                                       // +0x28 CNetworkConnection[64]
    unsigned char Sessions[256];                                         // +0x68 CNetworkSession[256]
    char          String[128];                                           // +0x168
    long          StringPos;                                             // +0x1e8
    void*         CurrentConnection;                                     // +0x1ec CNetworkConnection*
    unsigned char TempBuffer[2048];                                      // +0x1f0
    char          SessionString[20][256];                                // +0x9f0
    long          FirstSessionString;                                    // +0x1df0
    long          LastSessionString;                                     // +0x1df4
    long          NoSessionStrings;                                      // +0x1df8
    unsigned char PTempGraphics[8];                                      // +0x1dfc CCountedPointer<CGraphicDataBank>
    CCountedPointer<const CFontBank> MainFont;                           // +0x1e04
    std::vector<CWideString> ProfileNames;                               // +0x1e0c
    long          ProfileNamesOffset;                                    // +0x1e18
    std::vector<CWideString> SaveNames;                                  // +0x1e1c
    long          SaveNamesOffset;                                       // +0x1e28
    CWideString   SaveToLoad;                                            // +0x1e2c
    std::vector<CWideString> WorldNames;                                 // +0x1e30
    long          WorldNamesOffset;                                      // +0x1e3c
    CWideString   WorldToLoad;                                           // +0x1e40
    long          CursorHighlight;                                       // +0x1e44
    double        TimeLastMovementHappened;                              // +0x1e48
    bool          StartedRepeatingMovement;                              // +0x1e50
    NGameComponentHelpers::ECursorMovement CurrentMovement;              // +0x1e54
    CTexture      SplashScreen;                                          // +0x1e58
};

void CFrontendGameComponent::Output()
{
    CFontManager* font_manager = GFGetSystemManager()->GetFontManager();
    bool load_asynchronously = font_manager->GetLoadFontGlyphsAsynchronously();
    font_manager->SetLoadFontGlyphsAsynchronously(false);
    C2DBoxF tv_window = GPDisplayManager->GetVVisibleTelevisionScreenArea();
    C2DBoxF full_window(0.0f, 0.0f, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
    CRGBColour normal_colour(255, 255, 255, 255);
    CRGBColour highlight_colour(255, 255, 0, 255);
    CRGBColour colour;
    if (!SplashScreen.IsValid()) {
        CPixelFormat format;
        GPDisplayManager->GetBestCompressedOpaqueTextureFormat(16, format);
        SplashScreen.Initialise(CTexture::GetNextValidDimensions(C2DExtentsI(GPDisplayManager->GetBackBuffer().GetWidth(), GPDisplayManager->GetBackBuffer().GetHeight()), format),
                                1, format, 0, SURFACE_POOL_MANAGED, false, false);
        SplashScreen.GetSurfaceLevel(0).LoadFromBitmapFileToPreallocatedSurface(NResourceDirectoryNames::GetMiscDir() + L"splash.tga", 2);
    }
    GPDisplayManager->BeginRender();
    PRenderManager->SetVWindow(full_window);
    PRenderManager->Draw2DTexture(C2DBoxF(0.0f, 0.0f, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT), CRGBColour(255, 255, 255, 255), SplashScreen, 0);
    PRenderManager->SetVWindow(tv_window);
    float y;
    float line_height;   // the font height as a named temporary: only then does retail's `mov eax,[esi+1E44h]` (CursorHighlight)
                         // schedule ahead of the `fadd` that steps y (every `y += MainFont->GetVFontMaxHeight()` spelling keeps it after)
    switch (Mode) {
    case 1:
        PRenderManager->Draw2DText(C2DVector(0.0f, 0.0f), L"Fable " + FableFullBuildVersion.ToWideString(), *MainFont, normal_colour, 0);
        y = GFAYToVY(32.0f);
        colour = CursorHighlight == 1 ? highlight_colour : normal_colour;
        PRenderManager->Draw2DText(C2DVector(0.0f, y), L"1 - Single Player [" + g_WorldToLoad + L"]", *MainFont, colour, 0);
        line_height = MainFont->GetVFontMaxHeight();
        y += line_height;
        colour = CursorHighlight == 2 ? highlight_colour : normal_colour;
        PRenderManager->Draw2DText(C2DVector(0.0f, y), L"2 - Select World", *MainFont, colour, 0);
        line_height = MainFont->GetVFontMaxHeight();
        y += line_height;
        colour = CursorHighlight == 3 ? highlight_colour : normal_colour;
        PRenderManager->Draw2DText(C2DVector(0.0f, y), L"3 - Select Save", *MainFont, colour, 0);
        line_height = MainFont->GetVFontMaxHeight();
        y += line_height;
        colour = CursorHighlight == 6 ? highlight_colour : normal_colour;
        PRenderManager->Draw2DText(C2DVector(0.0f, y), L"6 - Quit", *MainFont, colour, 0);
        line_height = MainFont->GetVFontMaxHeight();
        y += line_height;
        colour = CursorHighlight == 7 ? highlight_colour : normal_colour;
        PRenderManager->Draw2DText(C2DVector(0.0f, y), L"7 - Switch to Release Front End", *MainFont, colour, 0);
        line_height = MainFont->GetVFontMaxHeight();
        y += line_height;
        break;
    case 10: {
        PRenderManager->Draw2DText(C2DVector(0.0f, 0.0f), L"Select World to Load", *MainFont, normal_colour, 0);
        y = GFAYToVY(32.0f);
        for (long i = 0; i < 8 && WorldNamesOffset + i < WorldNames.size(); ++i) {
            colour = CursorHighlight == i + 1 ? highlight_colour : normal_colour;
            PRenderManager->Draw2DText(C2DVector(0.0f, y), GFIntToWideString(i + 1) + L" - " + WorldNames[WorldNamesOffset + i], *MainFont, colour, 0);
            line_height = MainFont->GetVFontMaxHeight();
            y += line_height;
        }
        line_height = MainFont->GetVFontMaxHeight();
        y += line_height;
        colour = CursorHighlight == 9 ? highlight_colour : normal_colour;
        PRenderManager->Draw2DText(C2DVector(0.0f, y), L"9 - Quit", *MainFont, colour, 0);
        line_height = MainFont->GetVFontMaxHeight();
        y += line_height;
        if (WorldNames.size() > 8) {
            colour = CursorHighlight == 0 ? highlight_colour : normal_colour;
            PRenderManager->Draw2DText(C2DVector(0.0f, y), L"0 - See more worlds", *MainFont, colour, 0);
            line_height = MainFont->GetVFontMaxHeight();
            y += line_height;
        }
        break;
    }
    case 11: {
        PRenderManager->Draw2DText(C2DVector(0.0f, 0.0f), L"Select Save to Load", *MainFont, normal_colour, 0);
        y = GFAYToVY(32.0f);
        for (long i = 0; i < 8 && SaveNamesOffset + i < SaveNames.size(); ++i) {
            colour = CursorHighlight == i + 1 ? highlight_colour : normal_colour;
            PRenderManager->Draw2DText(C2DVector(0.0f, y), GFIntToWideString(i + 1) + L" - " + SaveNames[SaveNamesOffset + i], *MainFont, colour, 0);
            line_height = MainFont->GetVFontMaxHeight();
            y += line_height;
        }
        line_height = MainFont->GetVFontMaxHeight();
        y += line_height;
        colour = CursorHighlight == 9 ? highlight_colour : normal_colour;
        PRenderManager->Draw2DText(C2DVector(0.0f, y), L"9 - Quit", *MainFont, colour, 0);
        line_height = MainFont->GetVFontMaxHeight();
        y += line_height;
        if (SaveNames.size() > 8) {
            colour = CursorHighlight == 0 ? highlight_colour : normal_colour;
            PRenderManager->Draw2DText(C2DVector(0.0f, y), L"0 - See more saves", *MainFont, colour, 0);
            line_height = MainFont->GetVFontMaxHeight();
            y += line_height;
        }
        break;
    }
    case 12: {
        PRenderManager->Draw2DText(C2DVector(0.0f, 0.0f), L"Select Profile", *MainFont, normal_colour, 0);
        y = GFAYToVY(32.0f);
        for (long i = 0; i < 8 && ProfileNamesOffset + i < ProfileNames.size(); ++i) {
            colour = CursorHighlight == i + 1 ? highlight_colour : normal_colour;
            PRenderManager->Draw2DText(C2DVector(0.0f, y), GFIntToWideString(i + 1) + L" - " + ProfileNames[ProfileNamesOffset + i], *MainFont, colour, 0);
            line_height = MainFont->GetVFontMaxHeight();
            y += line_height;
        }
        line_height = MainFont->GetVFontMaxHeight();
        y += line_height;
        colour = CursorHighlight == 9 ? highlight_colour : normal_colour;
        PRenderManager->Draw2DText(C2DVector(0.0f, y), L"9 - Quit", *MainFont, colour, 0);
        line_height = MainFont->GetVFontMaxHeight();
        y += line_height;
        if (ProfileNames.size() > 8) {
            colour = CursorHighlight == 0 ? highlight_colour : normal_colour;
            PRenderManager->Draw2DText(C2DVector(0.0f, y), L"0 - See more profiles", *MainFont, colour, 0);
            line_height = MainFont->GetVFontMaxHeight();
            y += line_height;
        }
        if (ProfileNamesOffset == 0) {
            line_height = MainFont->GetVFontMaxHeight();
            y += line_height;
            colour = CursorHighlight == 10 ? highlight_colour : normal_colour;
            PRenderManager->Draw2DText(C2DVector(0.0f, y), L"A - [Debug Profile]", *MainFont, colour, 0);
            line_height = MainFont->GetVFontMaxHeight();
            y += line_height;
            colour = CursorHighlight == 11 ? highlight_colour : normal_colour;
            PRenderManager->Draw2DText(C2DVector(0.0f, y), L"B - Create New Profile", *MainFont, colour, 0);
            line_height = MainFont->GetVFontMaxHeight();
            y += line_height;
        }
        break;
    }
    }
    PRenderManager->Render2DDrawList(true);
    GPDisplayManager->EndRender();
    GPDisplayManager->SwapScreens();
    PRenderManager->SetVWindow(full_window);
    font_manager->SetLoadFontGlyphsAsynchronously(load_asynchronously);
}
