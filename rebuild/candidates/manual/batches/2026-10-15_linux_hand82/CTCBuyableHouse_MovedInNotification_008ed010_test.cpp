// Behaviour test for 008ed010 (catalog CTCBuyableHouse::MovedInNotification): true when an 8-byte possession entry is keyed by the thing.
#include <cstdio>

struct CThing { int id; };
struct SRandomAccessTag { SRandomAccessTag() {} };
struct SPossessionEntry { CThing* m_pThing; long m_count; };

SPossessionEntry* FindPossessionEntry(SPossessionEntry* first, SPossessionEntry* last, CThing* key, const SRandomAccessTag&)
{
    for (; first != last; ++first)
        if (first->m_pThing == key)
            return first;
    return last;
}

struct CTCPossessionList_008ed010
{
    SPossessionEntry* m_first;
    SPossessionEntry* m_last;
    bool ContainsThing(CThing* thing) const
    {
        SPossessionEntry* last = m_last;
        return FindPossessionEntry(m_first, last, thing, SRandomAccessTag()) != last ? true : false;
    }
};

int main()
{
    CThing a, b, c;
    SPossessionEntry entries[2]; entries[0].m_pThing = &a; entries[0].m_count = 1; entries[1].m_pThing = &b; entries[1].m_count = 3;
    CTCPossessionList_008ed010 list; list.m_first = entries; list.m_last = entries + 2;
    if (!list.ContainsThing(&b) || list.ContainsThing(&c)) { std::printf("008ed010_TEST FAIL\n"); return 1; }
    std::printf("008ed010_TEST PASS\n");
    return 0;
}
