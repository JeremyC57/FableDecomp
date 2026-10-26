#include <cstdio>
// Behaviour test for NCreatureMode::CModeDragonStrafe::OnDeactivate (retail 0x008DC660): a state-1 dragon has its animation
// resumed; the mode goes inactive; the interface-0x3a component's flag bit is set again.
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
class CThing : public CThingLookup { public:
    unsigned char m_type;
    int suspends;
    bool lastSuspend;
    void SetSuspendAnimation(bool suspend, long reason) { ++suspends; lastSuspend = suspend; (void)reason; }
};
struct CTCFlagged : CTCBase {
    unsigned char m_flag : 1;
    unsigned char m_other : 7;
};
struct CModeBase {
    CThing* ParentThing;
    bool Active;
    void OnDeactivate() { Active = false; }
};
struct CModeDragonStrafe : CModeBase {
    void OnDeactivate()
    {
        if (ParentThing->m_type == 1)
            ParentThing->SetSuspendAnimation(false, 0);
        CModeBase::OnDeactivate();
        CTCFlagged* flagged;
        if (ParentThing->GetInterface(&flagged, 0x3a))
            flagged->m_flag = 1;
    }
};

int main()
{
    CTCFlagged flagged;
    flagged.m_flag = 0; flagged.m_other = 3;
    CThing dragon;
    dragon.ResetInterfaces();
    dragon.AddInterface(0x3a, &flagged);
    dragon.m_type = 1; dragon.suspends = 0; dragon.lastSuspend = true;
    CModeDragonStrafe m;
    m.ParentThing = &dragon; m.Active = true;
    m.OnDeactivate();
    int pass = !m.Active && dragon.suspends == 1 && !dragon.lastSuspend && flagged.m_flag == 1 && flagged.m_other == 3;
    dragon.m_type = 0;
    m.OnDeactivate();
    pass = pass && dragon.suspends == 1;
    if (!pass) { std::printf("008dc660_TEST FAIL\n"); return 1; }
    std::printf("008dc660_TEST PASS\n");
    return 0;
}