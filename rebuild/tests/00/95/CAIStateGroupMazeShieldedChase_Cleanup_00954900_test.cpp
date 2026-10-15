// Behaviour test for CAIStateGroup_MazeShieldedChase::Cleanup (0x00954900): the interface-8 component's target
// pointer is cleared through a temporary intelligent pointer.
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

struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    void* GetPItem() const { return m_pItem; }
    void SetPItem(void* item) { m_pItem = item; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }
};
struct CThingCreatureBase { int dummy; };
struct CTCTargeting : CTCBase {
    CTCTargeting() : m_target(0) {}
    CIntelligentPointer<CThingCreatureBase> m_target;
};
struct CAIStateGroup_MazeShieldedChase {
    CAIBrain* PBrain;
    void Cleanup()
    {
        CTCTargeting* targeting;
        PBrain->PCreature->GetInterface(&targeting, 8);
        targeting->m_target = CIntelligentPointer<CThingCreatureBase>(0);
    }
};
int main()
{
    CThingCreatureBase enemy; CTCTargeting t; t.m_target.SetPItem(&enemy);
    CInterfaceMap::SEntry e[2]; e[0].type = 3; e[0].component = 0; e[1].type = 8; e[1].component = &t;
    CThing creature; creature.m_interfaces.first = e; creature.m_interfaces.last = e + 2; creature.SetBit(8);
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_MazeShieldedChase g; g.PBrain = &brain;
    g.Cleanup();
    if (t.m_target.GetPItem() != 0) { std::printf("00954900_TEST FAIL\n"); return 1; }
    std::printf("00954900_TEST PASS\n");
    return 0;
}