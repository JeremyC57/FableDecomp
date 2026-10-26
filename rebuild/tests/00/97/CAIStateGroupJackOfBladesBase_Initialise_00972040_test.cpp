// Behaviour test for CAIStateGroup_JackOfBladesBase::Initialise (0x00972040): the base initialisation gets
// the five arguments unchanged, then the creature's boss component goes back to phase 0 (not immediate)
// and its counter at +0x10 is cleared.
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

class CCharString { public: const char* m_pData; };
enum EScriptAIPriority { PRIORITY_A, PRIORITY_B };
class CTCBoss : public CTCBase { public:
    long m_phaseCounter;
    long m_phase; bool m_immediate;
    void SetNextBossPhase(long phase, bool immediate) { m_phase = phase; m_immediate = immediate; }
};
static bool g_baseOk = false;
class CAIStateGroupBase { public:
    CAIBrain* PBrain;
    virtual ~CAIStateGroupBase() {}
    virtual void Initialise(CAIBrain& brain, CThing& creature, long scriptPriority, EScriptAIPriority priority,
        const CCharString& name)
    {
        PBrain = &brain;
        g_baseOk = brain.PCreature == &creature && scriptPriority == 7 && priority == PRIORITY_B && name.m_pData[0] == 'J';
    }
};
struct CAIStateGroup_JackOfBladesBase : CAIStateGroupBase {
    virtual void Initialise(CAIBrain& brain, CThing& creature, long scriptPriority, EScriptAIPriority priority,
        const CCharString& name)
    {
        CAIStateGroupBase::Initialise(brain, creature, scriptPriority, priority, name);
        CTCBoss* boss;
        PBrain->PCreature->GetInterface(&boss, 0xad);
        boss->SetNextBossPhase(0, false);
        boss->m_phaseCounter = 0;
    }
};

int main()
{
    CTCBoss boss; boss.m_phaseCounter = 9; boss.m_phase = 4; boss.m_immediate = true;
    CInterfaceMap::SEntry e[1]; e[0].type = 0xad; e[0].component = &boss;
    CThing creature; creature.m_interfaces.first = e; creature.m_interfaces.last = e + 1; creature.SetBit(0xad);
    CAIBrain brain; brain.PCreature = &creature;
    CCharString name; name.m_pData = "Jack";
    CAIStateGroup_JackOfBladesBase g; g.PBrain = 0;
    CAIStateGroupBase* base = &g;
    base->Initialise(brain, creature, 7, PRIORITY_B, name);
    if (!g_baseOk || boss.m_phase != 0 || boss.m_immediate || boss.m_phaseCounter != 0) { std::printf("00972040_TEST FAIL\n"); return 1; }
    std::printf("00972040_TEST PASS\n");
    return 0;
}