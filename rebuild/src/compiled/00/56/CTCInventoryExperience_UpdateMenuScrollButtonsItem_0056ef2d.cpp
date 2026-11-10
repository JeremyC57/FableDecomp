// CTCInventoryExperience::UpdateMenuScrollButtonsItem() -- while the item list is active, light the item scroll arrows that can move
// Leaf: ?UpdateMenuScrollButtonsItem@CTCInventoryExperience@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056EF2D, 66 bytes, thiscall, `ret`. Vtable 0x0124F11C slot 29 (the base's
//   NInventory::CTCInventoryBase::UpdateMenuScrollButtonsItem is the empty `ret` 0x005BC58A). Identity: Ego_r.pdb
//   ?UpdateMenuScrollButtonsItem@CTCInventoryExperience@@UAEXXZ is the only UpdateMenuScrollButtons* override in the class's PDB name set,
//   and the body sets the item arrows; reached through `jmp [eax+74h]` from ProcessInventoryInputUp / Down 0x0056EC8C / 0x0056ECCB.
// In the item state (InventorySelectionState +0x1C == 1) both category arrows go OFF (1); the up arrow is ON (2) when the first
// displayed stat index is above 0, the down arrow when the last one is below statIndicatorMax - 1.
// Retail loads statIndicatorMax into edi before the up-arrow setg: the count is read into a named local at the top of the
//   item branch (0x0056EF49 `mov edi,[ecx+1C0h]`); reading the member in the down-arrow compare gives the edx/edi swap.
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
        long statCount = statIndicatorMax;
        MenuArrowHighlightsCategoryUp = SELECTION_STATE_OFF;
        MenuArrowHighlightsCategoryDn = SELECTION_STATE_OFF;
        if (MenuDisplayListExperienceItem[0] > 0) MenuArrowHighlightsItemUp = SELECTION_STATE_ON;
        else MenuArrowHighlightsItemUp = SELECTION_STATE_OFF;
        if (MenuDisplayListExperienceItem[MenuDisplayListExperienceItem.size() - 1] < statCount - 1) MenuArrowHighlightsItemDn = SELECTION_STATE_ON;
        else MenuArrowHighlightsItemDn = SELECTION_STATE_OFF;
    }
}