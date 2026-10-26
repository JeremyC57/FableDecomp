// CTCInventoryItem::CTCInventoryItem(CThing& owner) -- CTCBase(owner), then a null def, no inventory, UID 0, not a default item
// Retail 0x004DAEA9, 41 bytes, ret 4. Called from the factory 0x004DC5C7 (CTCInventoryItem_allocate).
// Shape: the base constructor CTCBase::CTCBase(CThing&) 0x00686800 gets the owner through the
// pass-through `push dword ptr [esp+8]` (/Os), then the CTCInventoryItem vtable 0x01240004 is stored and
// every member is zeroed from one `xor eax,eax`: PDef (CDefPointer<const CInventoryItemDef>, +0x0C),
// PInventory (+0x10), InventoryUIDToAttachTo (unsigned __int64, +0x18/+0x1C) and DefaultItem (+0x20).
// Layout = engine/CTCInventoryItem.h (Ego_r.pdb, 0x28 bytes); that header flattens the polymorphic
// CTCBase base into a POD, so the class is modelled locally with the same offsets.
// Leaf: ??0CTCInventoryItem
#pragma optimize("s", on)

class CThing;
struct CTCInventoryBase;
class CInventoryItemDef;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing& owner);                        // 0x00686800
};

struct CInventoryItemDefPointer {                  // CDefPointer<const CInventoryItemDef>
    const CInventoryItemDef* Object;
    CInventoryItemDefPointer() : Object(0) { }
};

class CTCInventoryItem : public CTCBase { public:                // vtable 0x01240004
    CInventoryItemDefPointer PDef;                 // +0x0C
    CTCInventoryBase* PInventory;                  // +0x10
    unsigned __int64 InventoryUIDToAttachTo;       // +0x18
    bool DefaultItem;                              // +0x20
    virtual void v00();
    CTCInventoryItem(CThing& owner);
};

CTCInventoryItem::CTCInventoryItem(CThing& owner)
    : CTCBase(owner), PInventory(0), InventoryUIDToAttachTo(0), DefaultItem(false)
{
}