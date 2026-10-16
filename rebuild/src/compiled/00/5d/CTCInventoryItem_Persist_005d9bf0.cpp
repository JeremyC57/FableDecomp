// CTCInventoryItem::Persist(CPersistContext* context, bool flag) -- persists "InventoryUID": when saving, the UID of the inventory's owner (0 when unattached); when loading, into InventoryUIDToAttachTo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D9BF0, length 81 bytes (+ int3 padding). Vtable 0x01240004 (CTCInventoryItem) slot 1.
// Callees: CPersistContext::Transfer<unsigned __int64>(name, value, default) @ 0x0044BB00 (manifest "Transfer<signed_char>", folded).
// Shape: two Transfer calls (save / load arms) that VC7.1 tail-merges into one call on a selected pointer (`lea eax,[esp]` / `add eax,18h`); the 64-bit
// zero default is a temporary bound to the const reference ([esp+8]). Context mode +0x18: 1 and 3 are the save modes.
// Layout: CTCInventoryItem PInventory +0x10, InventoryUIDToAttachTo +0x18 (engine header); CTCInventoryBase ParentThing +0x04;
// CThing UID +0x18 (local models).
// Leaf: Persist
#include "engine/CTCInventoryItem.h"

struct CPersistContext {
    unsigned char _pad_0x00[0x18];
    long Mode;                                                // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
};
struct CThing_Persist {
    unsigned char _pad_0x00[0x18];
    unsigned __int64 UID;                                     // +0x18
};
struct CTCInventoryBase_Persist {
    void* __vftable;
    CThing_Persist* ParentThing;                              // +0x04
};

struct CTCInventoryItem_Persist : CTCInventoryItem {
    void Persist(CPersistContext* context, bool flag);
};

void CTCInventoryItem_Persist::Persist(CPersistContext* context, bool flag)
{
    if (context->IsSaving()) {
        unsigned __int64 uid = 0;
        if (PInventory)
            uid = ((CTCInventoryBase_Persist*)PInventory)->ParentThing->UID;
        context->Transfer("InventoryUID", uid, 0);
    } else
        context->Transfer("InventoryUID", InventoryUIDToAttachTo, 0);
}