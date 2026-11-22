// CMainGameComponent::OnPreDeviceReset() -- CDeviceResetCallback override: nothing to release before a device reset
// Leaf: ?OnPreDeviceReset@CMainGameComponent@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00415E83, 1 byte (`ret`); slot 0 of the CDeviceResetCallback vtable 0x0122F174 (its sibling slot 1 is
//   OnPostDeviceReset 0x004175DA).
// Name (NF278, 2026-11-22): Ego_r.pdb 0x00415023 + FableWin.pdb 0x018A169C ?OnPreDeviceReset@CMainGameComponent@@UAEXXZ; both landed vtable
//   emitters (0x00418DCA ctor, 0x00418B7A dtor) bind slot 0 of 0x0122F174 under this spelling. Was the stand-in struct T ?Hook@T@@QAEXXZ.
//   On the shared header (engine/CMainGameComponent.h declares the override on its default path).
#include "engine/CMainGameComponent.h"

void CMainGameComponent::OnPreDeviceReset()
{
}
