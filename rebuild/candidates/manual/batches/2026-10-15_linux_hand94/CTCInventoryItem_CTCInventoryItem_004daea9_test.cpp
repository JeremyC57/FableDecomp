#include <cstdio>
#include <new>
// Behaviour test for 0x004DAEA9: base built from the owner, every member cleared.
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct CThing { int id; };
struct CTCInventoryBase;
struct CInventoryItemDef;
struct CTCBase {
    virtual void v00() {}
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing& owner) : ParentThing(&owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
};
struct CInventoryItemDefPointer { const CInventoryItemDef* Object; CInventoryItemDefPointer() : Object(0) {} };
struct CTCInventoryItem : CTCBase {
    CInventoryItemDefPointer PDef;
    CTCInventoryBase* PInventory;
    u64 InventoryUIDToAttachTo;
    bool DefaultItem;
    virtual void v00() {}
    CTCInventoryItem(CThing& owner);
};
CTCInventoryItem::CTCInventoryItem(CThing& owner)
    : CTCBase(owner), PInventory(0), InventoryUIDToAttachTo(0), DefaultItem(false)
{
}
int main()
{
    CThing t;
    unsigned char raw[sizeof(CTCInventoryItem)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAB;
    CTCInventoryItem* item = new (raw) CTCInventoryItem(t);
    if (item->ParentThing != &t || item->PDef.Object != 0 || item->PInventory != 0 || item->InventoryUIDToAttachTo != 0 || item->DefaultItem) {
        std::printf("004daea9_TEST FAIL\n"); return 1;
    }
    std::printf("004daea9_TEST PASS\n");
    return 0;
}
