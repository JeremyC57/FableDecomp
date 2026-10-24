#include <cstdio>
// Behaviour test for NCreatureMode::CModeConversation::OnDeactivate (retail 0x008D93C0): the talk component (interface 15)
// gets the attitude it had when the conversation started back, and the mode goes inactive.
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
enum EConversationAttitude { ATTITUDE_NEUTRAL, ATTITUDE_HAPPY, ATTITUDE_ANGRY };
struct CTCTalk : CTCBase {
    EConversationAttitude Attitude;
    void SetAttitude(EConversationAttitude attitude) { Attitude = attitude; }
};
struct CModeBase {
    CThing* ParentThing;
    bool Active;
    void OnDeactivate() { Active = false; }
};
struct CModeConversation : CModeBase {
    EConversationAttitude InitialAttitude;
    void OnDeactivate()
    {
        CTCTalk* talk;
        if (ParentThing->GetInterface(&talk, 15))
            talk->SetAttitude(InitialAttitude);
        CModeBase::OnDeactivate();
    }
};

int main()
{
    CTCTalk talk;
    talk.Attitude = ATTITUDE_ANGRY;
    CThing villager;
    villager.ResetInterfaces();
    villager.AddInterface(3, 0);
    villager.AddInterface(15, &talk);
    CModeConversation m;
    m.ParentThing = &villager; m.Active = true; m.InitialAttitude = ATTITUDE_HAPPY;
    m.OnDeactivate();
    int pass = talk.Attitude == ATTITUDE_HAPPY && !m.Active;
    villager.ResetInterfaces();
    talk.Attitude = ATTITUDE_ANGRY; m.Active = true;
    m.OnDeactivate();
    pass = pass && talk.Attitude == ATTITUDE_ANGRY && !m.Active;
    if (!pass) { std::printf("008d93c0_TEST FAIL\n"); return 1; }
    std::printf("008d93c0_TEST PASS\n");
    return 0;
}