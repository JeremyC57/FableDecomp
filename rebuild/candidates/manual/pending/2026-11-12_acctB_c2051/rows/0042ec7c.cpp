// CNewFrontendGameComponent::Run(CGameComponent** next_component) -- the front end's main loop: play the three start-up movies, open the front-end banks, start the front end, run input / update / draw (with the 120 s attract-mode movie) until quit, then hand over to the developer front end or the main game
// Leaf: ?Run@CNewFrontendGameComponent@@UAE_NPAPAVCGameComponent@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042EC7C, 2191 bytes, `ret 4` (CGameComponent vtable slot 2; Ego_r.pdb / FableWin.pdb
//   ?Run@CNewFrontendGameComponent@@UAE_NPAPAVCGameComponent@@@Z). Size TU: `#pragma optimize("s", on)` alone gives the biased
//   frame `lea ebp,[esp-74h]` (adding ("y", off) as the small landed siblings do loses it; GOTCHAS chunk 247) and `pop ecx` after
//   operator new.
// Flags evidence (/Gr): the TU's own free functions are __fastcall with no keyword in the source: Ego_r.pdb / FableWin.pdb
//   ?GPlayMusic@@YIXM@Z and ?InstallationComplete@@YI_NXZ (both defined in this TU, FableWin COMDAT order right before Run), the
//   statics ?SetClearColour@CXMVPlayer@@SIXVCRGBColour@@@Z / ?SetSkipOnAnyButton@CXMVPlayer@@SIX_N@Z, and every free callee
//   (GFGetTime YINXZ, GFPlayFullScreenXBoxMovie, the NResourceDirectoryNames getters YI?AVCWideString@@XZ); same flag as the matched
//   chunk-2051 row CWorld::Init 0x004A6E30.
// Inline-budget: CRGBColour's copy constructor (FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_COPY, the default body forced). Measured with
//   the c2 inline trace (c2.dll 0x1078D1AA) on this file with the plain body: decision 3 of 32 in Run, ??0CRGBColour@@QAE@ABV0@@Z
//   size 31, savings 5 -> cost 26, over the size-TU plain-inline limit 15, refused with no score (the four-byte constructor
//   ??0CRGBColour@@QAE@EEEE@Z, size 32 savings 26 -> 6, is expanded). Plain body: DIFFER(2179v2191) 568/624, the by-value
//   SetClearColour(movie.ClearColour) argument copied by a call; retail copies the four bytes in place (0x0042ED68..0x0042ED7C).
// Shape: CMovieInfo movies[3] (Ego_r.pdb aggregate) played in a counted loop guarded by the two start-up flags; the clear colour
//   goes through the inline static CXMVPlayer::SetClearColour (by value: the byte-wise CRGBColour copy, 0x0042ED68); the movie error
//   string lands in the front-end manager when a movie fails. The main loop is `while (Update() != 2 && !Quit)`.
// Allocation: the menu-font glyph string is a NAMED local in its own block (`{ CWideString glyphs(L"..."); m_pMenuFont->Preload(glyphs,
//   true); }`). With the unnamed temporary (`Preload(CWideString(L"..."), true)`) VC7.1 loads m_pMenuFont's vtable before the string
//   constructor into a callee-saved one, which outranks &NBankFileManager::GBankFileManager for edi: the bank manager address
//   is then hoisted into the bank-if test block and re-materialised in the leave block (DIFFER 2179v2191, 587/624). Named, the
//   address lives in edi from the first OpenRetailBank (0x0042EE7D) through the main loop to the leave-frontend banks, with the
//   skip-path load 0x0042EF2E and the reload after the attract movie 0x0042F20A, exactly as retail.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_COPY
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_FORCEMUSICPLAY
#define FABLE_RETAIL_CFRONTENDMANAGER
#define FABLE_RETAIL_CFRONTENDMANAGER_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CTBASESINGLETON_RELEASE
#define FABLE_RETAIL_CFONTMANAGER_ASYNC
#define FABLE_RETAIL_CASOUNDMANAGER
#define FABLE_RETAIL_CINPUTMANAGER_CLEAREVENTS
#define FABLE_RETAIL_CBANKFILEMANAGER
#define FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS
#define FABLE_RETAIL_CBANKFILEMANAGER_CLOSERETAILBANK
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_RESETDIMENSIONS
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#include "fable_string.h"
#include "fable_profile.h"
#include "fable_retail_banks.h"
#include "engine/CRGBColour.h"
#include "engine/CCountedPointer.h"
#include "engine/CSystemManager.h"
#include "engine/CRenderManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CFontBank.h"
#include "engine/CFontManager.h"
#include "engine/CInputManager.h"
#include "engine/CASoundManager.h"
#include "engine/CFrontEndManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/CTBaseSingleton.h"
#include "engine/CFrontEndDef.h"
#include "engine/CMovieInfo.h"
#include "engine/CXMVPlayer.h"
#include "engine/CXMVPlayerBuffers.h"
#include "engine/CEngineInterpolationInfo.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CMainGameComponent.h"
#include "engine/CFrontendGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"
#include <stdlib.h>
#pragma optimize("s", on)

