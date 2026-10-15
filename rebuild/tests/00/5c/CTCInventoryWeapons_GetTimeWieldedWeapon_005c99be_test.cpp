#include <cstdio>
// Behaviour of CTCInventoryWeapons::GetTimeWieldedWeapon (0x005C99BE).
struct CPerItemData { long TimeWielded; };
struct CItem { long ItemType; long NumberOfItems; CPerItemData* PPerItemData; void* PPerItemDataBlock; long AverageBoughtPrice; };
static long g_destroyed = 0;
struct CItemVector {
    CItem* _First; CItem* _Last; CItem* _End;
    CItemVector() : _First(0), _Last(0), _End(0) {}
    ~CItemVector() { ++g_destroyed; }
    CItem* begin() { return _First; }
    CItem* end() { return _Last; }
};
static CItem g_items[2];
struct CTCInventoryBase_Retail {
    long m_count;
    void GetAllInventoryItemsOfType(long itemType, CItemVector* items) { if (itemType == 3) { items->_First = g_items; items->_Last = g_items + m_count; } }
};
struct CTCInventoryWeapons : CTCInventoryBase_Retail {
    long GetTimeWieldedWeapon(long itemType)
    {
        long timeWielded = 0;
        CItemVector items;
        GetAllInventoryItemsOfType(itemType, &items);
        if (items.begin() != items.end()) {
            CPerItemData* data = items.begin()->PPerItemData;
            if (data != 0)
                timeWielded = data->TimeWielded;
        }
        return timeWielded;
    }
};
int main()
{
    CPerItemData d; d.TimeWielded = 1234;
    g_items[0].PPerItemData = &d; g_items[1].PPerItemData = 0;
    CTCInventoryWeapons w; w.m_count = 2;
    int bad = (w.GetTimeWieldedWeapon(3) != 1234);
    bad |= (w.GetTimeWieldedWeapon(4) != 0);
    g_items[0].PPerItemData = 0; bad |= (w.GetTimeWieldedWeapon(3) != 0);
    bad |= (g_destroyed != 3);
    if (bad) { std::printf("005c99be_TEST FAIL\n"); return 1; }
    std::printf("005c99be_TEST PASS\n");
    return 0;
}