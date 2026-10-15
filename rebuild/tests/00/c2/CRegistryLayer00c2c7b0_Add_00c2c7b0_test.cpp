#include <cstdio>
struct CRegisteredItem { int a, b; };
struct CItemList { CRegisteredItem* items[4]; int n; void Add(CRegisteredItem* item) { items[n++] = item; } };
struct CRegistryParent {
    CRegisteredItem* last; bool fa, fb;
    CRegisteredItem* Add(CRegisteredItem* item, bool flagA, bool flagB) { last = item; fa = flagA; fb = flagB; return item; }
};
struct CRegistryLayer {
    CRegistryParent* m_pParent; CItemList m_items;
    CRegisteredItem* Add(CRegisteredItem* item, bool flagA, bool flagB)
    {
        m_pParent->Add(item, flagA, flagB);
        m_items.Add(item);
        return item;
    }
};
int main()
{
    CRegisteredItem x, y; CRegistryParent p; p.last = 0; CRegistryLayer l; l.m_pParent = &p; l.m_items.n = 0;
    if (l.Add(&x, true, false) != &x || p.last != &x || !p.fa || p.fb || l.m_items.n != 1) { std::printf("00c2c7b0_TEST FAIL\n"); return 1; }
    if (l.Add(&y, false, true) != &y || p.last != &y || p.fa || !p.fb || l.m_items.n != 2 || l.m_items.items[1] != &y) { std::printf("00c2c7b0_TEST FAIL\n"); return 1; }
    std::printf("00c2c7b0_TEST PASS\n");
    return 0;
}