class CIEngine;
class CFileInstaller;
namespace NGameText { class CDataBank; }
namespace NResourceDirectoryNames {
CWideString GetGraphicBankDir();                                         // 0x0041A100 ?GetGraphicBankDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
CWideString GetShaderBankDir();                                          // 0x0041A1E0
CWideString GetHDDialogueDir();                                          // 0x0041A330
CWideString GetHDMiscDir();                                              // 0x0041A0C0
}
namespace NLocalisation { unsigned long GetLanguage(); }                 // 0x00415070 ?GetLanguage@NLocalisation@@YIKXZ
enum { LANGUAGE_CHINESE = 4, LANGUAGE_GERMAN = 7, LANGUAGE_SPANISH = 10, LANGUAGE_FRENCH = 12, LANGUAGE_ITALIAN = 16,
       LANGUAGE_JAPANESE = 17, LANGUAGE_KOREAN = 18 };                   // the landed NLocalisation rows' values
double GFGetTime();                                                      // 0x009E1BC0 ?GFGetTime@@YINXZ
void GPlayMusic(float offset);                                           // 0x0042DED5 ?GPlayMusic@@YIXM@Z
bool InstallationComplete();                                             // 0x0042E3CE ?InstallationComplete@@YI_NXZ
bool GFPlayFullScreenXBoxMovie(const CCharString& file_name, unsigned long flags, unsigned long width, unsigned long height,
                               unsigned long buffer_size, const NGameText::CDataBank* text_bank, CIEngine* engine,
                               bool display_subtitles, float no_skip_duration, bool (__fastcall* skip_callback)(),
                               CXMVPlayerBuffers* buffers, CWideString* error_string);   // 0x006286F0
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);         // KERNEL32 (IAT 0x0143FE1C)
extern bool g_bFrontEndFlag_01375448;                                    // 0x01375448
extern bool g_bFrontEndFlag_137544A;                                     // 0x0137544A
extern bool g_bRuntimeLevelFiles;                                        // 0x013B8616
extern CRenderManager* PRenderManager;                                   // 0x013B8384
extern CInputManager* PInputManager;                                     // 0x013B8388
extern CGameComponent* g_pRetiredGameComponent;                          // 0x013B7D58

