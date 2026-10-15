// Behaviour test for CAIStateGroup_GazeOutside::InitStateGroup (0x00918480): the gaze marker is cleared
// through a temporary intelligent pointer and, only when the creature has the idle scheduler interface
// (type 0xd3), the group registers itself as idle group 8.
#include <cstdio>

struct CTCBase { int dummy; };

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

struct CThingAICreature {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
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

struct CAIStateGroupBase;
static CAIStateGroupBase* g_registered = 0;
static long g_idleGroup = -1;

struct CTCIdleScheduler : CTCBase {
    void RegisterIdleStateGroup(CAIStateGroupBase* group, long idleGroup) { g_registered = group; g_idleGroup = idleGroup; }
};

struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    void* GetPItem() const { return m_pItem; }
    void SetPItem(void* item) { m_pItem = item; }
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CThingGameObject { int dummy; };
struct CWorld { int dummy; };

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature) {}
};

struct CAIStateGroup_Gaze : CAIStateGroupBase {
    CAIStateGroup_Gaze() : GazeMarker(0) {}
    CIntelligentPointer<CThingGameObject> GazeMarker;
};

struct CAIStateGroup_GazeOutside : CAIStateGroup_Gaze {
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature)
    {
        GazeMarker = CIntelligentPointer<CThingGameObject>(0);
        CTCIdleScheduler* scheduler;
        if (creature.GetInterface(&scheduler, 0xd3))
            scheduler->RegisterIdleStateGroup(this, 8);
    }
};

int main()
{
    CWorld world;
    CThingGameObject marker;
    CTCIdleScheduler sched;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x31; entries[0].component = 0;
    entries[1].type = 0xd3; entries[1].component = &sched;
    CThingAICreature creature;
    for (int i = 0; i < 9; ++i) creature.m_presentBits[i] = 0;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;

    CAIStateGroup_GazeOutside group;
    group.GazeMarker.SetPItem(&marker);
    group.InitStateGroup(world, creature);          // no scheduler bit: only the marker is cleared
    if (group.GazeMarker.GetPItem() != 0 || g_registered != 0) { std::printf("00918480_TEST FAIL\n"); return 1; }

    creature.m_presentBits[6] = 1UL << 19;          // 0xd3 = 6*32 + 19
    group.GazeMarker.SetPItem(&marker);
    group.InitStateGroup(world, creature);
    if (group.GazeMarker.GetPItem() != 0 || g_registered != &group || g_idleGroup != 8) { std::printf("00918480_TEST FAIL\n"); return 1; }
    std::printf("00918480_TEST PASS\n");
    return 0;
}