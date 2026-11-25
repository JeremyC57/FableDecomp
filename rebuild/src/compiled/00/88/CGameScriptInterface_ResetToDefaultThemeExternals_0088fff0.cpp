// CGameScriptInterface::ResetToDefaultThemeExternals(float time) const -- World's environment: TransitionToDefaultTheme(0, 1, time)
// Leaf: ?ResetToDefaultThemeExternals@CGameScriptInterface@@UBEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088FFF0. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: the inline ?GetEnvironment@CWorld@@QAEAAVCEnvironment@@XZ (World +0x18), then
//   ?TransitionToDefaultTheme@CEnvironment@@QAEXJJM@Z (retail 0x006B3880) with (0, 1, time) (`push 1 ; push 0`, 0x0088FFFB).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME_CHANNEL
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"

void CGameScriptInterface::ResetToDefaultThemeExternals(float time) const
{
    World->PEnvironment_ptr->TransitionToDefaultTheme(0, 1, time);
}
