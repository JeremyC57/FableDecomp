// Behaviour test for 0x006A8B40 CThingFilter_HasFreeActionPoint::operator()
#include <cstdio>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const
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
struct CTCUserActionPointBase {
    bool m_free;
    bool IsActionPointFree() { return m_free; }
};
struct CThingFilter_HasFreeActionPoint {
    bool operator()(CThing* thing) const
    {
        CTCUserActionPointBase* actionPoint;
        return thing->GetInterface(0x2D, &actionPoint) && actionPoint->IsActionPointFree();
    }
};
int main()
{
    CTCUserActionPointBase ap; ap.m_free = true;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x2D; entries[1].component = (CTCBase*)&ap;
    CThing t;
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + 2;
    CThingFilter_HasFreeActionPoint f;
    bool ok = true;
    if (f(&t)) ok = false;                          // bit clear -> false
    t.m_presentBits[1] = 0x2000;
    if (!f(&t)) ok = false;                         // present + free
    ap.m_free = false;
    if (f(&t)) ok = false;                          // present + busy
    if (!ok) { std::printf("006a8b40_TEST FAIL\n"); return 1; }
    std::printf("006a8b40_TEST PASS\n");
    return 0;
}