// CMainGameComponent::SetQuit() -- CGameComponent override: flag the component to quit (Quit = true)
// Leaf: ?SetQuit@CMainGameComponent@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004197A0, 5 bytes: `mov byte ptr [ecx+8],1 ; ret` (CGameComponent::Quit +8, Ego_r.pdb layout);
//   slot 4 of vtable 0x0122F180 (the base table 0x0122ECB0 has CGameComponent::SetQuit 0x00415930 there).
// Name (NF278, 2026-11-22): FableWin.pdb 0x018A11F0 ?SetQuit@CMainGameComponent@@UAEXXZ (public virtual); both landed vtable emitters
//   (0x00418DCA ctor, 0x00418B7A dtor) bind slot 4 under this spelling. Was the stand-in struct T ?SetQuit@T@@QAEXXZ. On the shared header
//   with the landed declaration opt-in FABLE_RETAIL_CMAINGAMECOMPONENT_INIT_SETQUIT (NF275).
#define FABLE_RETAIL_CMAINGAMECOMPONENT_INIT_SETQUIT
#include "engine/CMainGameComponent.h"

void CMainGameComponent::SetQuit()
{
    Quit = true;
}
