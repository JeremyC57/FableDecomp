// CTCInventoryItem::GetBoughtPrice() const -- ask the owning inventory for the price paid for this item's def, 0 when not in an inventory
// Leaf: ?GetBoughtPrice@CTCInventoryItem@@QBEJXZ
// Retail 0x005D8E70, 29 bytes (+3 int3). Caller: CCreatureAction_UseWallMount::PerformAction (0x008483EF, no landed row) on the thing's
// type-26 component, i.e. a CTCInventoryItem: PInventory is +0x10, the owning thing (CTCBase::ParentThing, +0x04) holds its def index as
// an unsigned short at +0x8C.
// Name: Ego_r.pdb 0x00559DD0 / FableWin.pdb 0x01E009B0 ?GetBoughtPrice@CTCInventoryItem@@QBEJXZ (const; the landed leaf was QAE).
// Shape: `this` parked in eax, PInventory loaded into ecx and null-tested; the non-null arm pushes the zero-extended def index and calls
// NInventory::CTCInventoryBase::GetBoughtPrice 0x005BEAD5 (thiscall, one stack argument -- so no tail jump: `call ; ret`); the null arm
// returns 0. Size-optimised TU: `#pragma optimize("s", on)` (the speed build zero-extends into edx).
// Port (NF88, round 7, q11153 binder: it bound 0x005BEAD5 as ?GetBoughtPrice@CTCInventoryBase@@QAEJJ@Z through local CTCInventoryBase /
//   CTCBase / CTCInventoryItem models): the row is on engine/CTCInventoryItem.h (the CTCBase-derived retail branch + NF88 opt-in
//   FABLE_RETAIL_CTCINVENTORYITEM_GETBOUGHTPRICE), engine/NInventory_CTCInventoryBase.h (GetBoughtPrice declared) and engine/CThing.h.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_GETBOUGHTPRICE
#include "engine/CThing.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryItem.h"

long CTCInventoryItem::GetBoughtPrice() const
{
    if (PInventory)
        return PInventory->GetBoughtPrice(ParentThing->GetDefGlobalIndex());
    return 0;
}
