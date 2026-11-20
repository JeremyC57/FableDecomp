// CProgressDisplay::StopProgress(bool showStoppingText, bool clearScreen) -- end the loading/progress display: optionally clear the screen to black (or render a final "Stopping progress display" frame), then release every progress, guild-seal and loading-screen texture and the player GUI's graphics
// Leaf: ?StopProgress@CProgressDisplay@@UAEX_N0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00499D90, length 530 bytes (padded to 544), `ret 8` (vtable slot 2).
// Callees: GFGetSystemManager 0x009A4EC0; CDisplayManager BeginRender 0x009BEF20 / EndRender 0x009BEF50 / SwapScreens
// 0x009BEEB0; the render manager's Draw2DBox 0x009DE3F0 / Render2DDrawList 0x009DA9F0; CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0; RenderProgress 0x00499880; CTexture::Uninitialise 0x009F9F70;
// vector<CTexture>::erase(first, last) 0x0049B3B0; the player GUI's graphics release 0x00437570 ((&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790);
// globals: progress-enabled bool 0x01375445, suppress bool 0x013B89D3, the two black-screen bools 0x013B89C0 / 0x013B89C1;
// screen size doubles 0x013961E8 / 0x013961F0.
// Layout: Ego_r.pdb CProgressDisplay with 12-byte retail vectors (ProgressTextures +0x04, GuildSealTextures +0x10,
// LoadingScreenTextures +0x1c) and the flags 0x10 lower (DebugDisplay +0x78, Initialised +0x79, BlackScreen +0x7b), as the
// landed RenderProgress 0x00499880.
// Naming (NF142, 2026-11-20, q4292 / q6168): BeginRender 0x009BEF20 respelled in place in the row's existing local CDisplayManager model to the
//   Ego_r.pdb 0x00B23C30 + FableWin.pdb 0x0301EBA0 spelling ?BeginRender@CDisplayManager@@QAE_NXZ (bool; the result is unused here).
// Shape: a named C2DBoxF local (a temporary schedules its zero X0/Y0 stores early); the black CRGBColour temporary reuses the dead `clearScreen` argument slot; each texture loop recomputes the size.
// Naming (NF160, 2026-11-20, q4293 / q6169): the local CCharString / C2DBoxF / CRGBColour / CRenderManager / CDisplayManager /
//   CSystemManager / CTexture / TextureVector / CTBaseSingletonNoDefaultAlloc / CPlayerGui / CProgressDisplay models replaced by the
//   shared headers (the recipe of the landed siblings StartProgress 0x00499AA0 / DrawRetailDisplay 0x00498490), so the render-list call
//   binds Ego_r.pdb 0x00B2E390 / FableWin.pdb ?Render2DDrawList@CRenderManager2D@@QAEX_N@Z (was the stand-in CRenderManager member);
//   with it Draw2DBox 0x009DE3F0 (?Draw2DBox@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCRGBColour@@K@Z), the CPlayerGui call 0x00437570
//   (?ChangeGraphicsBankScene@CPlayerGui@@QAEXXZ, Ego_r 0x0044A200 / FableWin 0x01A7CC80), vector<CTexture>::erase 0x0049B3B0 and
//   RenderProgress 0x00499880 take the spellings the header-based siblings already bind.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D_LOADINGSCREEN
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK
#define FABLE_RETAIL_SINGLETON_ISSETUP
#include "fable_string.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CTexture.h"
#include "engine/CSystemManager.h"
#include "engine/CRenderManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CProgressDisplay.h"

CSystemManager* __fastcall GFGetSystemManager();      // 0x009A4EC0
extern double g_ProgressScreenWidth;                   // 0x013961E8
extern double g_ProgressScreenHeight;                  // 0x013961F0
extern bool   g_ProgressDisplayEnabled;                // 0x01375445
extern bool   g_ProgressDisplaySuppressed;             // 0x013B89D3
extern bool   g_ProgressBlackScreenA;                  // 0x013B89C1
extern bool   g_ProgressBlackScreenB;                  // 0x013B89C0

void CProgressDisplay::StopProgress(bool showStoppingText, bool clearScreen)
{
    if (!g_ProgressDisplayEnabled || g_ProgressDisplaySuppressed)
        return;
    if (Initialised) {
        if (clearScreen) {
            CDisplayManager* display = GFGetSystemManager()->GetDisplayManager();
            if (display) {
                CRenderManager* renderer = &display->GetRenderManager();
                if (renderer) {
                    bool wasRendering = display->IsRendering();
                    if (!wasRendering)
                        display->BeginRender();
                    C2DBoxF box(0.0f, 0.0f, (float)g_ProgressScreenWidth, (float)g_ProgressScreenHeight);
                    renderer->Draw2DBox(box, CRGBColour(0, 0, 0, 0xff), 0);
                    renderer->Render2DDrawList(true);
                    display->EndRender();
                    display->SwapScreens();
                    if (wasRendering)
                        display->BeginRender();
                }
            }
        } else if (!DebugDisplay && showStoppingText) {
            bool oldBlackScreen = BlackScreen;
            if (g_ProgressBlackScreenA || g_ProgressBlackScreenB) {
                BlackScreen = true;
                SetToDisplayText(false);
            }
            RenderProgress("Stopping progress display", -1.0f, false, false);
            BlackScreen = oldBlackScreen;
        }
    }
    unsigned int i;
    for (i = 0; i < ProgressTextures.size(); ++i)
        ProgressTextures[i].Uninitialise();
    ProgressTextures.clear();
    for (i = 0; i < GuildSealTextures.size(); ++i)
        GuildSealTextures[i].Uninitialise();
    GuildSealTextures.clear();
    for (i = 0; i < LoadingScreenTextures.size(); ++i)
        LoadingScreenTextures[i].Uninitialise();
    LoadingScreenTextures.clear();
    if (CPlayerGui::IsSingletonSetup())
        CPlayerGui::Get().ChangeGraphicsBankScene();
    Initialised = false;
}