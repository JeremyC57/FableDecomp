// CTCInventoryExperience::UpdateMenuScrollButtonsItem() -- while the item list is active, light the item scroll arrows that can move
// Grade: DIFFER(66v66) -- register tie only, 12 instructions: retail loads statIndicatorMax into edi right after `push edi` (before the setg of the up arrow) and keeps the last index in edx; ours loads _M_finish into edi after the up-arrow store and statIndicatorMax into edx. Same instructions otherwise; invariant under ~19 spellings (if/else, ?:, named last/limit/list locals, reversed compares, +1 form, iterator, default-then-set).
// Leaf: ?UpdateMenuScrollButtonsItem@CTCInventoryExperience@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056EF2D, 66 bytes, thiscall, `ret`. Vtable 0x0124F11C slot 29 (the base's
//   NInventory::CTCInventoryBase::UpdateMenuScrollButtonsItem is the empty `ret` 0x005BC58A). Identity: Ego_r.pdb
//   ?UpdateMenuScrollButtonsItem@CTCInventoryExperience@@UAEXXZ is the only UpdateMenuScrollButtons* override in the class's PDB name set,
//   and the body sets the item arrows; reached through `jmp [eax+74h]` from ProcessInventoryInputUp / Down 0x0056EC8C / 0x0056ECCB.
// In the item state (InventorySelectionState +0x1C == 1) both category arrows go OFF (1); the up arrow is ON (2) when the first
// displayed stat index is above 0, the down arrow when the last one is below statIndicatorMax - 1.
// Layout: engine/NInventory_CTCInventoryBase.h (InventorySelectionState +0x1C, MenuArrowHighlights* +0x38..+0x44),
//   engine/CTCInventoryExperience.h retail branch (statIndicatorMax +0x1C0, MenuDisplayListExperienceItem +0x208 vector<long>).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_MENULIST
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VIRTUALS
#include "engine/CTCInventoryExperience.h"

void CTCInventoryExperience::UpdateMenuScrollButtonsItem()
{
    if (InventorySelectionState == INVENTORY_SELECTION_STATE_ITEMS) {
        MenuArrowHighlightsCategoryUp = SELECTION_STATE_OFF;
        MenuArrowHighlightsCategoryDn = SELECTION_STATE_OFF;
        if (MenuDisplayListExperienceItem[0] > 0) MenuArrowHighlightsItemUp = SELECTION_STATE_ON;
        else MenuArrowHighlightsItemUp = SELECTION_STATE_OFF;
        if (MenuDisplayListExperienceItem[MenuDisplayListExperienceItem.size() - 1] < statIndicatorMax - 1) MenuArrowHighlightsItemDn = SELECTION_STATE_ON;
        else MenuArrowHighlightsItemDn = SELECTION_STATE_OFF;
    }
}
