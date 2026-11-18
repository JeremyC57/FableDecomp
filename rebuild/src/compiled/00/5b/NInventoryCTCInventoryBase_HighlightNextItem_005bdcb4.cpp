// NInventory::CTCInventoryBase::HighlightNextItem() -- while the inventory is in item-selection state (1) and no category slide is running, plays the movement sound and moves the selected category's highlight to the next item
// Leaf: ?HighlightNextItem@CTCInventoryBase@NInventory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDCB4, length 40 bytes; size TU (the landed CTCInventoryBase module is "s").
// Callees: NInventory::CTCInventoryBase::PlayInventoryMovementSound 0x005BD33C (private const), NInventory::CCategory::HighlightNextItem
//   0x005BD054 (tail jump `jmp` on &Categories[SelectedCategoryIndex], E9 at 0x005BDCD5).
// Name fix (agentNF91, q11203): Ego_r.pdb (0x0062026D) + FableWin.pdb (0x02368D18) ?HighlightNextItem@CTCInventoryBase@NInventory@@QAEXXZ (the
//   PASS22 complete-methods block of engine/NInventory_CTCInventoryBase.h declares it). The old row defined ?HighlightNextItem@CTCInventoryBase_Retail@@QAEXXZ
//   on local CTCInventoryBase_Retail / CCategory_Retail / vector views and tail-jumped to 0x005BD054 as ?HighlightNextItem@CCategory_Retail@@QAEXXZ;
//   now on the shared headers it binds ?HighlightNextItem@CCategory@NInventory@@QAEXXZ (NF89's 0x005BD054 spelling) and the Ego_r.pdb / FableWin.pdb
//   ?PlayInventoryMovementSound@CTCInventoryBase@NInventory@@ABEXXZ (private const).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_UNIQUEITEMCOUNT
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
void CTCInventoryBase::HighlightNextItem()
{
    if (InventorySelectionState == INVENTORY_SELECTION_STATE_ITEMS && !CategoryMoving) {
        PlayInventoryMovementSound();
        Categories[SelectedCategoryIndex].HighlightNextItem();
    }
}
}
