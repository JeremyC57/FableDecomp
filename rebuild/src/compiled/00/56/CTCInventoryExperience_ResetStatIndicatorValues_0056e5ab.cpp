// CTCInventoryExperience::ResetStatIndicatorValues() -- reset the stat indicator: position 0, maximum the slot count of the selected category
// Leaf: ?ResetStatIndicatorValues@CTCInventoryExperience@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E5AB, 31 bytes, thiscall plain `ret`; size-optimised TU (`and [esi+1BCh],0`).
// Callees: NInventory::CTCInventoryBase::GetSelectedCategoryIndex @ 0x005BC4B2, GetNumberOfSlotsForCategory @ 0x005BD328 (both on `this`).
// Name fix (agentNF89, q11167): Ego_r.pdb ?ResetStatIndicatorValues@CTCInventoryExperience@@QAEXXZ (engine/CTCInventoryExperience.h VSLOTS3 declares it
//   at this address; the landed 0x0056EC79 binds it). Was the __fastcall global stand-in ?CTCInventoryExperience_ResetStatIndicatorValues@@YIX..
//   on a row-local CTCInventoryExperience model, binding 0x005BC4B2 as ?HelperGetValue@@YAHXZ and 0x005BD328 as StoreIndicator. Now on the
//   shared headers: statIndicatorPos +0x1BC / statIndicatorMax +0x1C0 (Ego_r names), GetSelectedCategoryIndex and the PDB
//   GetNumberOfSlotsForCategory (overlay opt-in CTCINVENTORYBASE_NUMBEROFSLOTSFORCATEGORY).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VSLOTS3
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDCATEGORYINDEX
#define FABLE_RETAIL_CTCINVENTORYBASE_NUMBEROFSLOTSFORCATEGORY
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryExperience.h"
void CTCInventoryExperience::ResetStatIndicatorValues()
{
    statIndicatorPos = 0;
    statIndicatorMax = GetNumberOfSlotsForCategory(GetSelectedCategoryIndex());
}
