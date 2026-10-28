// Behaviour test for 0x005BF00B NInventory::CItem::RemoveItem: the count drops by n (clamped at zero); an emptied slot resets its per-item data and clears its type and average bought price, a slot with items left keeps them
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static int g_released = 0;
struct CP { void* Data; void* Info; void ReleaseData() { ++g_released; Data = 0; Info = 0; } void Reset(void* d) { ReleaseData(); Data = d; } };
inline int ClampToZero(int& v) { if (v < 0) v = 0; return v; }
struct CItem {
    int ItemType; int NumberOfItems; CP PPerItemData; int AverageBoughtPrice;
    void RemoveItem(int count) { NumberOfItems -= count; if (ClampToZero(NumberOfItems) == 0) { PPerItemData.Reset(0); AverageBoughtPrice = 0; ItemType = 0; } }
};
int main()
{
    int data; CItem i; i.ItemType = 12; i.NumberOfItems = 5; i.PPerItemData.Data = &data; i.PPerItemData.Info = &data; i.AverageBoughtPrice = 40;
    i.RemoveItem(2); CHECK(i.NumberOfItems == 3 && i.ItemType == 12 && i.AverageBoughtPrice == 40 && g_released == 0);
    i.RemoveItem(7); CHECK(i.NumberOfItems == 0 && i.ItemType == 0 && i.AverageBoughtPrice == 0 && g_released == 1 && i.PPerItemData.Data == 0 && i.PPerItemData.Info == 0);
    i.ItemType = 3; i.NumberOfItems = 2; i.RemoveItem(2); CHECK(i.NumberOfItems == 0 && i.ItemType == 0 && g_released == 2);
    if (fails == 0) printf("005bf00b_TEST PASS\n"); else printf("005bf00b_TEST FAIL\n");
    return fails;
}