// CGameComponent::SetQuit() -- the base component's protected quit hook does nothing (`ret`); overrides set their own flag
// Leaf: ?SetQuit@CGameComponent@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415930, 1 byte (`ret`); vtable 0x0122ECB0 slot 4.
// Names (NF233, 2026-11-22, q12638): Ego_r.pdb + FableWin.pdb + Ego_d.pdb ?SetQuit@CGameComponent@@MAEXXZ (protected virtual), the spelling the
//   header-built CGameComponent vtables (0x0051D8E0 / 0x0042F50B) bind; was the placeholder `struct T` (?Init@T@@QAEXXZ). On engine/CGameComponent.h.
//   Landed 0x00418B7A still binds the public stand-in ?SetQuit@CGameComponent@@UAEXXZ from its local model (NAME_FIXES).
#include "engine/CGameComponent.h"

void CGameComponent::SetQuit()
{
}
