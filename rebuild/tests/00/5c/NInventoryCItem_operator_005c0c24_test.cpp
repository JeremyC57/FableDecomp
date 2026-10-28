// Behaviour test for 0x005C0C24 NInventory::CItem::operator=: type, count and average price assigned, the per-item data through the counted pointer's assignment; returns *this
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static int g_cpCopies = 0, g_cpAssigns = 0;
struct CP { void* Data; int* Info;
    CP() : Data(0), Info(0) { }
    CP(const CP& o) : Data(o.Data), Info(o.Info) { ++g_cpCopies; if (Info) ++*Info; }
    CP& operator=(const CP& o) { ++g_cpAssigns; if (Info != o.Info) { if (Info) --*Info; Data = o.Data; Info = o.Info; if (Info) ++*Info; } return *this; } };
struct CItem {
    int ItemType; int NumberOfItems; CP PPerItemData; int AverageBoughtPrice;
    CItem() { }
    CItem(const CItem& o) : ItemType(o.ItemType), NumberOfItems(o.NumberOfItems), PPerItemData(o.PPerItemData), AverageBoughtPrice(o.AverageBoughtPrice) { }
    CItem& operator=(const CItem& o) { ItemType = o.ItemType; NumberOfItems = o.NumberOfItems; PPerItemData = o.PPerItemData; AverageBoughtPrice = o.AverageBoughtPrice; return *this; }
};

int main()
{
    int ca = 1, cb = 1; int da, db;
    CItem a; a.ItemType = 3; a.NumberOfItems = 4; a.PPerItemData.Data = &da; a.PPerItemData.Info = &ca; a.AverageBoughtPrice = 25;
    CItem b; b.ItemType = 9; b.NumberOfItems = 1; b.PPerItemData.Data = &db; b.PPerItemData.Info = &cb; b.AverageBoughtPrice = 7;
    CItem& r = (b = a);
    CHECK(&r == &b && b.ItemType == 3 && b.NumberOfItems == 4 && b.AverageBoughtPrice == 25 && b.PPerItemData.Data == &da);
    CHECK(ca == 2 && cb == 0 && g_cpAssigns == 1 && g_cpCopies == 0);
    if (fails == 0) printf("005c0c24_TEST PASS\n"); else printf("005c0c24_TEST FAIL\n");
    return fails;
}