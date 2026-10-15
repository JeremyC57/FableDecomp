// Behaviour test for 0x006a8450
#include <cstdio>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCInterface36 {
    unsigned char _pad[0x0c];
    bool m_flagA; bool m_flagB; bool m_flagC;
};
struct CThingFilter_IsPerceptible {
    bool operator()(CThing* thing) const
    {
        CTCInterface36* component;
        if (thing->GetInterface(0x36, &component))
            return component->m_flagA || component->m_flagB || component->m_flagC;
        return true;
    }
};
int main()
{
    CTCInterface36 c; c.m_flagA = c.m_flagB = c.m_flagC = false;
    CInterfaceMap::SEntry e[1]; e[0].type = 0x36; e[0].component = (CTCBase*)&c;
    CThing t; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = e; t.m_interfaces.last = e + 1;
    CThingFilter_IsPerceptible f;
    bool ok = f(&t);
    t.m_presentBits[1] = 0x400000;
    if (f(&t)) ok = false;
    c.m_flagC = true; if (!f(&t)) ok = false;
    c.m_flagC = false; c.m_flagB = true; if (!f(&t)) ok = false;
    if (!ok) { std::printf("006a8450_TEST FAIL\n"); return 1; }
    std::printf("006a8450_TEST PASS\n");
    return 0;
}