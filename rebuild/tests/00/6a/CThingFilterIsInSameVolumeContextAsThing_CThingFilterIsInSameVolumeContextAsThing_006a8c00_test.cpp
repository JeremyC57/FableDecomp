// Behaviour test for 0x006a8c00
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
struct CBaseIntelligentPointer {
    void*   vt;
    CThing* PItem;
    CThing* GetPItem() const { return PItem; }
};
struct CTCInterface75 {
    unsigned char _pad[0x0c];
    CBaseIntelligentPointer m_inside;
};
struct CThingFilter_IsInSameVolumeContextAsThing {
    CThing* PThingInside;
    bool    Override;
    CThingFilter_IsInSameVolumeContextAsThing(CThing* thing, bool override)
        : Override(override)
    {
        PThingInside = 0;
        CTCInterface75* component;
        if (thing->GetInterface(0x75, &component))
            PThingInside = component->m_inside.GetPItem();
    }
};
int main()
{
    CThing building;
    CTCInterface75 c; c.m_inside.vt = 0; c.m_inside.PItem = &building;
    CInterfaceMap::SEntry e[1]; e[0].type = 0x75; e[0].component = (CTCBase*)&c;
    CThing t; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = e; t.m_interfaces.last = e + 1;
    bool ok = true;
    CThingFilter_IsInSameVolumeContextAsThing a(&t, true);
    if (a.PThingInside != 0 || !a.Override) ok = false;
    t.m_presentBits[3] = 0x200000;
    CThingFilter_IsInSameVolumeContextAsThing b(&t, false);
    if (b.PThingInside != &building || b.Override) ok = false;
    if (!ok) { std::printf("006a8c00_TEST FAIL\n"); return 1; }
    std::printf("006a8c00_TEST PASS\n");
    return 0;
}