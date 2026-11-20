// CThingManager::RemoveFromPhysicalList(CThingPhysical& thing) -- an empty body in retail (`ret 4`)
// Leaf: ?RemoveFromPhysicalList@CThingManager@@QAEXAAVCThingPhysical@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051DFE0, 3 bytes: `ret 4` (thiscall, one reference argument, no code).
// Identity (two sources): Ego_r.pdb 0x00527F00 / FableWin.pdb 0x01D40AC0 ?RemoveFromPhysicalList@CThingManager@@QAEXAAVCThingPhysical@@@Z;
//   its only caller in the exe is ~CThingPhysical 0x006A5D50 (`GetThingManager().RemoveFromPhysicalList(*this);`, landed, already binding
//   this spelling; the landed 0x00838A00 donor-order note: AddToPhysicalList ... RemoveFromPhysicalList). No other E8 / E9 / absolute
//   reference reaches 0x0051DFE0, so the empty body is not a linker fold of another function.
// Was: the placeholder C0051dfe0::M(int) (rebuild/src/compiled/00/51/E2_E2_0051dfe0.cpp).
// Header: engine/CThingManager.h (overlay opt-in FABLE_RETAIL_CTHINGMANAGER_REMOVEFROMPHYSICALLIST, a declaration only).
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_REMOVEFROMPHYSICALLIST
#include "engine/CThingManager.h"

void CThingManager::RemoveFromPhysicalList(CThingPhysical& thing)
{
}
