#include <cstdio>
// Behaviour test for NCreatureMode::CModeDragonStrafe::OnActivate (retail 0x008DC600): base activation; a state-1 dragon has
// its animation suspended by this mode; the interface-0x3a component's flag bit is cleared.
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
    long lastReason;
    void SetSuspendAnimation(bool suspend, long reason) { ++suspends; lastSuspend = suspend; lastReason = reason; }
};
struct CTCFlagged : CTCBase {
    unsigned char m_flag : 1;
    unsigned char m_other : 7;
};
static int g_baseActivations;
struct CModeBase {
    CThing* ParentThing;
    void OnActivate() { ++g_baseActivations; }
};
struct CModeDragonStrafe : CModeBase {
    void OnActivate()
    {
        CModeBase::OnActivate();
        if (ParentThing->m_type == 1)
            ParentThing->SetSuspendAnimation(true, (long)this);
        CTCFlagged* flagged;
        if (ParentThing->GetInterface(&flagged, 0x3a))
            flagged->m_flag = 0;
    }
};

int main()
{
    CTCFlagged flagged;
    flagged.m_flag = 1; flagged.m_other = 5;
    CThing dragon;
    dragon.ResetInterfaces();
    dragon.AddInterface(0x3a, &flagged);
    dragon.m_type = 1; dragon.suspends = 0;
    CModeDragonStrafe m;
    m.ParentThing = &dragon;
    g_baseActivations = 0;
    m.OnActivate();
    int pass = g_baseActivations == 1 && dragon.suspends == 1 && dragon.lastSuspend && dragon.lastReason == (long)&m
        && flagged.m_flag == 0 && flagged.m_other == 5;
    dragon.m_type = 2;
    flagged.m_flag = 1;
    m.OnActivate();
    pass = pass && dragon.suspends == 1 && flagged.m_flag == 0;
    if (!pass) { std::printf("008dc600_TEST FAIL\n"); return 1; }
    std::printf("008dc600_TEST PASS\n");
    return 0;
}