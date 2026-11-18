// NInventory::CCategory::SetSelectedItemIndex(long index) -- select a slot: the selection and the highlight both take the index; no
//   selection (-1) puts the highlight back on slot 0
// Leaf: ?SetSelectedItemIndex@CCategory@NInventory@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC479, 22 bytes, size-optimised TU (`ret 4`).
// Identity (NF92, q11210): Ego_r.pdb 0x0061F2FF + FableWin.pdb ?SetSelectedItemIndex@CCategory@NInventory@@QAEXJ@Z (public); the ported
//   0x005BF060 / 0x005BF5A4 bind this address under it (engine/CCategory.h opt-in FABLE_RETAIL_CCATEGORY_SETSELECTEDITEMINDEX, NF90);
//   Ego_d.pdb names the parameter `index`. Was the local model `CCategory_Methods : CCategory` (leaf
//   ?SetSelectedItemIndex@CCategory_Methods@@QAEXJ@Z) over the generated header; now the retail class of engine/CCategory.h.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_SETSELECTEDITEMINDEX
#include "fable_string.h"
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
void CCategory::SetSelectedItemIndex(long index)
{
    SelectedItemIndex = index;
    HighlightedItemIndex = index;
    if (index == -1)
        HighlightedItemIndex = 0;
}
}
