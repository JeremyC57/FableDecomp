#include <cstdio>
// Behaviour test for NCreatureMode::CModeScreamerAggressive::FrameUpdate (retail 0x008DA610): while the target is alive, a
// screamer with a look component (interface 0x43) looks at the target's position (priority 0x67); then the base update.
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
class C3DVector { public: float X, Y, Z; };
class CThing : public CThingLookup { public:
    unsigned char m_flags91;
    C3DVector Position;
    const C3DVector& GetPosition() const { return Position; }
};
class CTCLook : public CTCBase { public:
    const C3DVector* At;
    long Priority;
    void LookAtPos(const C3DVector* position, long priority) { At = position; Priority = priority; }
};
struct CBaseIntelligentPointer {
    CThing* Item;
    CThing* GetPItem() const { return Item; }
};
static int g_baseUpdates;
struct CModeBase {
    CThing* ParentThing;
    void FrameUpdate() { ++g_baseUpdates; }
};
struct CModeScreamerAggressive : CModeBase {
    CBaseIntelligentPointer PTarget;
    void FrameUpdate()
    {
        CTCLook* look;
        if (PTarget.GetPItem() != 0 && !(PTarget.GetPItem()->m_flags91 & 1) && ParentThing->GetInterface(&look, 0x43))
            look->LookAtPos(&PTarget.GetPItem()->GetPosition(), 0x67);
        CModeBase::FrameUpdate();
    }
};

int main()
{
    CTCLook look;
    look.At = 0; look.Priority = 0;
    CThing screamer;
    screamer.ResetInterfaces();
    screamer.AddInterface(0x43, &look);
    screamer.m_flags91 = 0;
    CThing hero;
    hero.ResetInterfaces();
    hero.m_flags91 = 0;
    CModeScreamerAggressive m;
    m.ParentThing = &screamer; m.PTarget.Item = &hero;
    g_baseUpdates = 0;
    m.FrameUpdate();
    int pass = look.At == &hero.Position && look.Priority == 0x67;
    look.At = 0;
    hero.m_flags91 = 1;
    m.FrameUpdate();
    pass = pass && look.At == 0;
    hero.m_flags91 = 0;
    screamer.ResetInterfaces();
    m.FrameUpdate();
    pass = pass && look.At == 0;
    m.PTarget.Item = 0;
    m.FrameUpdate();
    pass = pass && look.At == 0 && g_baseUpdates == 4;
    if (!pass) { std::printf("008da610_TEST FAIL\n"); return 1; }
    std::printf("008da610_TEST PASS\n");
    return 0;
}