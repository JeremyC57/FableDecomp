// CFrontendGameComponent::OnPostDeviceReset() -- nothing to rebuild after a device reset; report success (the CDeviceResetCallback override)
// Leaf: ?OnPostDeviceReset@CFrontendGameComponent@@UAE_NXZ
// Retail: Fable.exe @ 0x00494360, 3 bytes: `mov al,1 ; ret`.
// Names (NF235, 2026-11-22, q12656): Ego_r.pdb + FableWin.pdb + Ego_d.pdb ?OnPostDeviceReset@CFrontendGameComponent@@UAE_NXZ (public
//   virtual, bool); retail RTTI vtable 0x0123875C (COL 0x0133309C) slot 1 holds this address. Was the free stand-in
//   ?CFrontendGameComponent_OnPostDeviceReset@@YI_NXZ; now the member on engine/CFrontendGameComponent.h, with the vtable emitters
//   0x00495620 / 0x00496070, which bind slot 1 under the PDB spelling.
#include "engine/CFrontendGameComponent.h"

bool CFrontendGameComponent::OnPostDeviceReset()
{
    return true;
}
