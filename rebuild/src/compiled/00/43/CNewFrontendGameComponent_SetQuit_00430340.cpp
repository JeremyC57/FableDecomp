// CNewFrontendGameComponent::SetQuit() -- the front end's public override of the protected CGameComponent::SetQuit: raise Quit (+0x08).
// Leaf: ?SetQuit@CNewFrontendGameComponent@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00430340, 5 bytes: `mov byte ptr [ecx+8],1 ; ret`. FableWin.pdb ?SetQuit@CNewFrontendGameComponent@@UAEXXZ
//   (public virtual) and Ego_d.pdb's CNewFrontendGameComponent field list (SetQuit, public, virtual); retail RTTI vtable
//   ??_7CNewFrontendGameComponent@@6BCBaseClassNonCopyable@@@ (0x01230CA0) slot 4 (0x01230CB0) holds 0x00430340, where CGameComponent's slot 4 is
//   0x00415930 (?SetQuit@CGameComponent@@MAEXXZ). Callers reach it only through slot 4 (`call [eax+10h]`).
// Layout = engine/CGameComponent.h: Quit +0x08 (protected, Ego_r.pdb). Header: engine/CNewFrontendGameComponent.h with the
//   FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_SETQUIT opt-in (nf230); was a local stand-in struct with a non-virtual SetQuit (QAE).
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_SETQUIT
#include "engine/CNewFrontendGameComponent.h"

void CNewFrontendGameComponent::SetQuit()
{
    Quit = true;
}
