// CFrontendGameComponent::OnPreDeviceReset() -- nothing to release before a device reset (the CDeviceResetCallback override)
// Leaf: ?OnPreDeviceReset@CFrontendGameComponent@@UAEXXZ
// Retail: Fable.exe @ 0x00494350, 1 byte: `ret`.
// Names (NF235, 2026-11-22, q12656): Ego_r.pdb + FableWin.pdb + Ego_d.pdb ?OnPreDeviceReset@CFrontendGameComponent@@UAEXXZ (public
//   virtual); retail RTTI vtable 0x0123875C (COL 0x0133309C, the CDeviceResetCallback subobject at +4) slot 0 holds this address.
//   Was the free stand-in ?CFrontendGameComponent_OnPreDeviceReset@@YIXXZ; now the member on engine/CFrontendGameComponent.h, with the
//   vtable emitters 0x00495620 / 0x00496070, which bind slot 0 under the PDB spelling.
#include "engine/CFrontendGameComponent.h"

void CFrontendGameComponent::OnPreDeviceReset()
{
}
