// Behaviour test for 0x005D8EC0 CTCInventoryItem::OnDie:
// when in an inventory, ask it to remove the parent thing and forget it; then the base OnDie runs either way.
#include <cstdio>
struct Thing { unsigned short DefIndex; };
static int g_removed = 0;
static int g_baseOnDie = 0;
static const Thing* g_removedThing = 0;
struct TCBase {
    Thing* ParentThing;
    virtual ~TCBase() { }
    virtual void OnDie() { ++g_baseOnDie; }
};
struct TCInventoryBase : TCBase {
    void RemoveItemFromInventory(Thing& thing) { ++g_removed; g_removedThing = &thing; }
};
struct TCInventoryItem : TCBase {
    void* PDef; TCInventoryBase* PInventory;
    void RemoveFromInventory() { if (PInventory) { PInventory->RemoveItemFromInventory(*ParentThing); PInventory = 0; } }
    virtual void OnDie() { RemoveFromInventory(); TCBase::OnDie(); }
};
int main()
{
    Thing owner = { 5 };
    TCInventoryBase inventory; inventory.ParentThing = 0;
    TCInventoryItem item; item.ParentThing = &owner; item.PDef = 0; item.PInventory = &inventory;
    TCBase* base = &item;
    base->OnDie();
    if (g_removed != 1 || g_removedThing != &owner || item.PInventory != 0 || g_baseOnDie != 1) { std::printf("005d8ec0_TEST FAIL\n"); return 1; }
    base->OnDie();
    if (g_removed != 1 || g_baseOnDie != 2) { std::printf("005d8ec0_TEST FAIL\n"); return 1; }
    std::printf("005d8ec0_TEST PASS\n");
    return 0;
}