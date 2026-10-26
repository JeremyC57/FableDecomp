// Behaviour test for 00747450 (catalog CTCBuyableHouse::MovedInNotification): true when the thing is held by one of the 16-byte entries.
#include <cstdio>

class CThing { public: int id; };
struct SRandomAccessTag { SRandomAccessTag() {} };

struct SThingEntry { CThing* m_pThing; long a, b; };

SThingEntry* FindThingEntry(SThingEntry* first, SThingEntry* last, CThing* key, const SRandomAccessTag&)
{
    for (; first != last; ++first)
        if (first->m_pThing == key)
            return first;
    return last;
}

inline SThingEntry* FindThing(SThingEntry* first, SThingEntry* last, CThing* const& key)
{
    return FindThingEntry(first, last, key, SRandomAccessTag());
}

struct CTCThingList_00747450
{
    SThingEntry* m_first;
    SThingEntry* m_last;
    bool ContainsThing(CThing* thing) const { return FindThing(m_first, m_last, thing) != m_last; }
};

int main()
{
    CThing a, b, c;
    SThingEntry entries[2]; entries[0].m_pThing = &a; entries[1].m_pThing = &b;
    CTCThingList_00747450 list; list.m_first = entries; list.m_last = entries + 2;
    if (!list.ContainsThing(&a) || !list.ContainsThing(&b) || list.ContainsThing(&c)) { std::printf("00747450_TEST FAIL\n"); return 1; }
    list.m_last = entries;
    if (list.ContainsThing(&a)) { std::printf("00747450_TEST FAIL\n"); return 1; }
    std::printf("00747450_TEST PASS\n");
    return 0;
}