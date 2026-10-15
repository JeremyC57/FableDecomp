// Behaviour test for 0x00848D10 CCreatureAction_DropGenericBox::PerformAction: the carrying component (0x46) drops its
// primary-slot thing (argument true) and the mode manager (0x31) removes mode 0x22.
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
struct CTCCarrying : CTCBase { void RemoveThingFromPrimarySlot(bool drop) { ++calls; arg = drop ? (void*)1 : (void*)0; } };
struct CTCCreatureModeManager : CTCBase { void RemoveMode(unsigned long mode) { ++calls; arg = (void*)mode; } };
struct CCreatureAction_DropGenericBox {
    CBaseIntelligentPointer PCreature;
    void PerformAction()
    {
        CTCCarrying* carrying;
        PCreature.GetPItem()->GetInterface(&carrying, 0x46);
        carrying->RemoveThingFromPrimarySlot(true);
        CTCCreatureModeManager* modes;
        PCreature.GetPItem()->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x22);
    }
};
int main()
{
    CTCCarrying carry; carry.calls = 0; carry.arg = 0;
    CTCCreatureModeManager modes; modes.calls = 0; modes.arg = 0;
    CInterfaceMap::SEntry e[2] = { { 0x31, &modes }, { 0x46, &carry } };
    CThing creature; creature.Init(); creature.AddTC(e, 2, 0x46); creature.AddTC(e, 2, 0x31);
    CCreatureAction_DropGenericBox a; a.PCreature.m_pItem = &creature;
    a.PerformAction();
    if (carry.calls != 1 || carry.arg != (void*)1 || modes.calls != 1 || modes.arg != (void*)0x22) {
        std::printf("00848d10_TEST FAIL\n"); return 1;
    }
    std::printf("00848d10_TEST PASS\n");
    return 0;
}
