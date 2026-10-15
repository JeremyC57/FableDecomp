// CTCInventoryItem::GetBoughtPrice() -- ask the owning inventory for the price paid for this item's def, 0 when not in an inventory
// Retail 0x005D8E70, 29 bytes (+3 int3). Caller: CCreatureAction_UseWallMount::PerformAction
// (0x008483EF) on the thing's type-26 component, i.e. a CTCInventoryItem: PInventory is +0x10, the
// owning thing (CTCBase::ParentThing, +0x04) holds its def index as an unsigned short at +0x8C.
// Shape: `this` parked in eax, PInventory loaded into ecx and null-tested; the non-null arm pushes the
// zero-extended def index and calls the CTCInventoryBase price query 0x005BEAD5 (thiscall, one stack
// argument -- so no tail jump: `call ; ret`); the null arm returns 0.
// engine/CTCInventoryItem.h flattens the polymorphic CTCBase base into a POD, so a local model with
// the header's offsets (PInventory +0x10) is used; CThing is modelled by the one word read.
// Size-optimised TU: `#pragma optimize("s", on)` (the speed build zero-extends into edx).
#pragma optimize("s", on)

struct CThing {
    unsigned char _pad_00[0x8C];
    unsigned short DefIndex;                       // +0x8C
};

struct CTCInventoryBase {
    long GetBoughtPrice(long defIndex);            // 0x005BEAD5
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};

struct CTCInventoryItem : CTCBase {
    void* PDef;                                    // +0x0C
    CTCInventoryBase* PInventory;                  // +0x10
    long GetBoughtPrice();
};

long CTCInventoryItem::GetBoughtPrice()
{
    if (PInventory)
        return PInventory->GetBoughtPrice(ParentThing->DefIndex);
    return 0;
}
