// Behaviour test for 0x008BA970 CCreatureAction_CastWillHarvestSoulSpell::PerformAction: a live target is killed
// (Kill(true)) and the caster's interface-0xa7 component is notified once; a dead or missing target does nothing.
#include <cstdio>

struct CTCBase { int calls; void* arg; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
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
struct CTCSpecialAbilities : CTCBase { void OnSoulHarvested() { ++calls; } };
struct CCreatureAction_CastWillHarvestSoulSpell {
    CBaseIntelligentPointer PCreature, PTarget;
    void PerformAction()
    {
        if (IS_THING_ALIVE(PTarget)) {
            PTarget.GetPItem()->Kill(true);
            CTCSpecialAbilities* abilities;
            PCreature.GetPItem()->GetInterface(&abilities, 0xa7);
            abilities->OnSoulHarvested();
        }
    }
};
int main()
{
    CTCSpecialAbilities sa; sa.calls = 0; CTCBase other = { 0, 0 };
    CInterfaceMap::SEntry e[2] = { { 0x31, &other }, { 0xa7, &sa } };
    CThing caster; caster.Init(); caster.AddTC(e, 2, 0xa7);
    CThing target; target.Init();
    CCreatureAction_CastWillHarvestSoulSpell a; a.PCreature.m_pItem = &caster; a.PTarget.m_pItem = &target;
    a.PerformAction();
    bool ok = target.m_killed == 2 && sa.calls == 1;
    a.PerformAction();                      // now dead: nothing
    ok = ok && sa.calls == 1;
    a.PTarget.m_pItem = 0; a.PerformAction();
    ok = ok && sa.calls == 1 && other.calls == 0;
    if (!ok) { std::printf("008ba970_TEST FAIL\n"); return 1; }
    std::printf("008ba970_TEST PASS\n");
    return 0;
}
