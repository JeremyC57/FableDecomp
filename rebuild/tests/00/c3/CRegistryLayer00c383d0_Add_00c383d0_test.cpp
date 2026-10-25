// Behaviour test for 00c383d0: CRegistryLayer::Add -- forwards the item and both flags to the parent layer, records it in this layer's list and returns it
#include <cstdio>

struct CRegisteredItem { int m_id; };
struct CItemList {
    CRegisteredItem* m_last;
    void Add(CRegisteredItem* item) { m_last = item; }
};
struct CRegistryParent {
    CRegisteredItem* m_item; bool m_a, m_b;
    CRegisteredItem* Add(CRegisteredItem* item, bool a, bool b) { m_item = item; m_a = a; m_b = b; return item; }
};
struct CRegistryLayer {
    CRegistryParent* m_pParent;
    CItemList m_items;
    CRegisteredItem* Add(CRegisteredItem* item, bool flagA, bool flagB)
    {
        m_pParent->Add(item, flagA, flagB);
        m_items.Add(item);
        return item;
    }
};

int main()
{
    CRegisteredItem item = { 4 };
    CRegistryParent parent = { 0, false, false };
    CRegistryLayer layer = { &parent, { 0 } };
    if (layer.Add(&item, true, false) != &item || parent.m_item != &item || !parent.m_a || parent.m_b
        || layer.m_items.m_last != &item) { std::printf("00c383d0_TEST FAIL\n"); return 1; }
    std::printf("00c383d0_TEST PASS\n");
    return 0;
}