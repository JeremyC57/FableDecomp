// Behaviour test for CAIStateGroup_MazeTutorialBattleRespondToLightning::FindHitEvent (0x00954C60): no events
// component gives false; otherwise the result is HasEvent(type 0x1b created after FrameOfLastHit).
#include <cstdio>

struct CTCBase { int dummy; };

// Sorted {type, component} vector map searched by LowerBound (0x0040F020 in retail).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};

struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CThing() : m_flags91(0)
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = 0;
    }
    void SetBit(long t) { m_presentBits[t >> 5] |= 1UL << (t & 31); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CAIBrain { CThing* PCreature; };

namespace NEntityEvents {
    struct CIsEventOfTypeCreatedAfter {
        long Type, Frame;
        CIsEventOfTypeCreatedAfter(long type, long frame) : Type(type), Frame(frame) { }
    };
}
struct CTCEntityEvents : CTCBase {
    long m_eventType, m_eventFrame;
    bool HasEvent(const NEntityEvents::CIsEventOfTypeCreatedAfter& p) { return m_eventType == p.Type && m_eventFrame > p.Frame; }
};
struct CAIStateGroup_MazeTutorialBattleRespondToLightning {
    CAIBrain* PBrain;
    long FrameOfLastHit;
    bool FindHitEvent()
    {
        CTCEntityEvents* events;
        if (PBrain->PCreature->GetInterface(&events, 0x42)) {
            if (events->HasEvent(NEntityEvents::CIsEventOfTypeCreatedAfter(0x1b, FrameOfLastHit)))
                return true;
        }
        return false;
    }
};
int main()
{
    CTCEntityEvents ev; ev.m_eventType = 0x1b; ev.m_eventFrame = 100;
    CInterfaceMap::SEntry e[1]; e[0].type = 0x42; e[0].component = &ev;
    CThing creature; creature.m_interfaces.first = e; creature.m_interfaces.last = e + 1;
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_MazeTutorialBattleRespondToLightning g; g.PBrain = &brain; g.FrameOfLastHit = 50;
    bool ok = g.FindHitEvent() == false;              // no component
    creature.SetBit(0x42);
    ok = ok && g.FindHitEvent() == true;
    g.FrameOfLastHit = 100; ok = ok && g.FindHitEvent() == false;
    g.FrameOfLastHit = 50; ev.m_eventType = 3; ok = ok && g.FindHitEvent() == false;
    if (!ok) { std::printf("00954c60_TEST FAIL\n"); return 1; }
    std::printf("00954c60_TEST PASS\n");
    return 0;
}