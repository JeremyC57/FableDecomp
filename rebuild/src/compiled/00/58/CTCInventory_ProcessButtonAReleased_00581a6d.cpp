// CTCInventory::ProcessButtonAReleased() -- outside the quick-access modes and in item selection, selects the highlighted item and closes the inventory when asked to; then the base handling
// Leaf: ?ProcessButtonAReleased@CTCInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581A6D, length 70 bytes.
// Callees: NInventory::CTCInventoryBase::SelectHighlightedItem(bool&) 0x005BDD04, the virtual
//          AddCloseInventoryEvent (slot 32, `call [eax+80h]`; CTCInventory 0x00584A73), and
//          NInventory::CTCInventoryBase::ProcessButtonAReleased() 0x005BC65C (landed).
// Vtable slot 58 of CTCInventory (0x01250DE4). Ego_r PDB: ?ProcessButtonAReleased@CTCInventory@@UAEXXZ.
// Retail layout (engine/CTCInventory.h is the wider PDB layout): InventorySelectionState +0x1c (base,
// PDB +0x20), EnableQuickAccessAssign +0x1cc, EnableQuickAccessRemove +0x1cd.
// Shape: frame-pointer /Os TU (`push ecx` frame for the out bool, `leave`): `optimize("y",off)` +
// Names (NF95, 2026-11-19, queue q11234): moved off the row-local CTCInventoryBase / CTCInventory models onto engine/CTCInventory.h
//   (PASS22 complete class) and engine/NInventory_CTCInventoryBase.h, so the helper binds Ego_r.pdb 0x00620A83 / FableWin.pdb 0x02368DDE
//   ?SelectHighlightedItem@CTCInventoryBase@NInventory@@QAEXAA_N@Z (0x005BDD04) instead of the global-namespace stand-in.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#include "engine/ETCInterfaceType.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventory.h"

void CTCInventory::ProcessButtonAReleased()
{
    if (!EnableQuickAccessAssign && !EnableQuickAccessRemove && InventorySelectionState == INVENTORY_SELECTION_STATE_ITEMS) {
        bool closeInventory = false;
        SelectHighlightedItem(closeInventory);
        if (closeInventory)
            AddCloseInventoryEvent();
    }
    NInventory::CTCInventoryBase::ProcessButtonAReleased();
}
