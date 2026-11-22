// CGameComponent::Init() -- the base component needs no initialisation: an empty virtual (`ret`)
// Leaf: ?Init@CGameComponent@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415920, 1 byte (`ret`); vtable 0x0122ECB0 slot 1.
// Names (NF233, 2026-11-22, q12638): Ego_r.pdb + FableWin.pdb + Ego_d.pdb ?Init@CGameComponent@@UAEXXZ (public virtual), the spelling the
//   CGameComponent vtables of 0x0051D8E0 / 0x0042F50B / 0x00418B7A bind; was the placeholder `struct T` (?Init@T@@QAEXXZ). On engine/CGameComponent.h.
#include "engine/CGameComponent.h"

void CGameComponent::Init()
{
}
