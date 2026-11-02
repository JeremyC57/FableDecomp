// Behaviour test for 0x005D8E90 CTCInventoryItem::GetInventoryOwner:
// the owning inventory's parent thing, else the thing with InventoryUIDToAttachTo (only when the 64-bit UID is non-zero), else 0.
#include <cstdio>
struct Thing { int Id; };
static Thing g_things[3] = { { 10 }, { 11 }, { 12 } };
static unsigned long long g_lastUid = 0;
struct ThingManager {
    const Thing* PeekPThingWithUID(unsigned long long uid) const { g_lastUid = uid; return uid == 0x100000002ULL ? &g_things[2] : 0; }
};
static ThingManager g_manager;
struct TCBase {
    Thing* ParentThing;
    const ThingManager& GetThingManager() const { return g_manager; }
};
struct TCInventoryBase : TCBase { };
struct TCInventoryItem : TCBase {
    void* PDef; TCInventoryBase* PInventory; unsigned long long InventoryUIDToAttachTo; bool DefaultItem;
    const Thing* GetInventoryOwner() const
    {
        if (PInventory)
            return PInventory->ParentThing;
        if (InventoryUIDToAttachTo > 0)
            return GetThingManager().PeekPThingWithUID(InventoryUIDToAttachTo);
        return 0;
    }
};
int main()
{
    TCInventoryBase inventory; inventory.ParentThing = &g_things[0];
    TCInventoryItem item; item.ParentThing = &g_things[1]; item.PDef = 0; item.DefaultItem = false;
    item.PInventory = &inventory; item.InventoryUIDToAttachTo = 0x100000002ULL;
    if (item.GetInventoryOwner() != &g_things[0] || g_lastUid != 0) { std::printf("005d8e90_TEST FAIL\n"); return 1; }
    item.PInventory = 0;
    if (item.GetInventoryOwner() != &g_things[2] || g_lastUid != 0x100000002ULL) { std::printf("005d8e90_TEST FAIL\n"); return 1; }
    item.InventoryUIDToAttachTo = 0x100000000ULL;           // high word only: still looked up
    if (item.GetInventoryOwner() != 0 || g_lastUid != 0x100000000ULL) { std::printf("005d8e90_TEST FAIL\n"); return 1; }
    item.InventoryUIDToAttachTo = 0; g_lastUid = 7;
    if (item.GetInventoryOwner() != 0 || g_lastUid != 7) { std::printf("005d8e90_TEST FAIL\n"); return 1; }
    std::printf("005d8e90_TEST PASS\n");
    return 0;
}