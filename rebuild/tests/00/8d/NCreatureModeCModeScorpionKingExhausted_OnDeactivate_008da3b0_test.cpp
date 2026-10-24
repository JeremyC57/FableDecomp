#include <cstdio>
// Behaviour test for NCreatureMode::CModeScorpionKingExhausted::OnDeactivate (retail 0x008DA3B0): the type-9 component is
// switched on, the scorpion's physics woken, creature flag 1 set and the mode marked inactive.
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
struct CPhysicsObject {
    int wakes;
    void Wake() { ++wakes; }
};
struct CThing : CThingLookup {
    CPhysicsObject* PPhysics;
    unsigned char CreatureFlags;
};
struct CTCType9 : CTCBase { bool m_active; };
struct CModeBase {
    CThing* ParentThing;
    bool Active;
    void OnDeactivate() { Active = false; }
};
struct CModeScorpionKingExhausted : CModeBase {
    void OnDeactivate()
    {
        CTCType9* tc = 0;
        if (ParentThing->HasTC(9))
            tc = ParentThing->GetTC<CTCType9>(9);
        tc->m_active = true;
        ParentThing->PPhysics->Wake();
        ParentThing->CreatureFlags |= 1;
        CModeBase::OnDeactivate();
    }
};

int main()
{
    CTCType9 tc;
    tc.m_active = false;
    CPhysicsObject body;
    body.wakes = 0;
    CThing scorpion;
    scorpion.ResetInterfaces();
    scorpion.AddInterface(9, &tc);
    scorpion.PPhysics = &body;
    scorpion.CreatureFlags = 0x40;
    CModeScorpionKingExhausted m;
    m.ParentThing = &scorpion; m.Active = true;
    m.OnDeactivate();
    if (!tc.m_active || body.wakes != 1 || scorpion.CreatureFlags != 0x41 || m.Active) {
        std::printf("008da3b0_TEST FAIL\n");
        return 1;
    }
    std::printf("008da3b0_TEST PASS\n");
    return 0;
}