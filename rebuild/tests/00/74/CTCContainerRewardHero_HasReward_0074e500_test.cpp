#include <cstdio>
struct CTCBase { long m_gold; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CTCGold : CTCBase { long GetGold() { return m_gold; } };
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool GetInterface(long type, CTCGold** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (CTCGold*)it->component;
            return true;
        }
        return false;
    }
};
struct SContainerItem { long first; void* second; };
struct CTCContainerRewardHero {
    CThing* ParentThing; SContainerItem* ItemsStart; SContainerItem* ItemsFinish;
    unsigned long ItemCount() const { return ItemsFinish - ItemsStart; }
    long HasReward()
    {
        long gold = 0;
        CTCGold* goldComponent;
        if (ParentThing->GetInterface(204, &goldComponent))
            gold = goldComponent->GetGold();
        if (ItemCount() > 0 || gold > 0)
            return 1;
        return 0;
    }
};
int main()
{
    CTCGold gold; gold.m_gold = 25;
    CInterfaceMap::SEntry entries[2] = { { 9, 0 }, { 204, &gold } };
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    SContainerItem items[2] = { { 1, 0 }, { 2, 0 } };
    CTCContainerRewardHero r; r.ParentThing = &thing; r.ItemsStart = items; r.ItemsFinish = items;
    if (r.HasReward() != 0) { std::printf("0074e500_TEST FAIL\n"); return 1; }   /* no gold interface, no items */
    thing.m_presentBits[6] = 0x1000;                                              /* interface 204 present */
    if (r.HasReward() != 1) { std::printf("0074e500_TEST FAIL\n"); return 1; }
    gold.m_gold = 0;
    if (r.HasReward() != 0) { std::printf("0074e500_TEST FAIL\n"); return 1; }
    r.ItemsFinish = items + 2;
    if (r.HasReward() != 1) { std::printf("0074e500_TEST FAIL\n"); return 1; }
    std::printf("0074e500_TEST PASS\n");
    return 0;
}