// CGameScriptInterface::IsScreenFadingOut() const -- the display engine's screen fade-out is active and opening
// Leaf: ?IsScreenFadingOut@CGameScriptInterface@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088E4D0. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: DisplayEngine (+0x0c), the inline ?IsScreenFadeOut@CDisplayEngine@@QBE_NXZ (ScreenFadeOutInfo Active, +0xBC) and
//   ?IsScreenFadeOutOpening@CDisplayEngine@@QBE_NXZ (Opening, +0xC9), both expanded in retail.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CFADEINFADEOUTBASE_OPEN
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISOPENING_PASS26
#define FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFADEOUTOPENING
#include "engine/CGameScriptInterface.h"
#include "engine/CDisplayEngine.h"

bool CGameScriptInterface::IsScreenFadingOut() const
{
    return DisplayEngine->IsScreenFadeOut() && DisplayEngine->IsScreenFadeOutOpening();
}
