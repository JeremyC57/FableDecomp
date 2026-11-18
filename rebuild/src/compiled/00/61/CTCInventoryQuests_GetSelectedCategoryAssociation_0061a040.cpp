// CTCInventoryQuests::GetSelectedCategoryAssociation() -- the thing associated with the currently selected category (the quest card of the selected quest)
// Leaf: ?GetSelectedCategoryAssociation@CTCInventoryQuests@@QAEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A040, length 24 bytes; size TU (sibling `jmp` for the last call).
// No direct caller or vtable reference in the image: an out-of-line copy in the CTCInventoryQuests TU (between GetScreenType 0x0061A010 and
//   ProcessInventoryInputLeft 0x0061A120).
// Callees: NInventory::CTCInventoryBase::GetSelectedCategoryIndex @ 0x005BC4B2, GetCategoryWithIndex(long) @ 0x005BCE3C,
//   NInventory::CCategory::GetCategoryThingAssociation @ 0x005BD1AE.
// Name fix (agentNF89, q11165 / q11166 / q11167): Ego_r.pdb (0x00597A90) + FableWin.pdb ?GetSelectedCategoryAssociation@CTCInventoryQuests@@QAEPAVCThing@@XZ,
//   the only CTCInventoryQuests member of either PDB returning CThing* with no argument; the body is exactly
//   GetCategoryWithIndex(GetSelectedCategoryIndex())->GetCategoryThingAssociation(), and the address sits in the CTCInventoryQuests TU.
//   Was the catalog's CTCInventoryBase::GetSelectedCategoryThing on the row-local CTCInventoryBase_Retail / CCategory_Retail models (no PDB has
//   that name). Now on engine/CTCInventoryQuests.h (overlay opt-in FABLE_RETAIL_CTCINVENTORYQUESTS_GETSELECTEDCATEGORYASSOCIATION) and the
//   NInventory_CTCInventoryBase.h declarations NF87's 0x006223C0 uses.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDCATEGORYINDEX
#define FABLE_RETAIL_CTCINVENTORYBASE_CATEGORYWITHINDEX
#define FABLE_RETAIL_NINVENTORY_CCATEGORY_ACCESSORS
#define FABLE_RETAIL_CTCINVENTORYQUESTS_GETSELECTEDCATEGORYASSOCIATION
#include "engine/CTCInventoryQuests.h"
CThing* CTCInventoryQuests::GetSelectedCategoryAssociation()
{
    return GetCategoryWithIndex(GetSelectedCategoryIndex())->GetCategoryThingAssociation();
}
