// Behaviour test for 0x005BDE96 NInventory::CCategory::GetNumberOfItemsOfTypeInCategory: NumberOfItems of the first item whose type matches the definition index (find_if with bind2nd(mem_fun_ref(&CItem::IsSameItemTypeAsDefIndex))), else 0
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
struct CItem { int ItemType; int NumberOfItems; int pad[3]; bool IsSameItemTypeAsDefIndex(int d) const { return ItemType == d; } };
struct CCategory {
    const CItem* start; const CItem* finish;
    int Get(int def) const
    {
        const CItem* it = start;
        for ( ; it != finish; ++it) if (it->IsSameItemTypeAsDefIndex(def)) break;
        if (it != finish) return it->NumberOfItems;
        return 0;
    }
};
int main()
{
    CItem v[3] = { { 4, 1 }, { 9, 7 }, { 9, 2 } };
    CCategory c; c.start = v; c.finish = v + 3;
    CHECK(c.Get(9) == 7);              // the first match wins
    CHECK(c.Get(4) == 1);
    CHECK(c.Get(5) == 0);
    c.finish = v; CHECK(c.Get(4) == 0);
    if (fails == 0) printf("005bde96_TEST PASS\n"); else printf("005bde96_TEST FAIL\n");
    return fails;
}