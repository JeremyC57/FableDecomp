#include <cstdio>
class CThing { public: long type; };
static CThing g_made; static int g_creates = 0;
struct Inv {
    long counts[4];
    long GetNumberOfItemsOfTypeInInventory(long itemType) { return counts[itemType]; }
    CThing* CreateInventoryObject(long itemType) { ++g_creates; g_made.type = itemType; return &g_made; }
    CThing* GetItemOfTypeInInventory(long itemType)
    {
        if (GetNumberOfItemsOfTypeInInventory(itemType) > 0)
            return CreateInventoryObject(itemType);
        return 0;
    }
};
int main()
{
    Inv inv; inv.counts[0] = 0; inv.counts[1] = 3; inv.counts[2] = -1; inv.counts[3] = 1;
    if (inv.GetItemOfTypeInInventory(0) || inv.GetItemOfTypeInInventory(2) || g_creates) { std::printf("005bf3f1_TEST FAIL\n"); return 1; }
    CThing* t = inv.GetItemOfTypeInInventory(1);
    if (t != &g_made || t->type != 1 || g_creates != 1) { std::printf("005bf3f1_TEST FAIL\n"); return 1; }
    std::printf("005bf3f1_TEST PASS\n");
    return 0;
}