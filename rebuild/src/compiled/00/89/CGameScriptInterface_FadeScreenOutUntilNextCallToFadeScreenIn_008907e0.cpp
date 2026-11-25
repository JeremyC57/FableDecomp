// CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn(float fade_time, float hold_time) -- a black FadeScreenOut, then the display engine's fade-out lock when it started
// Leaf: ?FadeScreenOutUntilNextCallToFadeScreenIn@CGameScriptInterface@@UAEXMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008907E0. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: ??0CRGBColour@@QAE@EEE@Z (0, 0, 0; alpha 255, inline in retail), the virtual
//   ?FadeScreenOut@CGameScriptInterface@@UAE_NMMVCRGBColour@@@Z (slot 373, +0x5D4) with (fade_time, hold_time, colour), then, when it returns
//   true, the inline ?SetScreenFadeOutLockFlag@CDisplayEngine@@QAEX_N@Z (true; DisplayEngine +0x0c, byte +0xD8).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CDISPLAYENGINE_SETSCREENFADEOUTLOCKFLAG
#include "engine/CGameScriptInterface.h"
#include "engine/CRGBColour.h"
#include "engine/CDisplayEngine.h"

void CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn(float fade_time, float hold_time)
{
    if (FadeScreenOut(fade_time, hold_time, CRGBColour(0, 0, 0)))
        DisplayEngine->SetScreenFadeOutLockFlag(true);
}
