// Behaviour test for 0x0085afd0
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
struct CTCCameraPointScriptedSpline {
    long m_type; long m_anim; CThing* m_thing;
    virtual long GetCameraPointType() { return m_type; }
    bool IsCombatSplineValidForAnimAndThing(long animIndex, CThing* thing) { return animIndex == m_anim && thing == m_thing; }
};
struct CThingFilter_IsValidCombatSpline {
    long    AnimIndex;
    CThing* Thing;
    bool operator()(CThing* thing) const
    {
        CTCCameraPointScriptedSpline* spline;
        if (thing->GetInterface(0x3F, &spline) && spline->GetCameraPointType() == 7)
            return spline->IsCombatSplineValidForAnimAndThing(AnimIndex, Thing);
        return false;
    }
};
int main()
{
    CThing other;
    CTCCameraPointScriptedSpline s; s.m_type = 7; s.m_anim = 4; s.m_thing = &other;
    CInterfaceMap::SEntry e[1]; e[0].type = 0x3F; e[0].component = (CTCBase*)&s;
    CThing t; for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    t.m_interfaces.first = e; t.m_interfaces.last = e + 1;
    CThingFilter_IsValidCombatSpline f; f.AnimIndex = 4; f.Thing = &other;
    bool ok = !f(&t);
    t.m_presentBits[1] = 0x80000000UL;
    if (!f(&t)) ok = false;
    f.AnimIndex = 5; if (f(&t)) ok = false;
    f.AnimIndex = 4; s.m_type = 6; if (f(&t)) ok = false;
    if (!ok) { std::printf("0085afd0_TEST FAIL\n"); return 1; }
    std::printf("0085afd0_TEST PASS\n");
    return 0;
}
