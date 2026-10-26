// Behaviour test for CTCActionUseAcceptBoast::vfunc_28 @ 0x007EBBC0: after the (empty) base slot, the boasting position (interface 0x71) is told which creature accepted.
#include <cstdio>

class CTCBase { public: int unused; };

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

class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
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

// A thing carrying components of types 0x10, TYPE and 0x90 (sorted), with TYPE's presence bit set.
struct CTestThing : CThing {
    CInterfaceMap::SEntry entries[4];
    void Init(long type, CTCBase* component, CTCBase* sentinel)
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_flags91 = 0;
        entries[0].type = 0x10; entries[0].component = 0;
        entries[1].type = type;  entries[1].component = component;
        entries[2].type = 0x90; entries[2].component = 0;
        entries[3].type = 0x7fffffff; entries[3].component = sentinel;   // the map's end slot
        m_interfaces.first = entries;
        m_interfaces.last = entries + 3;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
};

class CThingCreatureBase { public: int unused; };
static int g_baseCalls = 0;

class CTCBoastingPosition : public CTCBase { public:
    CThingCreatureBase* accepted; int calls;
    void OnBoastAccepted(CThingCreatureBase* creature) { accepted = creature; ++calls; }
};

class CTCActionUseBase { public:
    CThing* ParentThing;
    void vfunc_28(CThingCreatureBase*) { ++g_baseCalls; }
};

struct CTCActionUseAcceptBoast : CTCActionUseBase {
    void vfunc_28(CThingCreatureBase* creature)
    {
        CTCActionUseBase::vfunc_28(creature);
        CTCBoastingPosition* position;
        ParentThing->GetInterface(&position, 0x71);
        position->OnBoastAccepted(creature);
    }
};

int main()
{
    CTCBoastingPosition position; position.accepted = 0; position.calls = 0;
    CTCBoastingPosition other; other.accepted = 0; other.calls = 0;
    CTestThing thing; thing.Init(0x71, &position, &other);
    CThingCreatureBase hero;
    CTCActionUseAcceptBoast use; use.ParentThing = &thing;
    use.vfunc_28(&hero);
    bool ok = g_baseCalls == 1 && position.accepted == &hero && position.calls == 1 && other.calls == 0;
    if (!ok) { std::printf("007ebbc0_TEST FAIL\n"); return 1; }
    std::printf("007ebbc0_TEST PASS\n");
    return 0;
}