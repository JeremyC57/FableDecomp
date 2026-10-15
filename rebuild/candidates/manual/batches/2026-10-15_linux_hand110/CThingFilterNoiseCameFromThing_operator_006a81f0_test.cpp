// Behaviour test for 0x006a81f0
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
struct CTCInterface37 {
    CThing* m_src;
    CThing* GetSourceThing() { return m_src; }
};
struct CThingFilter_NoiseCameFromThing {
    CThing* Thing;
    bool operator()(CThing* thing) const
    {
        CTCInterface37* component;
        if (thing->GetInterface(0x37, &component) && component->GetSourceThing() == Thing)
            return true;
        return false;
    }
};
int main()
{
    CThing src;
    CTCInterface37 c; c.m_src = &src;
    CInterfaceMap::SEntry e[1]; e[0].type = 0x37; e[0].component = (CTCBase*)&c;
    CThing t; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = e; t.m_interfaces.last = e + 1;
    CThingFilter_NoiseCameFromThing f; f.Thing = &src;
    bool ok = !f(&t);
    t.m_presentBits[1] = 0x800000;
    if (!f(&t)) ok = false;
    f.Thing = &t; if (f(&t)) ok = false;
    if (!ok) { std::printf("006a81f0_TEST FAIL\n"); return 1; }
    std::printf("006a81f0_TEST PASS\n");
    return 0;
}
