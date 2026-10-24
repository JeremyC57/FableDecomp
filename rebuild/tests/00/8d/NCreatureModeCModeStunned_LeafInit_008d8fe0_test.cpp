#include <cstdio>
// Behaviour test for NCreatureMode::CModeStunned::LeafInit (retail 0x008D8FE0): the stunned creature's targeting component
// (interface 9) gets flags 4, and its combat component (interface 0x30) takes it out of combat.
struct CTCBase { int tag; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThingLookup {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry m_store[9];
    void ResetInterfaces()
    {
        unsigned int w = 0;
        while (w < 9) { m_presentBits[w] = 0; m_store[w].type = 0x7fff; m_store[w].component = 0; ++w; }
        m_interfaces.first = m_store;
        m_interfaces.last = m_store;
    }
    void AddInterface(long type, CTCBase* component)
    {
        CInterfaceMap::SEntry* it = m_interfaces.last;
        while (it != m_interfaces.first && (it - 1)->type > type) { *it = *(it - 1); --it; }
        it->type = type;
        it->component = component;
        ++m_interfaces.last;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = GetTC<T>(interfaceType);
            return true;
        }
        return false;
    }
};
struct CThing : CThingLookup { };
struct CTCTargetedFlags : CTCBase {
    unsigned long m_flags;
    void SetFlags(unsigned long flags) { m_flags = flags; }
};
struct CTCCombat : CTCBase {
    int removals;
    void RemoveFromCombat() { ++removals; }
};
struct CModeStunned {
    CThing* ParentThing;
    void LeafInit()
    {
        CTCTargetedFlags* targeted;
        if (ParentThing->GetInterface(&targeted, 9))
            targeted->SetFlags(4);
        if (ParentThing->HasTC(0x30))
            ParentThing->GetTC<CTCCombat>(0x30)->RemoveFromCombat();
    }
};

int main()
{
    CTCTargetedFlags targeted;
    targeted.m_flags = 0x13;
    CTCCombat combat;
    combat.removals = 0;
    CThing bandit;
    bandit.ResetInterfaces();
    bandit.AddInterface(0x30, &combat);
    bandit.AddInterface(9, &targeted);
    CModeStunned m;
    m.ParentThing = &bandit;
    m.LeafInit();
    int pass = targeted.m_flags == 4 && combat.removals == 1;
    bandit.ResetInterfaces();
    targeted.m_flags = 1;
    m.LeafInit();
    pass = pass && targeted.m_flags == 1 && combat.removals == 1;
    if (!pass) { std::printf("008d8fe0_TEST FAIL\n"); return 1; }
    std::printf("008d8fe0_TEST PASS\n");
    return 0;
}