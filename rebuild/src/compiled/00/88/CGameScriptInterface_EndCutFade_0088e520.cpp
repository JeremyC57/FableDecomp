// CGameScriptInterface::EndCutFade() -- end a cut-scene fade: EndFadeScreen(), then Rest(0.5f)
// Leaf: ?EndCutFade@CGameScriptInterface@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088E520, 22 bytes. `push 3F000000h` is the float argument 0.5f.
// Body port (NF321, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT; was
//   ?EndCutFade@CGameScriptInterface_Methods@@QAEXXZ); the retail RTTI vftable placement names this PDB member here. Callees, as the donor
//   FableWin.exe EndCutFade calls them (both PDB name sets): ?EndFadeScreen@CGameScriptInterface@@IAEXXZ 0x006E7270 and
//   ?Rest@CGameScriptInterface@@IBEXM@Z 0x006E71F0 (landed self row), declared by the add-only opt-in
//   FABLE_RETAIL_CGAMESCRIPTINTERFACE_ENDCUTFADE_CALLEES (were the made-up helperA() / helperB(int)).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTINTERFACE_ENDCUTFADE_CALLEES
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EndCutFade()
{
    EndFadeScreen();
    Rest(0.5f);
}
