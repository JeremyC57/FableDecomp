// Behaviour test for CTCQuickAccessMenu::GetItemsListUI (0x006276F0): a copy of the item sort over the inventory's (def, count)
// pairs -- expressions skipped, UI category 0..4 into trophies / potions / gifts / produce / others (unknown categories into
// others), numItems summed over the kept items, no list when every category is empty, otherwise five category entries in
// tab order with the usable-target check only on gifts.
#include <cstdio>
#include <vector>
#include <utility>

typedef std::vector<std::pair<long, long> > CItemVector;

struct Def { long id; bool expression; long category; };
static const Def g_defs[] = {
    { 10, false, 0 }, { 11, false, 1 }, { 12, false, 2 }, { 13, false, 3 }, { 14, false, 4 },
    { 15, true, 1 }, { 16, false, 9 }, { 17, false, 1 }
};
static const Def* FindDef(long id)
{
    for (unsigned i = 0; i < sizeof(g_defs) / sizeof(g_defs[0]); ++i)
        if (g_defs[i].id == id) return &g_defs[i];
    return 0;
}
static bool IsExpression(long id) { return FindDef(id)->expression; }
static long GetUIInventoryCategory(long id) { return FindDef(id)->category; }

struct Entry { int tab; long items; long total; bool checkUsable; };
struct Menu {
    std::vector<Entry> entries;
    bool listMade;
    void ConstructItemCategoryList(int tab, const CItemVector& items, bool checkUsable)
    {
        Entry e; e.tab = tab; e.items = (long)items.size(); e.total = 0; e.checkUsable = checkUsable;
        for (unsigned long i = 0; i < items.size(); ++i) e.total += items[i].second;
        entries.push_back(e);
    }
    bool GetItemsListUI(const CItemVector& items, long& numItems)
    {
        CItemVector trophies, potions, gifts, produce, others;
        numItems = 0;
        for (unsigned long i = 0; i < items.size(); ++i) {
            if (IsExpression(items[i].first))
                continue;
            switch (GetUIInventoryCategory(items[i].first)) {
            case 0: trophies.push_back(items[i]); break;
            case 1: potions.push_back(items[i]); break;
            case 2: gifts.push_back(items[i]); break;
            case 3: produce.push_back(items[i]); break;
            case 4: others.push_back(items[i]); break;
            default: others.push_back(items[i]); break;
            }
            numItems += items[i].second;
        }
        listMade = false;
        if (!trophies.empty() || !potions.empty() || !gifts.empty() || !produce.empty() || !others.empty()) {
            listMade = true;
            ConstructItemCategoryList(0, trophies, false);
            ConstructItemCategoryList(1, potions, false);
            ConstructItemCategoryList(2, gifts, true);
            ConstructItemCategoryList(3, produce, false);
            ConstructItemCategoryList(4, others, false);
        }
        return listMade;
    }
};

int main()
{
    int fails = 0;
    CItemVector items;
    items.push_back(std::make_pair(10L, 1L));
    items.push_back(std::make_pair(11L, 3L));
    items.push_back(std::make_pair(15L, 7L));   // expression: skipped, not counted
    items.push_back(std::make_pair(16L, 2L));   // unknown category: others
    items.push_back(std::make_pair(17L, 4L));
    items.push_back(std::make_pair(12L, 5L));
    Menu m;
    long n = -1;
    if (!m.GetItemsListUI(items, n)) ++fails;
    if (n != 15) ++fails;
    if (m.entries.size() != 5) ++fails;
    else {
        for (int t = 0; t < 5; ++t) if (m.entries[t].tab != t) ++fails;
        if (m.entries[0].items != 1 || m.entries[0].total != 1) ++fails;
        if (m.entries[1].items != 2 || m.entries[1].total != 7) ++fails;
        if (m.entries[2].items != 1 || !m.entries[2].checkUsable) ++fails;
        if (m.entries[3].items != 0 || m.entries[3].checkUsable) ++fails;
        if (m.entries[4].items != 1 || m.entries[4].total != 2) ++fails;
    }
    CItemVector onlyExpressions;
    onlyExpressions.push_back(std::make_pair(15L, 2L));
    Menu e;
    long n2 = -1;
    if (e.GetItemsListUI(onlyExpressions, n2) || n2 != 0 || !e.entries.empty()) ++fails;
    if (fails == 0) std::printf("006276f0_TEST PASS\n");
    else std::printf("006276f0_TEST FAIL %d\n", fails);
    return fails;
}