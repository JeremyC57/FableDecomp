// CGameScriptInterface::ResetToDefaultTheme(float time) const -- World's environment: TransitionToDefaultThemeAllChannels(1, time)
// Leaf: ?ResetToDefaultTheme@CGameScriptInterface@@UBEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088FF10. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: the inline ?GetEnvironment@CWorld@@QAEAAVCEnvironment@@XZ (World +0x18), then
//   ?TransitionToDefaultThemeAllChannels@CEnvironment@@QAEXJM@Z (retail 0x006B3950) with priority 1 (`push 1`, 0x0088FF1B).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"

void CGameScriptInterface::ResetToDefaultTheme(float time) const
{
    World->PEnvironment_ptr->TransitionToDefaultThemeAllChannels(1, time);
}
