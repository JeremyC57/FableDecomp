// NUISystem::CManager::GetMouseDisabled() -- whether the mouse is disabled (MouseDisabled +0xC9)
// Leaf: ?GetMouseDisabled@CManager@NUISystem@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042D4F0, 7 bytes (`mov al,[ecx+0C9h] ; ret`).
// Identity (NF227, 2026-11-22, q12582 / q12583): slot 11 of NUISystem::CManager's vtable 0x01230134 (the only reference to this
//   address, .rdata 0x01230160); engine/NUISystem_CManager.h slot 11 is `virtual bool GetMouseDisabled();` (the header vtable COMDAT of
//   0x0041E641 / 0x0041E3F6 matches retail slot for slot). Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?GetMouseDisabled@CManager@NUISystem@@UAE_NXZ
//   (public virtual). The body reads MouseDisabled, which the header places at +0xC9.
// Was: NUISystem::CManager::vfunc_11 (rebuild/src/compiled/00/42/gapscan_sub0042d4f0_0042d4f0.cpp, placeholder ?Get@T@@QAEDXZ).
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
bool CManager::GetMouseDisabled()
{
    return MouseDisabled;
}
}
