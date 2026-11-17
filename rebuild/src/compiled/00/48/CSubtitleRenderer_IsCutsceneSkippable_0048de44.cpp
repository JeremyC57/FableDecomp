// CSubtitleRenderer::IsCutsceneSkippable() const -- false while the world is performing a region load (CWorld RegionLoadStatus +0x104); true outside cutscenes; inside one only when the cutscene is skippable, not yet skipped, no forced-wait text is up and the skip prompt has finished opening
// Leaf: IsCutsceneSkippable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048DE44, 70 bytes. No callees. Global: the game-component pointer @
// 0x013B86A0 (its +0x24 word is PWorld, Ego_r.pdb CMainGameComponent).
// Layout: engine/CSubtitleRenderer.h now carries the retail offsets (DisplayFlags +0x7C, the cutscene bools +0xDC..,
// SkipCutsceneTimer +0xE8); NF47 moved the row onto it (the earlier local model existed because the header was the PDB view).
// Shape: one zero register (`xor eax,eax ; cmp [mem],al`) serves every compare = a size TU (#pragma optimize("s",
// on)); the in-cutscene checks are NESTED under `if (InCutscene)` so the not-in-cutscene path and the last test share
// the trailing `mov al,1` (flat early returns end in `sete al`); the early-outs share the `xor al,al` exit.
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its world (PWorld +0x24, the inline PeekWorld()) and the world's
//   RegionLoadStatus +0x104 through the inline FableWin.pdb ?IsPerformingRegionLoadReally@CWorld@@QBE_NXZ (engine/CWorld.h); the row's
//   "display engine +0x24 / m_busy +0x104" reading was wrong (Ego_r.pdb CMainGameComponent +0x24 is PWorld, CWorld +0x104 RegionLoadStatus).
//   The local CSubtitleRenderer / CFadeInFadeOutBase / C2DBoxF / C2DVector / CRGBColour / CWideString models are replaced by
//   engine/CSubtitleRenderer.h (BUTTONGRAPHICS: SkipCutsceneTimer typed) and the shared headers it pulls in; was the stand-in CGameComponent_0048de44* g_pGameComponent with its CDisplayEngine_0048de44 stand-in.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ISPERFORMINGREGIONLOADREALLY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONGRAPHICS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CSubtitleRenderer.h"

bool CSubtitleRenderer::IsCutsceneSkippable() const
{
    if (CMainGameComponent::Get()->PeekWorld().IsPerformingRegionLoadReally())
        return false;
    if (InCutscene) {
        if (!CutsceneSkippable)
            return false;
        if (CutsceneSkipped)
            return false;
        if (DisplayFlags & 0x48)
            return false;
        if (!SkipCutsceneTimer.Active)
            return false;
        if (SkipCutsceneTimer.Opening)
            return false;
    }
    return true;
}