bool CNewFrontendGameComponent::Run(CGameComponent** next_component)
{
    m_fLastUpdateTime = (float)GFGetTime();
    m_fLastRenderTime = (float)GFGetTime();

    CMovieInfo movies[3] = {
        { "Data\\Video\\lionhead_logo.xmv", 640, 400, CRGBColour(255, 255, 255, 255), 0, 0x1a0, 20.0f, InstallationComplete },
        { "Data\\Video\\Microsoft_Logo.xmv", 640, 480, CRGBColour(0, 0, 0, 255), 0, 0x1a0, 20.0f, InstallationComplete },
        { "Data\\Video\\intro_comp.xmv", 640, 360, CRGBColour(0, 0, 0, 0), 0, 0x1a0, 10.0f, InstallationComplete },
    };
    if (g_bFrontEndFlag_01375448 && g_bFrontEndFlag_137544A) {
        for (unsigned long i = 0; i < 3; ++i) {
            const CMovieInfo& movie = movies[i];
            CXMVPlayer::SetClearColour(movie.ClearColour);
            CXMVPlayer::SetSkipOnAnyButton(true);
            CWideString error_string;
            bool played = GFPlayFullScreenXBoxMovie(movie.FilePath, movie.Flags, movie.Width, movie.Height, 0xa00000,
                                                    m_pTextBank.Get(), m_pEngine.Get(), true, movie.NoSkipDuration,
                                                    movie.SkipCallback, 0, &error_string);
            if (!played) {
                WindowsMediaPlayerInstalled = false;
                NUISystem::CFrontEndManager::GetInstance()->SetMediaPlayerErrorString(error_string);
            }
            CXMVPlayer::SetSkipOnAnyButton(false);
            CXMVPlayer::SetClearColour(CRGBColour(0, 0, 0, 255));
            Sleep(movie.DelayMS);
            if (GFGetSystemManager()->Update() == 2)
                return false;
        }
    }

    if (g_bRuntimeLevelFiles) {
        CWideString platform(L"pc\\");
        NBankFileManager::GBankFileManager.OpenRetailBank(NResourceDirectoryNames::GetGraphicBankDir() + platform + L"frontend.big", true);
        NBankFileManager::GBankFileManager.OpenRetailBank(NResourceDirectoryNames::GetHDDialogueDir() + L"text.big", false);
        NBankFileManager::GBankFileManager.OpenRetailBank(NResourceDirectoryNames::GetShaderBankDir() + platform + L"shaders.big", false);
    }

    Running = true;
    Init2();
    NProfileTimer::StartProfile("Init Engine", 0);
    InitialiseEngine();
    NProfileTimer::EndProfile(0);

    NProfileTimer::StartProfile("Init frontend", 0);
    CXMVPlayerBuffers* xmv_buffers = new CXMVPlayerBuffers;
    PFontManager->SetLoadFontGlyphsAsynchronously(false);
    {
        CWideString glyphs(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz()");
        m_pMenuFont->Preload(glyphs, true);
    }
    CDisplayManager* display = GFGetSystemManager()->GetDisplayManager();
    PRenderManager->ClearRenderTarget(CRGBColour(0, 0, 0, 255), 0);
    display->SwapScreens();
    GPlayMusic(0.0f);
    m_pFrontEndManager->ChangeStateFirstTime();
    m_attractModeTimer.StartZero();
    m_attractModeTimer.Reset();
    NProfileTimer::EndProfile(0);
    NUISystem::CUIScreenUnitConverter::ResetDimensions();

    while (GFGetSystemManager()->Update() != 2 && !Quit) {
        float music_offset;
        if (GFGetSystemManager()->GetForceMusicPlay(music_offset)) {
            GFGetSystemManager()->ResetForceMusicPlay();
            GPlayMusic(music_offset);
        }
        if (m_inputDelayTimer.IsRunning() && m_inputDelayTimer.GetElapsedSeconds() >= 0.2f)
            m_inputDelayTimer.Stop();
        Input();
        Update();
        CEngineInterpolationInfo info;
        UpdateInterpolation(info);
        Draw(info);
        if (m_attractModeTimer.GetElapsedSeconds() >= 120.0f && m_pFrontEndManager->CanStartAttractMode()) {
            if (PSoundManager)
                PSoundManager->StopMusic(0);
            long index = rand() % (long)m_pFrontEndDef->vAttractModeMovie.size();
            CCharString movie = m_pFrontEndDef->vAttractModeMovie[index];
            switch (NLocalisation::GetLanguage()) {
            case LANGUAGE_CHINESE:  movie += "English"; break;
            case LANGUAGE_GERMAN:   movie += "German"; break;
            case LANGUAGE_SPANISH:  movie += "Spanish"; break;
            case LANGUAGE_FRENCH:   movie += "French"; break;
            case LANGUAGE_ITALIAN:  movie += "Italian"; break;
            case LANGUAGE_JAPANESE: movie += "English"; break;
            case LANGUAGE_KOREAN:   movie += "English"; break;
            default:                movie += "English"; break;
            }
            movie += ".wmv";
            CXMVPlayer::SetSkipOnAnyButton(true);
            GFPlayFullScreenXBoxMovie(movie, 0x221, 0, 0, 0x100000, m_pTextBank.Get(), m_pEngine.Get(), true, -1.0f, 0, xmv_buffers, 0);
            CXMVPlayer::SetSkipOnAnyButton(false);
            m_attractModeTimer.Reset();
            PInputManager->ClearEvents();
            m_inputDelayTimer.StartZero();
            GPlayMusic(0.0f);
        }
    }

    Quit = true;
    Running = false;
    delete xmv_buffers;
    ReleaseXMVCode();
    m_pNewFrontendGameComponent = 0;
    CleanUpInterfaces();
    bool result;
    if (CreateDevFrontEnd) {
        Shutdown();
        CGameComponent* component = new CFrontendGameComponent(Game, CFrontendGameComponentInit());
        component->Init();
        *next_component = component;
        m_pNewFrontendGameComponent = 0;
        result = true;
    } else if (m_bCreateGame) {
        g_bFrontEndFlag_01375448 = false;
        NProfileTimer::StartProfile("Leave frontend", 0);
        if (PSoundManager)
            PSoundManager->StopMusic(500);
        if (g_bRuntimeLevelFiles) {
            CWideString platform(L"pc\\");
            NBankFileManager::GBankFileManager.CloseRetailBank(NResourceDirectoryNames::GetGraphicBankDir() + platform + L"frontend.big");
            NBankFileManager::GBankFileManager.OpenRetailBank(NResourceDirectoryNames::GetGraphicBankDir() + L"graphics.big", true);
            NBankFileManager::GBankFileManager.OpenRetailBank(NResourceDirectoryNames::GetGraphicBankDir() + platform + L"textures.big", true);
            NBankFileManager::GBankFileManager.OpenRetailBank(NResourceDirectoryNames::GetHDDialogueDir() + L"dialogue.big", false);
            NBankFileManager::GBankFileManager.OpenRetailBank(NResourceDirectoryNames::GetHDMiscDir() + platform + L"effects.big", false);
        }
        CTBaseSingleton<CFileInstaller>::Release();
        CMainGameComponentInit init;
        init.SaveGameName = SaveGame;
        init.InitialWorldName = CWideString::CreateFromCharString("FinalAlbion.wld");
        NProfileTimer::EndProfile(0);
        CGame& game = Game;
        Shutdown();
        NProfileTimer::StartProfile("Init Game", 1);
        CGameComponent* component = new CMainGameComponent(game, init);
        component->Init();
        *next_component = component;
        g_pRetiredGameComponent = this;
        return true;
    } else {
        m_pNewFrontendGameComponent = 0;
        Shutdown();
        result = false;
    }
    g_pRetiredGameComponent = this;
    return result;
}
