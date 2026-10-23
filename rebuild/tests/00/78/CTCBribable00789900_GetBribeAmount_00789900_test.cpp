#include <cstdio>
struct CTCBase { int x; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; unsigned char _pad1[0x45]; unsigned char m_flags91;
    bool HasInterface(long t) const { return (m_presentBits[t >> 5] & (1UL << (t & 31))) != 0; }
    template<class T> bool GetInterface(T** ppOut, long t)
    {
        if (m_presentBits[t >> 5] & (1UL << (t & 31))) { long type = t; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type); if (it == map->last || t < it->type) it = map->last; *ppOut = (T*)it->component; return true; }
        return false;
    }
    void Add(long t, CTCBase* c) { m_presentBits[t >> 5] |= 1UL << (t & 31); m_interfaces.last->type = t; m_interfaces.last->component = c; ++m_interfaces.last; }
};
static CInterfaceMap::SEntry g_entries[8];
static void InitThing(CThing& t) { for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_interfaces.first = g_entries; t.m_interfaces.last = g_entries; t.m_flags91 = 0; }

struct CTCGuard : CTCBase { long amount; long GetBribeAmountWillTake() { return amount; } };
struct CTC { virtual void v00() { } CThing* m_pThing; long m_08; long m_defaultBribeAmount; long GetBribeAmount() { long amount = m_defaultBribeAmount; CTCGuard* p; if (m_pThing->GetInterface(&p, 0xb4)) amount = p->GetBribeAmountWillTake(); return amount; } };
int main()
{
    CThing t; InitThing(t); CTC c; c.m_pThing = &t; c.m_defaultBribeAmount = 42;
    if (c.GetBribeAmount() != 42) { std::printf("00789900_TEST FAIL\n"); return 1; }
    CTCGuard g; g.amount = 99; t.Add(0xb4, &g);
    if (c.GetBribeAmount() != 99) { std::printf("00789900_TEST FAIL\n"); return 1; }
    std::printf("00789900_TEST PASS\n");
    return 0;
}