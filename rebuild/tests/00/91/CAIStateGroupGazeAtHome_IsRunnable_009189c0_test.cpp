// Behaviour test for CAIStateGroup_GazeAtHome::IsRunnable (0x009189C0): a scheduler that does not prefer
// this group vetoes it; otherwise it needs an owner and a free gaze position.
#include <cstdio>

class CTCBase { public: int dummy; };

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

class CThing { public:
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

class CAIBrain { public: CThing* PCreature; };
class CAIStateGroupBase;
static bool g_preferred = false;
class CTCIdleScheduler : public CTCBase { public:
    bool IsPreferredIdle(CAIStateGroupBase*) { return g_preferred; }
};

class CAIStateGroupBase { public:
    CAIBrain* PBrain;
    virtual ~CAIStateGroupBase() {}
    virtual void* GetOwner() { return 0; }
};

struct CAIStateGroup_GazeAtHome : CAIStateGroupBase {
    void* m_owner;
    void* m_gazePos;
    virtual void* GetOwner() { return m_owner; }
    void* FindFreeGazePos() { return m_gazePos; }
    bool IsRunnable()
    {
        CTCIdleScheduler* scheduler;
        if (PBrain->PCreature->GetInterface(&scheduler, 0xd3)) {
            if (!scheduler->IsPreferredIdle(this))
                return false;
        }
        return GetOwner() && FindFreeGazePos();
    }
};

int main()
{
    int dummy;
    CTCIdleScheduler sched;
    CInterfaceMap::SEntry e[1]; e[0].type = 0xd3; e[0].component = &sched;
    CThing creature; creature.m_interfaces.first = e; creature.m_interfaces.last = e + 1;
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_GazeAtHome g; g.PBrain = &brain; g.m_owner = &dummy; g.m_gazePos = &dummy;
    bool ok = true;
    ok = ok && g.IsRunnable() == true;                 // no scheduler: owner + gaze pos
    g.m_gazePos = 0; ok = ok && g.IsRunnable() == false;
    g.m_gazePos = &dummy; g.m_owner = 0; ok = ok && g.IsRunnable() == false;
    g.m_owner = &dummy;
    creature.SetBit(0xd3); g_preferred = false; ok = ok && g.IsRunnable() == false;
    g_preferred = true; ok = ok && g.IsRunnable() == true;
    if (!ok) { std::printf("009189c0_TEST FAIL\n"); return 1; }
    std::printf("009189c0_TEST PASS\n");
    return 0;
}