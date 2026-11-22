// CFrontendGameComponent::SetQuit() -- ask the debug front end to quit (the Quit flag of the game-component base, +8)
// Leaf: ?SetQuit@CFrontendGameComponent@@UAEXXZ
// Retail: Fable.exe @ 0x00494370, 5 bytes: `mov byte ptr [ecx+8],1 ; ret`.
// Names (NF235, 2026-11-22, q12656): Ego_r.pdb + FableWin.pdb + Ego_d.pdb ?SetQuit@CFrontendGameComponent@@UAEXXZ, the public virtual
//   override of the protected CGameComponent::SetQuit (?SetQuit@CGameComponent@@MAEXXZ 0x00415930); retail RTTI vtable 0x01238768
//   (COL 0x013330F0) slot 4 holds this address. Was the stand-in ?SetQuit@CFrontendGameComponent@@QAEXXZ on a local
//   `struct CFrontendGameComponent { char pad[8]; char quit; }` model; now on engine/CFrontendGameComponent.h (SETQUIT opt-in), with
//   the vtable emitters 0x00495620 / 0x00496070, which bind slot 4 under the PDB spelling.
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SETQUIT
#include "engine/CFrontendGameComponent.h"

void CFrontendGameComponent::SetQuit()
{
    Quit = true;
}
