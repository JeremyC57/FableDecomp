#ifndef FABLE_ENGINE_NPLAYERGUI_GRAPHICS_H
#define FABLE_ENGINE_NPLAYERGUI_GRAPHICS_H
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
#ifdef FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#include "engine/C2DBoxF.h"
#endif
#ifdef FABLE_RETAIL_NPLAYERGUI_GFTOSCREENSCALED_PASS26
#include "engine/C2DBoxF.h"
#endif
namespace NPlayerGui {
#ifdef FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
// Both original overload names; identity-copy expansion native006507d5/006509f8.
inline C2DBoxF FABLE_FASTCALL GFToScreen(const C2DBoxF& box){return box;}
#endif
// Original Ego00686ac7/FableWin020d908c: PC mapping copies the input vector.
#ifdef FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_CALLED
// Retail opt-in (Codex pass 24): both PDB names and three calls to 0x00643BE2.
C2DVector FABLE_FASTCALL GFToScreen(const C2DVector& pos);
#else
inline C2DVector FABLE_FASTCALL GFToScreen(const C2DVector& pos) { return pos; }
#endif
#ifdef FABLE_RETAIL_NPLAYERGUI_ANCHORS
// Retail opt-in (chunk 2043, row 0x0064ED4F NPlayerGui::CDrawDamage::Draw): the 640x480 GUI anchoring helpers, Ego_r.pdb / FableWin.pdb
// ?GFAnchorTo{TopLeft,TopRight,BottomLeft,BottomRight}@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z (__fastcall, hidden C2DVector result).
// GFAnchorToTopLeft is the identity: retail's out-of-line copy 0x00643C94 (`mov eax,ecx ; mov ecx,[edx] ; mov [eax],ecx ;
// mov ecx,[edx+4] ; mov [eax+4],ecx ; ret`, uncalled: every caller expands it, e.g. 0x0064EDE2 in Draw) sits right before
// GFAnchorToTopRight 0x00643CA1, the Ego_r.pdb order (0x00686B8B / 0x00686B98). The other three are called (landed rows).
inline C2DVector FABLE_FASTCALL GFAnchorToTopLeft(const C2DVector& pos) { return pos; }   // 0x00643C94
C2DVector FABLE_FASTCALL GFAnchorToTopRight(const C2DVector& pos);                         // 0x00643CA1
C2DVector FABLE_FASTCALL GFAnchorToBottomLeft(const C2DVector& pos);                       // 0x00643CDA
C2DVector FABLE_FASTCALL GFAnchorToBottomRight(const C2DVector& pos);                      // 0x00643D0C
#endif
#ifdef FABLE_RETAIL_NPLAYERGUI_GFTOSCREENSCALED_PASS26
// Retail opt-in (wfE9, row 0x00648820 NPlayerGui::CDrawMiniMap::Draw): Ego_r.pdb / FableWin.pdb
// ?GFToScreenScaled@NPlayerGui@@YI?AVC2DBoxF@@ABV2@@Z (__fastcall, hidden C2DBoxF result), out of line at 0x00643BFC (landed row
// NPlayerGui_GFToScreenScaled_00643bfc.cpp). Declaration only (includes engine/C2DBoxF.h).
C2DBoxF FABLE_FASTCALL GFToScreenScaled(const C2DBoxF& box);                               // 0x00643BFC
#endif
}
#endif
