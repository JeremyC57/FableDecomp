// NUISystem::CManager::SetRightStickForce(C2DVector&) -- store the right-stick force vector (RightStickForce +0x94)
// Leaf: ?SetRightStickForce@CManager@NUISystem@@UAEXAAVC2DVector@@@Z
// Retail 0x0042D592, 24 bytes (vtable 0x01230134 slot 38, its only reference, .rdata 0x012301CC). Name: Ego_r.pdb / FableWin.pdb /
// Ego_d.pdb ?SetRightStickForce@CManager@NUISystem@@UAEXAAVC2DVector@@@Z (public virtual).
// Port (NF227, 2026-11-22, q12582 / q12583): onto engine/NUISystem_CManager.h (retail layout: RightStickForce +0x94). Was a row-local
//   CManager model with a non-virtual member, leaf ?SetRightStickForce@CManager@NUISystem@@QAEXAAVC2DVector@@@Z, while the header vtable
//   COMDAT of 0x0041E641 / 0x0041E3F6 binds this slot as UAE.
#pragma optimize("s", on)
// CManager TU is size-optimised (odd addresses, `pop ecx` after cdecl calls).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetRightStickForce(C2DVector& force)
{
    RightStickForce = force;
}
}
