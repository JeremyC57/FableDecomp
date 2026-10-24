#include <cstdio>
// Behaviour test for NCreatureMode::CModeCrouch::OnDeactivate (retail 0x008D8AB0): the interface-0x36 component's crouch
// flags are both cleared, and the mode goes inactive.
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
struct CTCInterface36 : CTCBase {
    bool m_crouching;
    bool m_flag10;
};
struct CModeBase {
    CThing* ParentThing;
    bool Active;
    void OnDeactivate() { Active = false; }
};
struct CModeCrouch : CModeBase {
    void OnDeactivate()
    {
        CTCInterface36* tc;
        if (ParentThing->GetInterface(&tc, 0x36)) {
            tc->m_crouching = false;
            tc->m_flag10 = false;
        }
        CModeBase::OnDeactivate();
    }
};

int main()
{
    CTCInterface36 stance;
    stance.m_crouching = true; stance.m_flag10 = true;
    CThing hero;
    hero.ResetInterfaces();
    hero.AddInterface(0x36, &stance);
    CModeCrouch m;
    m.ParentThing = &hero; m.Active = true;
    m.OnDeactivate();
    int pass = !stance.m_crouching && !stance.m_flag10 && !m.Active;
    hero.ResetInterfaces();
    stance.m_crouching = true; m.Active = true;
    m.OnDeactivate();
    pass = pass && stance.m_crouching && !m.Active;
    if (!pass) { std::printf("008d8ab0_TEST FAIL\n"); return 1; }
    std::printf("008d8ab0_TEST PASS\n");
    return 0;
}