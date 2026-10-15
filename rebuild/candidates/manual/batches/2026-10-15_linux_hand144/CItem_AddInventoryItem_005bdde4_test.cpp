// Behaviour test for 0x005bdde4 (CItem::AddInventoryItem): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
struct CItem {
    long ItemType; long NumberOfItems; int perItem; long AverageBoughtPrice; long maximum;
    long GetMaximumNumberOfItems() { return NumberOfItems ? maximum : 0; }
    void AddInventoryItem(const CItem& other)
    {
        if (NumberOfItems == 0) {
            NumberOfItems = other.NumberOfItems; ItemType = other.ItemType; perItem = other.perItem; AverageBoughtPrice = other.AverageBoughtPrice;
        } else {
            long room = GetMaximumNumberOfItems() - NumberOfItems;
            long added = Min(room, other.NumberOfItems);
            long total = other.AverageBoughtPrice * added + AverageBoughtPrice * NumberOfItems;
            NumberOfItems = added + NumberOfItems;
            AverageBoughtPrice = total / NumberOfItems;
        }
    }
};
static void Run()
{
    CItem a; a.ItemType = 0; a.NumberOfItems = 0; a.perItem = 0; a.AverageBoughtPrice = 0; a.maximum = 10;
    CItem b; b.ItemType = 7; b.NumberOfItems = 4; b.perItem = 9; b.AverageBoughtPrice = 100; b.maximum = 10;
    a.AddInventoryItem(b); CHECK(a.ItemType == 7 && a.NumberOfItems == 4 && a.perItem == 9 && a.AverageBoughtPrice == 100);
    CItem c = b; c.NumberOfItems = 2; c.AverageBoughtPrice = 40;
    a.AddInventoryItem(c); CHECK(a.NumberOfItems == 6 && a.AverageBoughtPrice == 80);
    c.NumberOfItems = 9; c.AverageBoughtPrice = 0;
    a.AddInventoryItem(c); CHECK(a.NumberOfItems == 10 && a.AverageBoughtPrice == 48);
}

int main()
{
    Run();
    if (g_fail) { std::printf("005bdde4_TEST FAIL\n"); return 1; }
    std::printf("005bdde4_TEST PASS\n");
    return 0;
}
