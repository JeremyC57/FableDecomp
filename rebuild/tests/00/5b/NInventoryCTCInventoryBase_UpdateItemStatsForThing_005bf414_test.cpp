// Behaviour test for 005bf414: body copied from src/005bf414.cpp (CTCInventoryBase::UpdateItemStatsForThing), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

static int g_bitTests = 0; static bool g_present = true;
struct CPresenceBits { bool test(long bit) { ++g_bitTests; return g_present && bit == 0x1A; } };
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long wanted) { return m_present.test(wanted); }
};
struct CTCInventoryItem { long m_ident; long GetCategoryIdentifier() const { return m_ident; } };
static int g_updates = 0; static const CThing* g_updateThing = 0; static void* g_updateCategory = 0;
namespace NInventory {
struct CCategory {
    unsigned char _body[0x2C];
    void UpdateItemStatsForThing(const CThing& thing) { ++g_updates; g_updateThing = &thing; g_updateCategory = this; }
};
}
static NInventory::CCategory* g_findFirst = 0; static NInventory::CCategory* g_findLast = 0; static long g_findIdent = 0;
NInventory::CCategory* __fastcall FindCategory(NInventory::CCategory* first, NInventory::CCategory* last, long ident) { g_findFirst = first; g_findLast = last; g_findIdent = ident; return first + 1; }
struct CTCInventoryBase_Retail {
    unsigned char _pad_0x00[0x20];
    NInventory::CCategory* CategoriesFirst;
    NInventory::CCategory* CategoriesLast;
    NInventory::CCategory* CategoriesEnd;
    void UpdateItemStatsForThing(const CThing& thing);
};
void CTCInventoryBase_Retail::UpdateItemStatsForThing(const CThing& thing)
{
    CTCInventoryItem* item = 0;
    if (const_cast<CThing&>(thing).HasTC(0x1A))
        item = const_cast<CThing&>(thing).GetTC<CTCInventoryItem>(0x1A);
    long ident = item->GetCategoryIdentifier();
    FindCategory(CategoriesFirst, CategoriesLast, ident)->UpdateItemStatsForThing(thing);
}
int main()
{
    NInventory::CCategory cats[3];
    CTCInventoryBase_Retail inv; memset(&inv, 0, sizeof(inv)); inv.CategoriesFirst = cats; inv.CategoriesLast = cats + 3; inv.CategoriesEnd = cats + 3;
    CThing thing; memset(&thing, 0, sizeof(thing));
    CTCInventoryItem item; item.m_ident = 77;
    CInterfaceMap::SEntry entries[2] = { { 0x1A, &item }, { 0x30, (void*)0x99 } };
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    // the item component present (an inventory item thing always carries it): the category of its identifier is updated
    inv.UpdateItemStatsForThing(thing);
    CHECK(g_bitTests == 1 && g_findIdent == 77 && g_findFirst == cats && g_findLast == cats + 3);
    CHECK(g_updates == 1 && g_updateThing == &thing && g_updateCategory == cats + 1);
    // a map without the entry (bit set, entry missing) resolves to the end record: its component is what gets used
    CTCInventoryItem fallback; fallback.m_ident = 5;
    CInterfaceMap::SEntry entries2[2] = { { 0x30, (void*)0x99 }, { 0, &fallback } };
    thing.m_interfaces.first = entries2; thing.m_interfaces.last = entries2 + 1;
    inv.UpdateItemStatsForThing(thing);
    CHECK(g_bitTests == 2 && g_findIdent == 5 && g_updates == 2);
    if (fails == 0) printf("005bf414_TEST PASS\n"); else printf("005bf414_TEST FAIL\n");
    return fails;
}