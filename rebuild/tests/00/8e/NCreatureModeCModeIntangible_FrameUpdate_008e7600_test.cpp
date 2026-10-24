#include <cstdio>
// Behaviour test for NCreatureMode::CModeIntangible::FrameUpdate (retail 0x008E7600): the mode tracks the boss phase
// (interface 0xad); when the phase changes, or a running timer reaches zero, the mode manager (interface 0x31) removes the
// intangible mode (0x51).
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
struct CTCBoss : CTCBase {
    long Phase;
    long GetBossPhase() const { return Phase; }
};
struct CTCCreatureModeManager : CTCBase {
    int removals;
    long removed;
    void RemoveMode(long type) { ++removals; removed = type; }
};
struct CModeIntangible {
    CThing* ParentThing;
    long Timer;
    long BossPhase;
    void FrameUpdate()
    {
        bool changed = false;
        if (ParentThing->HasTC(0xad)) {
            long phase = ParentThing->GetTC<CTCBoss>(0xad)->GetBossPhase();
            if (phase != BossPhase) {
                changed = true;
                BossPhase = phase;
            }
        }
        if ((Timer > 0 && --Timer == 0) || changed) {
            CTCCreatureModeManager* modes = 0;
            ParentThing->GetInterface(&modes, 0x31);
            modes->RemoveMode(0x51);
        }
    }
};

int main()
{
    CTCBoss boss;
    boss.Phase = 1;
    CTCCreatureModeManager modes;
    modes.removals = 0; modes.removed = 0;
    CThing jack;
    jack.ResetInterfaces();
    jack.AddInterface(0xad, &boss);
    jack.AddInterface(0x31, &modes);
    CModeIntangible m;
    m.ParentThing = &jack; m.Timer = 2; m.BossPhase = 1;
    m.FrameUpdate();
    int pass = m.Timer == 1 && modes.removals == 0;
    m.FrameUpdate();
    pass = pass && m.Timer == 0 && modes.removals == 1 && modes.removed == 0x51;
    m.FrameUpdate();
    pass = pass && m.Timer == 0 && modes.removals == 1;
    boss.Phase = 2;
    m.FrameUpdate();
    pass = pass && m.BossPhase == 2 && modes.removals == 2;
    m.FrameUpdate();
    pass = pass && modes.removals == 2;
    if (!pass) { std::printf("008e7600_TEST FAIL\n"); return 1; }
    std::printf("008e7600_TEST PASS\n");
    return 0;
}