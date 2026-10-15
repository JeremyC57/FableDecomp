// Behaviour test for 0x008B79A0 CCreatureAction_GetUpFromSitting::PerformAction: the sitting mode is removed through the mode
// manager (interface 0x31) only when it is non-zero and RemoveOnPerformAction is RemoveOnPerformActionset.
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
struct CTCCreatureModeManager : CTCBase { void RemoveMode(unsigned long mode) { ++calls; arg = (void*)mode; } };
struct CCreatureAction_GetUpFromSitting {
    CBaseIntelligentPointer PCreature; long SittingMode; bool RemoveOnPerformAction; int m_baseInits;
    void PerformAction()
    {

        if (SittingMode != 0 && RemoveOnPerformAction) {
            CTCCreatureModeManager* modes;
            PCreature.GetPItem()->GetInterface(&modes, 0x31);
            modes->RemoveMode(SittingMode);
        }
    }
};
int main()
{
    CTCCreatureModeManager modes; modes.calls = 0; modes.arg = 0;
    CInterfaceMap::SEntry e[1] = { { 0x31, &modes } };
    CThing creature; creature.Init(); creature.AddTC(e, 1, 0x31);
    CCreatureAction_GetUpFromSitting a; a.PCreature.m_pItem = &creature; a.m_baseInits = 0;
    bool want = false;                      // the flag value that blocks the removal
    a.SittingMode = 0; a.RemoveOnPerformAction = !want; a.PerformAction();          // no mode: nothing
    bool ok = modes.calls == 0;
    a.SittingMode = 7; a.RemoveOnPerformAction = want; a.PerformAction();           // wrong flag: nothing
    ok = ok && modes.calls == 0;
    a.RemoveOnPerformAction = !want; a.PerformAction();                              // removes mode 7
    ok = ok && modes.calls == 1 && modes.arg == (void*)7;
    ok = ok && a.m_baseInits == 0;
    if (!ok) { std::printf("008b79a0_TEST FAIL\n"); return 1; }
    std::printf("008b79a0_TEST PASS\n");
    return 0;
}
