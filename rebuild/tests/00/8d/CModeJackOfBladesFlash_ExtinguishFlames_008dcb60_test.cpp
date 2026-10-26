// Behaviour test for 008dcb60: body copied from src/008dcb60.cpp (CModeJackOfBladesFlash::ExtinguishFlames), callees stubbed.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_frees = 0; static void* g_lastFreed = 0;
static void myfree(void* p) { ++g_frees; g_lastFreed = p; free(p); }
class CTCBase { public: };
class CThing;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type)
    {
        SEntry* it = first;
        while (it != last && it->type < type) ++it;
        return it;
    }
};
static long g_removed[16]; static CThing* g_removedThing[16]; static int g_removeCount = 0;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(long(interfaceType + 0));
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    void RemoveTC(long interfaceType) { g_removed[g_removeCount] = interfaceType; g_removedThing[g_removeCount++] = this; }
};
class CThingCreatureBase : public CThing { public: };
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
static CThing* g_perceived = 0; static int g_perceivedCalls = 0;
struct CTCPerception : CTCBase {
    CThing* GetPerceivedThing() { ++g_perceivedCalls; return g_perceived; }
};
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) myfree(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
};
struct CThingFilter_NearCreature {
    CThingCreatureBase* PCreature;
    CThingFilter_NearCreature(CThingCreatureBase* creature) : PCreature(creature) { }
};
static CThing* g_found[4]; static int g_foundCount = 0; static CThingCreatureBase* g_filterCreature = 0;
class CThingSearchTools { public:
    template <class F> void GetAllThings(const F& filter, vector<CThing*>& things)
    {
        g_filterCreature = filter.PCreature;
        if (g_foundCount) {
            things._M_start = (CThing**)malloc(g_foundCount * sizeof(CThing*));
            for (int i = 0; i < g_foundCount; ++i) things._M_start[i] = g_found[i];
            things._M_finish = things._M_start + g_foundCount;
            things._M_end_of_storage = things._M_finish;
        }
    }
};
class CWorld { public: unsigned char _pad00[0x20]; CThingSearchTools* ThingSearchTools; };
static CWorld g_world; static CThingSearchTools g_tools;
struct CModeJackOfBladesFlash {
    unsigned char           _pad00[0x68];
    long                    Timer;
    long                    BurnCounter;
    unsigned char           Filter[0xc];
    CThingCreatureBase*     PCreature;
    CBaseIntelligentPointer PTarget;
    CBaseIntelligentPointer PEmitter;
    CBaseIntelligentPointer PSpotLight;
    CWorld* GetWorld() { return &g_world; }
    void ExtinguishFlames();
};

void CModeJackOfBladesFlash::ExtinguishFlames()
{
    if (IS_THING_ALIVE(PTarget) && PCreature->HasTC(0x34)) {
        CTCPerception* perception = PCreature->GetTC<CTCPerception>(0x34);
        CThing* perceived = perception->GetPerceivedThing();
        if (perceived && !(perceived->m_flags91 & 1)) {
            CThing* target = perception->GetPerceivedThing();
            if (PTarget.GetPItem() == target)
                PTarget.GetPItem()->RemoveTC(0x89);
        }
    }
    {
        CThingFilter_NearCreature filter(PCreature);
        vector<CThing*> things;
        CWorld* world = GetWorld();
        world->ThingSearchTools->GetAllThings(filter, things);
        long count = things.size();
        for (long i = 0; i < count; ++i) {
            CThing* thing = things[i];
            if (thing && !(thing->m_flags91 & 1))
                thing->RemoveTC(0x89);
        }
    }
}
static CThingCreatureBase g_creature; static CInterfaceMap::SEntry g_entries[2]; static CTCPerception g_perception;
static CThing g_target, g_other, g_dead;
static void Reset() { g_removeCount = 0; g_perceivedCalls = 0; g_frees = 0; g_filterCreature = 0; }
int main()
{
    g_world.ThingSearchTools = &g_tools;
    memset(&g_creature, 0, sizeof(g_creature)); memset(&g_target, 0, sizeof(g_target)); memset(&g_other, 0, sizeof(g_other)); memset(&g_dead, 0, sizeof(g_dead));
    g_dead.m_flags91 = 1;
    g_entries[0].type = 0x10; g_entries[1].type = 0x34; g_entries[1].component = &g_perception;
    g_creature.m_interfaces.first = g_entries; g_creature.m_interfaces.last = g_entries + 2;
    g_creature.m_presentBits[0x34 >> 5] |= 1UL << (0x34 & 31);
    unsigned char raw[sizeof(CModeJackOfBladesFlash)]; memset(raw, 0, sizeof(raw));
    CModeJackOfBladesFlash* m = (CModeJackOfBladesFlash*)raw;
    m->PCreature = &g_creature;
    // 1: target alive, perceived == target -> target flames removed, then the search finds two things (one dead)
    Reset(); m->PTarget.m_pItem = &g_target; g_perceived = &g_target;
    g_found[0] = &g_other; g_found[1] = &g_dead; g_found[2] = 0; g_foundCount = 3;
    m->ExtinguishFlames();
    CHECK(g_removeCount == 2); CHECK(g_removedThing[0] == &g_target); CHECK(g_removed[0] == 0x89);
    CHECK(g_removedThing[1] == &g_other); CHECK(g_removed[1] == 0x89);
    CHECK(g_perceivedCalls == 2); CHECK(g_filterCreature == &g_creature); CHECK(g_frees == 1);
    // 2: perceived differs from the target -> no target removal
    Reset(); g_perceived = &g_other; g_foundCount = 0;
    m->ExtinguishFlames();
    CHECK(g_removeCount == 0); CHECK(g_perceivedCalls == 2); CHECK(g_frees == 0);
    // 3: perceived dead -> one perception call only
    Reset(); g_perceived = &g_dead;
    m->ExtinguishFlames();
    CHECK(g_removeCount == 0); CHECK(g_perceivedCalls == 1);
    // 4: no target -> perception never asked; the search still runs
    Reset(); m->PTarget.m_pItem = 0; g_perceived = &g_target; g_found[0] = &g_other; g_foundCount = 1;
    m->ExtinguishFlames();
    CHECK(g_removeCount == 1); CHECK(g_removedThing[0] == &g_other); CHECK(g_perceivedCalls == 0); CHECK(g_frees == 1);
    // 5: dead target
    Reset(); m->PTarget.m_pItem = &g_dead; g_foundCount = 0;
    m->ExtinguishFlames();
    CHECK(g_removeCount == 0); CHECK(g_perceivedCalls == 0);
    // 6: creature without the perception component
    Reset(); m->PTarget.m_pItem = &g_target; g_creature.m_presentBits[0x34 >> 5] = 0;
    m->ExtinguishFlames();
    CHECK(g_removeCount == 0); CHECK(g_perceivedCalls == 0);
    if (fails == 0) printf("008dcb60_TEST PASS\n"); else printf("008dcb60_TEST FAIL\n");
    return fails;
}