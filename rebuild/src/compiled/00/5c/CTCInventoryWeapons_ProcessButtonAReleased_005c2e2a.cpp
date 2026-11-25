// CTCInventoryWeapons::ProcessButtonAReleased() -- in item-selection state selects the highlighted item and, if one was selected, raises the close-inventory event; then runs the base handler
// Leaf: ?ProcessButtonAReleased@CTCInventoryWeapons@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2E2A, length 52 bytes. Frame-pointer size TU: optimize("s", on)
// (one-byte `selected` local at [ebp-1] in a `push ecx` frame). Virtual slot 58 of the CTCInventoryWeapons vtable 0x01253FF4.
// The catalog row is labelled CTCInventoryClothing::ProcessButtonAReleased (propagated from the byte-twin 0x005B34D5); the only
// reference to this body is slot 58 of the CTCInventoryWeapons vtable, so it is the weapons screen's override.
// Callees: CTCInventoryBase::SelectHighlightedItem(bool&) @ 0x005BDD04, virtual slot 32 (+0x80, AddCloseInventoryEvent @ 0x005BC527
// in these vtables), CTCInventoryBase::ProcessButtonAReleased @ 0x005BC65C (the explicit base call).
// Names (NF95, 2026-11-19, queue q11234): moved off the row-local CTCInventoryBase_Retail / CTCInventoryWeapons_Retail stand-ins onto engine/CTCInventoryWeapons.h
//   (opt-in FABLE_RETAIL_CTCINVENTORYWEAPONS_PROCESSBUTTONARELEASED: Ego_r.pdb / FableWin.pdb ?ProcessButtonAReleased@CTCInventoryWeapons@@UAEXXZ) and
//   engine/NInventory_CTCInventoryBase.h, so the helper binds ?SelectHighlightedItem@CTCInventoryBase@NInventory@@QAEXAA_N@Z (0x005BDD04)
//   and the base call ?ProcessButtonAReleased@CTCInventoryBase@NInventory@@UAEXXZ (0x005BC65C).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: this row's object is byte-identical without it (size mode keeps the
//   leaf's frame; T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_PROCESSBUTTONARELEASED
#include "engine/ETCInterfaceType.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryWeapons.h"

void CTCInventoryWeapons::ProcessButtonAReleased()
{
    if (InventorySelectionState == INVENTORY_SELECTION_STATE_ITEMS) {
        bool selected = false;
        SelectHighlightedItem(selected);
        if (selected)
            AddCloseInventoryEvent();
    }
    NInventory::CTCInventoryBase::ProcessButtonAReleased();
}
