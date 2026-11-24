// NUISystem::CManager::GetRightStickForce() -- the right-stick force vector by reference (RightStickForce +0x94)
// Leaf: ?GetRightStickForce@CManager@NUISystem@@UAEAAVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D5AA, 7 bytes (`lea eax,[ecx+94h] ; ret`).
// Identity (NF227, 2026-11-22, q12582 / q12583): slot 39 of NUISystem::CManager's vtable 0x01230134 (the only reference to this
//   address, .rdata 0x012301D0); engine/NUISystem_CManager.h slot 39 is `virtual C2DVector & GetRightStickForce();` (the header vtable
//   COMDAT of 0x0041E641 / 0x0041E3F6 matches retail slot for slot). Ego_r.pdb / FableWin.pdb / Ego_d.pdb
//   ?GetRightStickForce@CManager@NUISystem@@UAEAAVC2DVector@@XZ (public virtual). SetRightStickForce 0x0042D592 (slot 38) stores the
//   same member.
// Was: NUISystem::CManager::vfunc_39 (rebuild/src/compiled/00/42/gapscan_sub0042d5aa_0042d5aa.cpp, placeholder ?GetAddr@T@@QAEPAUSub@@XZ).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
C2DVector& CManager::GetRightStickForce()
{
    return RightStickForce;
}
}
