#include <cstdio>
// Behaviour test for NCreatureMode::CModeDragonStrafeBurninate::OnDeactivate (retail 0x008DCB00): the type-9 component is
// switched off, a running flame beam is ended, then the flying-mode deactivation runs.
class CTCBase { public: int tag; };
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
class CThing : public CThingLookup { public: };
struct CTCType9 : CTCBase { bool m_active; };
static int g_flyingDeactivations;
struct CModeDragonFlying {
    CThing* ParentThing;
    void OnDeactivate() { ++g_flyingDeactivations; }
};
struct CModeDragonStrafeBurninate : CModeDragonFlying {
    bool FlameActive;
    int ends;
    void EndBeamEffect() { ++ends; FlameActive = false; }
    void OnDeactivate()
    {
        CTCType9* tc = 0;
        ParentThing->GetInterface(&tc, 9);
        tc->m_active = false;
        if (FlameActive)
            EndBeamEffect();
        CModeDragonFlying::OnDeactivate();
    }
};

int main()
{
    CTCType9 tc;
    tc.m_active = true;
    CThing dragon;
    dragon.ResetInterfaces();
    dragon.AddInterface(9, &tc);
    CModeDragonStrafeBurninate m;
    m.ParentThing = &dragon; m.FlameActive = true; m.ends = 0;
    g_flyingDeactivations = 0;
    m.OnDeactivate();
    int pass = !tc.m_active && m.ends == 1 && g_flyingDeactivations == 1;
    m.OnDeactivate();
    pass = pass && m.ends == 1 && g_flyingDeactivations == 2;
    if (!pass) { std::printf("008dcb00_TEST FAIL\n"); return 1; }
    std::printf("008dcb00_TEST PASS\n");
    return 0;
}