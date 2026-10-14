#include <cstdio>
struct CTCBase { int x; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
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

struct CTCWill : CTCBase { unsigned char _pad[0xc]; unsigned char m_flag0c; };
struct CSeq { CThing* Creature; bool IsValid() { CThing* creature = Creature; if (creature->HasInterface(0x93)) return false; CTCWill* p; if (creature->GetInterface(&p, 0xce)) return p->m_flag0c != 0 ? true : false; return true; } };
int main()
{
    CThing t; InitThing(t); CSeq q; q.Creature = &t;
    if (!q.IsValid()) { std::printf("00909830_TEST FAIL\n"); return 1; }
    CTCWill w; w.m_flag0c = 0; t.Add(0xce, &w);
    if (q.IsValid()) { std::printf("00909830_TEST FAIL\n"); return 1; }
    w.m_flag0c = 3;
    if (!q.IsValid()) { std::printf("00909830_TEST FAIL\n"); return 1; }
    CTCBase s; t.Add(0x93, &s); if (q.IsValid()) { std::printf("00909830_TEST FAIL\n"); return 1; }

    std::printf("00909830_TEST PASS\n");
    return 0;
}