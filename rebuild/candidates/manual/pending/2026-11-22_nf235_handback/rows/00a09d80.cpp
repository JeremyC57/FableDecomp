// CMeshBankManager::OnPostDeviceReset() -- nothing to rebuild after a device reset; report success (the CDeviceResetCallback override)
// Leaf: ?OnPostDeviceReset@CMeshBankManager@@UAE_NXZ
// Retail: Fable.exe @ 0x00A09D80, 3 bytes: `mov al,1 ; ret`.
// Names (NF235, 2026-11-22, q12657): Ego_r.pdb + FableWin.pdb ?OnPostDeviceReset@CMeshBankManager@@UAE_NXZ (public virtual, bool);
//   retail RTTI vtable 0x0129C980 (COL 0x01356A64, the CDeviceResetCallback subobject at +4) slot 1 holds this address. Was the free
//   stand-in ?CMeshBankManager_OnPostDeviceReset@@YI_NXZ; now the member on engine/CMeshBankManager.h (CTOR_DTOR opt-in), with the
//   vtable emitters 0x00A0A110 / 0x00A0A150, which bind slot 1 under the PDB spelling.
#define FABLE_RETAIL_CMESHBANKMANAGER_CTOR_DTOR
#include "engine/CMeshBankManager.h"

bool CMeshBankManager::OnPostDeviceReset()
{
    return true;
}
