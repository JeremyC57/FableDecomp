// NInventory::CCategory::SelectHighlightedItem(bool& close_input_process) -- select the highlighted inventory slot if it holds anything:
//   when the inventory tracks selected items, re-selecting the selected slot deselects it (selection -1, highlight back to 0), otherwise
//   the slot becomes the selection; then tell the inventory (item deselected or item selected) with the slot's item type
// Leaf: ?SelectHighlightedItem@CCategory@NInventory@@QAEXAA_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD9B2, length 107 bytes (ret 4); size-optimised TU
//   (`or dword ptr [esi+20h],-1`, `and dword ptr [esi+24h],0`, pass-through `push dword ptr [esp+8]`).
// Identity (NF92, q11210 binder): Ego_r.pdb 0x006208AF + FableWin.pdb 0x0236333E ?SelectHighlightedItem@CCategory@NInventory@@QAEXAA_N@Z;
//   Ego_d.pdb names the parameter close_input_process and the locals item_deselected / item_def_index.
// Callees: CTCInventoryBase::IsTrackingSelectedItems 0x005BD4CE (Ego_r 0x00620377 / FableWin 0x0236A8C7, the only bool const query of the
//   class; it reads the inventory def's byte at retail +0x58 = Ego_r CInventoryDef::TrackSelectedItems +0x5C less the 4-byte vector
//   difference), CCategory::SetSelectedItemIndex 0x005BC479, CTCInventoryBase virtual slots 35 OnItemDeselected / 33 OnItemSelected.
// Port (NF92): was the local CCategory_Retail / CTCInventoryBase / CDefPointer model (it bound ?SetSelectedItemIndex@CCategory_Retail@@QAEXJ@Z
//   and ?CanToggleSelection@CTCInventoryBase@@QAE_NXZ); now engine/CCategory.h (NF90 _SETSELECTEDITEMINDEX, NF92 _SELECTHIGHLIGHTEDITEM)
//   and the complete CTCInventoryBase class of engine/NInventory_CTCInventoryBase.h (ORIGINAL_COMPLETE_METHODS_PASS22).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CBASE_ORIGINAL_MEMBERS_PASS22
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_SETSELECTEDITEMINDEX
#define FABLE_RETAIL_CCATEGORY_SELECTHIGHLIGHTEDITEM
#include "fable_string.h"
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
void CCategory::SelectHighlightedItem(bool& close_input_process)
{
    if (ItemsInCategory[HighlightedItemIndex].IsSlotEmpty())
        return;
    bool item_deselected = false;
    if (PInventory->IsTrackingSelectedItems()) {
        if (SelectedItemIndex == HighlightedItemIndex) {
            SelectedItemIndex = -1;
            HighlightedItemIndex = 0;
            item_deselected = true;
        } else {
            SetSelectedItemIndex(HighlightedItemIndex);
        }
    }
    long item_def_index = ItemsInCategory[HighlightedItemIndex].ItemType;
    if (item_deselected)
        PInventory->OnItemDeselected(item_def_index, close_input_process);
    else
        PInventory->OnItemSelected(item_def_index, close_input_process);
}
}
