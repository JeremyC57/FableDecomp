#ifndef FABLE_ENGINE_NPLAYERGUI_GRAPHICS_H
#define FABLE_ENGINE_NPLAYERGUI_GRAPHICS_H
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
namespace NPlayerGui {
// Original Ego00686ac7/FableWin020d908c: PC mapping copies the input vector.
inline C2DVector FABLE_FASTCALL GFToScreen(const C2DVector& pos) { return pos; }
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
}
#endif
