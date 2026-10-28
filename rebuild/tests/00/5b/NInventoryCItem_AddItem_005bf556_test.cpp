// Behaviour test for 0x005BF556 NInventory::CItem::AddItem: an empty slot forgets its average price; the type becomes the thing's definition index; a known price (not -1) is folded into the running average (sum * n + price) / (n + 1); the count goes up by one and the item stats are refreshed from the thing
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
class CThing { public: unsigned short DefGlobalIndex; };
static const CThing* g_updated = 0;
struct CItem {
    int ItemType; int NumberOfItems; int AverageBoughtPrice;
    void UpdateItemStats(const CThing& t) { g_updated = &t; }
    void AddItem(CThing& thing, int price)
    {
        if (NumberOfItems == 0) AverageBoughtPrice = 0;
        ItemType = thing.DefGlobalIndex;
        if (price != -1) { AverageBoughtPrice = AverageBoughtPrice * NumberOfItems + price; ++NumberOfItems; AverageBoughtPrice /= NumberOfItems; }
        else ++NumberOfItems;
        UpdateItemStats(thing);
    }
};
int main()
{
    CThing t; t.DefGlobalIndex = 0xBEEF;
    CItem i; i.ItemType = 0; i.NumberOfItems = 0; i.AverageBoughtPrice = 999;
    i.AddItem(t, 100); CHECK(i.ItemType == 0xBEEF && i.NumberOfItems == 1 && i.AverageBoughtPrice == 100 && g_updated == &t);
    i.AddItem(t, 50);  CHECK(i.NumberOfItems == 2 && i.AverageBoughtPrice == 75);
    i.AddItem(t, -1);  CHECK(i.NumberOfItems == 3 && i.AverageBoughtPrice == 75);
    i.AddItem(t, 0);   CHECK(i.NumberOfItems == 4 && i.AverageBoughtPrice == 56);     // (75 * 3 + 0) / 4
    if (fails == 0) printf("005bf556_TEST PASS\n"); else printf("005bf556_TEST FAIL\n");
    return fails;
}