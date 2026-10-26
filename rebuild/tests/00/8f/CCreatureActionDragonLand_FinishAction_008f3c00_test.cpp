// Behaviour test for 0x008F3C00 CCreatureAction_DragonLand::FinishAction: the interface-0x3a component (when present)
// gets bit 0 of its flag byte set; a creature-type thing (type byte 1) has its animation suspension released with
// this action as requester; the base FinishAction always runs.
#include <cstdio>

class CTCBase { public: int calls; void* arg; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    int m_killed;
    void Kill(bool removeNow) { m_killed = removeNow ? 2 : 1; m_flags91 |= 1; }
    template <class T> void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
    void Init()
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = 0; m_flags91 = 0; m_killed = 0;
    }
    void AddTC(CInterfaceMap::SEntry* entries, int n, long type)
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        m_interfaces.first = entries; m_interfaces.last = entries + n;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCInterface3A : CTCBase { unsigned char m_flags50; };
struct CCreature : CThing {
    unsigned char m_type; int m_suspendCalls; bool m_suspend; const void* m_requester;
    void SetSuspendAnimation(bool s, const void* r) { ++m_suspendCalls; m_suspend = s; m_requester = r; }
};
class CCreatureAction_DragonLand { public:
    CCreature* m_creature; int m_baseFinishes;
    void FinishAction()
    {
        CThing* c = m_creature;
        if (c->m_presentBits[0x3a >> 5] & (1UL << (0x3a & 31))) {
            CTCInterface3A* tc; c->GetInterface(&tc, 0x3a);
            tc->m_flags50 |= 1;
        }
        if (m_creature->m_type == 1) {
            CCreature* creature = m_creature;
            creature->SetSuspendAnimation(false, this);
        }
        ++m_baseFinishes;
    }
};
int main()
{
    CTCInterface3A tc; tc.calls = 0; tc.m_flags50 = 0x80;
    CInterfaceMap::SEntry e[1] = { { 0x3a, &tc } };
    CCreature dragon; dragon.Init(); dragon.m_type = 2; dragon.m_suspendCalls = 0; dragon.m_suspend = true; dragon.m_requester = 0;
    CCreatureAction_DragonLand a; a.m_creature = &dragon; a.m_baseFinishes = 0;
    a.FinishAction();                                   // no component, not a creature
    bool ok = tc.m_flags50 == 0x80 && dragon.m_suspendCalls == 0 && a.m_baseFinishes == 1;
    dragon.AddTC(e, 1, 0x3a); dragon.m_type = 1; a.FinishAction();
    ok = ok && tc.m_flags50 == 0x81 && dragon.m_suspendCalls == 1 && !dragon.m_suspend && dragon.m_requester == &a &&
         a.m_baseFinishes == 2;
    if (!ok) { std::printf("008f3c00_TEST FAIL\n"); return 1; }
    std::printf("008f3c00_TEST PASS\n");
    return 0;
}