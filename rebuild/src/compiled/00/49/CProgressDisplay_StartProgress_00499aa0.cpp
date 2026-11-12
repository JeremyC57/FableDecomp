// CProgressDisplay::StartProgress(bool fullScreen, bool forRelaunch, bool forFrontEnd, bool blackScreen, bool forFileInstaller, bool renderFlag) -- start the loading / progress display: reset the progress state, record the mode flags, load the textures and the ENG_ARIAL_16 font, and draw the first "Initialising progress display" frame unless the debug display is on
// Leaf: ?StartProgress@CProgressDisplay@@UAEX_N00000@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00499AA0, 418 bytes, `ret 18h`; CProgressDisplay vtable 0x012388B4 slot 1. Ego_r.pdb and
//   FableWin.pdb ?StartProgress@CProgressDisplay@@UAEX_N00000@Z (public virtual; the parameter names are not in the PDBs and are
//   descriptive, each named after the member it is stored into; the last one is RenderProgress's last argument).
// What it does: returns at once when the progress display is disabled (0x01375445), or when it is already initialised and not
//   suppressed (0x013B89D3); while the auto-save progress is shown it sets that display's end time to GFGetTime() + 3.0. Then it
//   zeroes the three time stamps, the loading screen index, the guild seal frame and the stage count, sets TextTagIndex to -1 and
//   clears TextTag / LastTextDisplayed; DebugDisplay is the global debug flag, forced on when the editor is active or the front-end
//   definitions are being compiled. It records the five mode flags, calls InitialiseTextures(forRelaunch, forFrontEnd) (own vtable
//   slot 4), sets word-wrap option 0 to 1, loads the static PFont from the font manager and, when not in debug display, renders
//   the first frame.
// Callees: GFGetTime 0x009E1BC0 (+3.0 double 0x012368F0), CCharString::operator=(const char*) 0x0099EFE0 ("" 0x0122D70E),
//   CMainGameComponent::Get() (static PMainGameComponent 0x013B86A0, inline) -> IsEditorActive (vtable slot 9, 0x004197B0),
//   InitialiseTextures (own vtable slot 4, 0x0049A0B0), WordWrap_SetOption 0x00A0FDB0 (__fastcall (0, 1)), CCharString(const char*,
//   long = -1) 0x0099EBF0 / ~CCharString 0x0099EAE0, GFGetSystemManager 0x009A4EC0 -> GetFontManager() (inline, PFontManager +84h)
//   -> CFontManager::GetFont 0x009E2C80, CCountedPointer<CFontBank>::operator= 0x00419463 on the static PFont 0x013B8998,
//   operator delete 0x00BFE9BC (the temporary's inline release), RenderProgress 0x00499880.
// Shape: both strings are implicit-conversion temporaries (`GetFont("ENG_ARIAL_16")`, `RenderProgress("Initialising ...", ...)`), so
//   they share one frame slot and the font temporary dies before its name. From the shared engine/CCountedPointer.h bodies VC7.1
//   makes retail's own choice here: it CALLS the copy assignment (COMDAT 0x00419463, passing the returned pointer, `push eax`)
//   and expands the temporary's destructor, keeping its `Data = 0 ; Info = 0` stores (0x00499BF8 / 0x00499BFC). The old local
//   model (a bodiless operator= and a named CCharString in a block) lost those two stores. No header opt-in beyond the defines.
// Made-up names (no PDB data symbol found): the six globals below, spelled as in the landed DrawRetailDisplay 0x00498490 /
//   DisplayProgress 0x00499C50 rows.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CFONTBANK
#include "engine/CTexture.h"
#include "engine/CSystemManager.h"
#include "engine/CFontBank.h"
#include "engine/CFontManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CProgressDisplay.h"

double __fastcall GFGetTime();                                           // 0x009E1BC0 ?GFGetTime@@YINXZ
void __fastcall WordWrap_SetOption(unsigned int option, unsigned int value);   // 0x00A0FDB0 ?WordWrap_SetOption@@YIXII@Z
extern bool   g_bProgressDisplayEnabled;                    // 0x01375445
extern bool   g_bProgressDisplaySuppressed;                 // 0x013B89D3
extern bool   g_bAutoSaveProgressShown;                     // 0x013B89C1
extern double g_AutoSaveProgressEndTime;                    // 0x013B89C8
extern bool   g_ProgressDebugDisplay;                       // 0x013B85F0
extern bool   g_bCompileFrontendDefinitions;                // 0x013B8648

void CProgressDisplay::StartProgress(bool fullScreen, bool forRelaunch, bool forFrontEnd, bool blackScreen, bool forFileInstaller, bool renderFlag)
{
    if (!g_bProgressDisplayEnabled)
        return;
    if (!g_bProgressDisplaySuppressed) {
        if (Initialised)
            return;
        if (g_bAutoSaveProgressShown)
            g_AutoSaveProgressEndTime = GFGetTime() + 3.0;
    }
    LastUpdateTime = 0.0;
    LoadingScreenIndex = 0;
    LastScreenUpdate = 0.0;
    TextTagIndex = -1;
    LastProgressUpdate = 0.0;
    TextTag = "";
    LastTextDisplayed = "";
    GuildSealFrameIndex = 0;
    StageCount = 0;
    DebugDisplay = g_ProgressDebugDisplay;
    if ((CMainGameComponent::Get() && CMainGameComponent::Get()->IsEditorActive()) || g_bCompileFrontendDefinitions)
        DebugDisplay = true;
    Initialised = true;
    FullScreen = fullScreen;
    BlackScreen = blackScreen;
    DisplayText = true;
    DisplayResetProgressTipText = false;
    ForRelaunch = forRelaunch;
    ForFrontEnd = forFrontEnd;
    ForFileInstaller = forFileInstaller;
    FileInstallerFinished = false;
    InitialiseTextures(forRelaunch, forFrontEnd);
    WordWrap_SetOption(0, 1);
    PFont = GFGetSystemManager()->GetFontManager()->GetFont("ENG_ARIAL_16");
    if (!DebugDisplay)
        RenderProgress("Initialising progress display", -1.0f, false, renderFlag);
}