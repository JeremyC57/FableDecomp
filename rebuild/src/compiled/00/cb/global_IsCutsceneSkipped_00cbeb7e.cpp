// IsCutsceneSkipped() -- true when the player skipped the cutscene: the action-mode button when that skip mode is set, else the interface's cut-scene-skipped message
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?IsCutsceneSkipped@@YI_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBEB7E, 41 bytes, `__fastcall` global with no arguments, plain `ret`
//   (PDB ?IsCutsceneSkipped@@YI_NXZ).
// Data: the skip-with-action-button flag byte @ 0x0143E8F4 (descriptive name), the game script interface @ 0x0143E8F8.
// Callees: CGameScriptInterface (vtable 0x01260F0C) slot 42 MsgIsActionModeButtonPressed (+0xa8, 0x00894440) and slot 44
//   MsgIsCutSceneSkipped (+0xb0, 0x00893BA0); the flag picks one.
// Shape: an if/else on the flag with one `if (call()) return true;` per arm and a shared `return false`: the interface
//   and its vtable are loaded once above the flag branch (common to both arms), the two calls merge into one byte
//   test and the early returns give `mov al,1` / `xor al,al` (a ternary of the two calls gives `setne al`, 37 v 41).
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern bool g_bSkipCutsceneWithActionButton;                  // 0x0143E8F4
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

bool __fastcall IsCutsceneSkipped()
{
    if (g_bSkipCutsceneWithActionButton) {
        if (g_pGameScriptInterface->MsgIsActionModeButtonPressed())
            return true;
    } else {
        if (g_pGameScriptInterface->MsgIsCutSceneSkipped())
            return true;
    }
    return false;
}