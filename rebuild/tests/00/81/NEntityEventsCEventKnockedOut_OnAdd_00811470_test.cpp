#include <cstdio>

class CTCBase { public: int dummy; };
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
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned short m_globalDefIndex;
    long GetGlobalDefIndex() const { return m_globalDefIndex; }
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
static int g_destroyed;
struct CDefPointee {
    long m_refCount;
    void Destroy() { ++g_destroyed; }
};
static __inline bool DecRefToZero(long& count) { return --count == 0; }
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->m_refCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
class CCreatureDef : public CDefPointee { public: long m_value40; };
static CCreatureDef g_creatureDef;
class CDefClassBase : public CDefPointee { public:
    bool GetSubDef(CDefPointer<CCreatureDef>* out) { out->Object = &g_creatureDef; ++g_creatureDef.m_refCount; return true; }
};
static CDefClassBase g_thingDef;
static long g_requestedIndex;
class CGameDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CDefClassBase>* out)
    { g_requestedIndex = globalIndex; out->Object = &g_thingDef; ++g_thingDef.m_refCount; return true; }
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
};
static long g_reported = -1; static int g_reportCount;
struct CStatEvent { long m_value; CStatEvent(long v) : m_value(v) {} };
class CTCHeroStats { public:
    void InformOfEvent(const CStatEvent* event) { g_reported = event->m_value; ++g_reportCount; }
};
struct CTCInterface_a0 { bool m_flag10; };
static bool g_ghost;
static bool IsThingGhostSword(const CThing*) { return g_ghost; }
class CTCEntityEvents { public:
    CThing* ParentThing;
    const CThing* GetThing() const { return ParentThing; }
};
struct CEventKill {
    CBaseIntelligentPointer m_pKiller;
    void OnAdd(const CTCEntityEvents& events)
    {
        if (m_pKiller.GetPItem() == 0) return;
        CTCHeroStats* stats;
        if (!m_pKiller.GetPItem()->GetInterface(&stats, 4)) return;
        CDefPointer<CDefClassBase> def;
        CGameDefinitionManager::Get().GetDef(events.GetThing()->GetGlobalDefIndex(), &def);
        CDefPointer<CCreatureDef> creatureDef;
        def->GetSubDef(&creatureDef);
        const CThing* thing = events.ParentThing;
        bool flagged = false;
        CTCInterface_a0* tc;
        if (thing->GetInterface(&tc, 0xa0) && tc->m_flag10) flagged = true;
        bool ghost = IsThingGhostSword(thing);
        if (!flagged && !ghost) {
            CStatEvent event(creatureDef->m_value40);
            stats->InformOfEvent(&event);
        }
    }
};

static CTCHeroStats g_stats;
static CTCInterface_a0 g_a0;
int Check()
{
    CThing hero = {}; CThing victim = {};
    CInterfaceMap::SEntry heroEntries[2] = { { 1, 0 }, { 4, (CTCBase*)&g_stats } };
    hero.m_interfaces.first = heroEntries; hero.m_interfaces.last = heroEntries + 2;
    hero.m_presentBits[0] = 0x10;                       // type 4 present
    CInterfaceMap::SEntry victimEntries[1] = { { 0xa0, (CTCBase*)&g_a0 } };
    victim.m_interfaces.first = victimEntries; victim.m_interfaces.last = victimEntries + 1;
    victim.m_globalDefIndex = 0x1234;
    g_creatureDef.m_refCount = 1; g_creatureDef.m_value40 = 77; g_thingDef.m_refCount = 1;
    CTCEntityEvents events; events.ParentThing = &victim;
    CEventKill ev; ev.m_pKiller.m_pItem = &hero;
    ev.OnAdd(events);
    if (g_reportCount != 1 || g_reported != 77 || g_requestedIndex != 0x1234 || g_creatureDef.m_refCount != 1 ||
        g_thingDef.m_refCount != 1 || g_destroyed != 0) return 1;
    victim.m_presentBits[5] = 1; g_a0.m_flag10 = true;   // type 0xa0 present and flagged: no report
    ev.OnAdd(events);
    if (g_reportCount != 1 || g_thingDef.m_refCount != 1) return 1;
    g_a0.m_flag10 = false; g_ghost = true;              // ghost sword: no report
    ev.OnAdd(events);
    if (g_reportCount != 1) return 1;
    g_ghost = false;
    ev.OnAdd(events);
    if (g_reportCount != 2) return 1;
    hero.m_presentBits[0] = 0; g_requestedIndex = 0;     // killer without hero stats: nothing fetched
    ev.OnAdd(events);
    if (g_reportCount != 2 || g_requestedIndex != 0) return 1;
    ev.m_pKiller.m_pItem = 0;
    ev.OnAdd(events);
    if (g_reportCount != 2) return 1;
    g_thingDef.m_refCount = 0;                           // the last reference is released: destroyed
    hero.m_presentBits[0] = 0x10; ev.m_pKiller.m_pItem = &hero;
    ev.OnAdd(events);
    if (g_destroyed != 1) return 1;
    return 0;
}

int main()
{
    if (Check()) { std::printf("00811470_TEST FAIL\n"); return 1; }
    std::printf("00811470_TEST PASS\n");
    return 0;
}