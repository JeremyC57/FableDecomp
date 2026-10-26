#include <cstdio>
// Behaviour test for NCreatureMode::CModeBriarRoseConcentrate::FrameUpdate (retail 0x008E8760): unless she was hit, a
// real (non-imposter) Briar Rose counts the concentrate timer down; at zero she tries to start the concentrate-out-of
// action at a live target, and when that cannot start (or she was hit) the mode removes itself (mode 0x3f).
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
    unsigned char m_flags91;
};
class CCreatureActionBase { public: CThing* Target; };
static int g_accept;
static CThing* g_startedAt;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action)
    {
        if (!g_accept) return false;
        g_startedAt = action.Target;
        return true;
    }
};
class CCreatureAction_BriarRoseConcentrateOutof : public CCreatureActionBase { public:
    CCreatureAction_BriarRoseConcentrateOutof(CThingCreatureBase* owner, CThing* target) { Target = target; (void)owner; }
};
class CTCCreatureModeManager : public CTCBase { public:
    long removed;
    void RemoveMode(long type) { removed = type; }
};
struct CBaseIntelligentPointer {
    CThing* Item;
    CThing* GetPItem() const { return Item; }
};
static int g_baseUpdates;
struct CModeBase {
    CThingCreatureBase* ParentThing;
    void FrameUpdate() { ++g_baseUpdates; }
};
struct CModeBriarRoseConcentrate : CModeBase {
    bool hit;
    bool Imposter;
    long Timer;
    CBaseIntelligentPointer PTarget;
    bool FindHitEvent() { return hit; }
    void FrameUpdate()
    {
        CModeBase::FrameUpdate();
        if (!FindHitEvent()) {
            if (Imposter)
                return;
            if (--Timer > 0)
                return;
            if (PTarget.GetPItem() != 0 && !(PTarget.GetPItem()->m_flags91 & 1)) {
                CThingCreatureBase* owner = ParentThing;
                bool started = owner->SetCurrentAction(CCreatureAction_BriarRoseConcentrateOutof(owner, PTarget.GetPItem()));
                if (started)
                    return;
            }
        }
        CTCCreatureModeManager* modes;
        if (ParentThing->GetInterface(&modes, 0x31))
            modes->RemoveMode(0x3f);
    }
};

int main()
{
    CTCCreatureModeManager modes;
    modes.removed = 0;
    CThingCreatureBase rose;
    rose.ResetInterfaces();
    rose.AddInterface(0x31, &modes);
    rose.m_flags91 = 0;
    CThing target;
    target.ResetInterfaces();
    target.m_flags91 = 0;
    CModeBriarRoseConcentrate m;
    m.ParentThing = &rose; m.hit = false; m.Imposter = false; m.Timer = 2; m.PTarget.Item = &target;
    g_accept = 1; g_startedAt = 0; g_baseUpdates = 0;
    m.FrameUpdate();
    int pass = m.Timer == 1 && g_startedAt == 0 && modes.removed == 0 && g_baseUpdates == 1;
    m.FrameUpdate();
    pass = pass && m.Timer == 0 && g_startedAt == &target && modes.removed == 0;
    g_accept = 0; g_startedAt = 0; m.Timer = 1;
    m.FrameUpdate();
    pass = pass && g_startedAt == 0 && modes.removed == 0x3f;
    modes.removed = 0; m.hit = true; m.Timer = 5;
    m.FrameUpdate();
    pass = pass && m.Timer == 5 && modes.removed == 0x3f;
    modes.removed = 0; m.hit = false; m.Imposter = true; m.Timer = 1;
    m.FrameUpdate();
    pass = pass && m.Timer == 1 && modes.removed == 0;
    m.Imposter = false; target.m_flags91 = 1; g_accept = 1;
    m.FrameUpdate();
    pass = pass && g_startedAt == 0 && modes.removed == 0x3f && g_baseUpdates == 6;
    if (!pass) { std::printf("008e8760_TEST FAIL\n"); return 1; }
    std::printf("008e8760_TEST PASS\n");
    return 0;
}