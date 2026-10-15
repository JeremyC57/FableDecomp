#include <cstdio>
// Behaviour test for 0x005D8E70: no inventory -> 0, else the inventory's price for the owner's def index.
struct CThing { unsigned short DefIndex; };
static long g_seen = -1;
struct CTCInventoryBase { long GetBoughtPrice(long defIndex) { g_seen = defIndex; return defIndex * 10 + 1; } };
struct CTCBase { virtual void v00() {} CThing* ParentThing; };
struct CTCInventoryItem : CTCBase {
    void* PDef;
    CTCInventoryBase* PInventory;
    long GetBoughtPrice();
};
long CTCInventoryItem::GetBoughtPrice()
{
    if (PInventory)
        return PInventory->GetBoughtPrice(ParentThing->DefIndex);
    return 0;
}
int main()
{
    CThing t; t.DefIndex = 0xFFFE;
    CTCInventoryBase inv;
    CTCInventoryItem item; item.ParentThing = &t; item.PInventory = 0;
    if (item.GetBoughtPrice() != 0 || g_seen != -1) { std::printf("005d8e70_TEST FAIL\n"); return 1; }
    item.PInventory = &inv;
    if (item.GetBoughtPrice() != 0xFFFE * 10 + 1 || g_seen != 0xFFFE) { std::printf("005d8e70_TEST FAIL\n"); return 1; }
    std::printf("005d8e70_TEST PASS\n");
    return 0;
}
