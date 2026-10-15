#include <cstdio>
struct CEngineGraphic { unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
static long g_lastType = -1;
static CEngineGraphic GetEngineGraphic(long itemType) { g_lastType = itemType; CEngineGraphic g = { (unsigned long)(itemType * 10), 0, 0, false, 0 }; return g; }
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long* begin() const { return first; }
    long& operator[](unsigned long n) { return *(begin() + n); }
};
// Behaviour of CTCInventory::GetQuickAccessItemGraphicIndex (0x00581FA4): the slot's graphic bank index, 0 for an empty (<= 0) slot.
struct CTCInventory {
    CLongVector QuickAccessItems;
    long GetQuickAccessItemGraphicIndex(long index)
    {
        if (QuickAccessItems[index] > 0)
            return GetEngineGraphic(QuickAccessItems[index]).BankIndex;
        return 0;
    }
};
int main()
{
    long items[4] = { 0, 7, -1, 3 };
    CTCInventory inv; inv.QuickAccessItems.first = items; inv.QuickAccessItems.last = items + 4; inv.QuickAccessItems.end_of_storage = items + 4;
    if (inv.GetQuickAccessItemGraphicIndex(0) != 0 || g_lastType != -1) { std::printf("00581fa4_TEST FAIL\n"); return 1; }
    if (inv.GetQuickAccessItemGraphicIndex(1) != 70 || g_lastType != 7) { std::printf("00581fa4_TEST FAIL\n"); return 1; }
    if (inv.GetQuickAccessItemGraphicIndex(2) != 0 || inv.GetQuickAccessItemGraphicIndex(3) != 30) { std::printf("00581fa4_TEST FAIL\n"); return 1; }
    std::printf("00581fa4_TEST PASS\n"); return 0;
}
