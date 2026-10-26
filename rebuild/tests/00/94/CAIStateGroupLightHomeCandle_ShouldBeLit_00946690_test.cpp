// Behaviour test for CAIStateGroup_LightHomeCandle::ShouldBeLit (0x00946690): no villager or no village gives
// false; otherwise the result is the time-of-day test on the village def's window, and the def's reference
// count is balanced (destroyed only when the temporary reference was the last).
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

static long g_now = 0;
static int g_destroyed = 0;
class CVillageDef { public:
    long m_refCount;
    long LightOnTime, LightOffTime;
    virtual ~CVillageDef() {}
    virtual void Destroy() { ++g_destroyed; }
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
class CTCVillage { public: CVillageDef* PDef; };
struct CTCVillager : CTCBase {
    CTCVillage* m_pVillage;
    CTCVillage* GetPVillage() const { return m_pVillage; }
};
class CGameTimeManager { public: bool IsTimeOfDayInRange(long a, long b) { return g_now >= a && g_now < b; } };
class CWorld { public: CGameTimeManager* m_pTimeManager; CGameTimeManager* GetPTimeManager() { return m_pTimeManager; } };
static CGameTimeManager g_time; static CWorld g_world;
class CAIStateGroup_LightHomeCandle { public:
    CAIBrain* PBrain;
    CWorld* GetPThingWorld() { return &g_world; }
    bool ShouldBeLit()
    {
        CTCVillager* villager = 0;   // (retail reads garbage on the miss path; the test seeds it)
        PBrain->PCreature->GetInterface(&villager, 0x23);
        if (villager && villager->GetPVillage()) {
            CVillageDef* def = villager->GetPVillage()->PDef;
            if (def)
                def->AddRef();
            CWorld* world = GetPThingWorld();
            bool lit = world->GetPTimeManager()->IsTimeOfDayInRange(def->LightOnTime, def->LightOffTime);
            def->Release();
            return lit;
        }
        return false;
    }
};

int main()
{
    g_world.m_pTimeManager = &g_time;
    CVillageDef def; def.m_refCount = 1; def.LightOnTime = 18; def.LightOffTime = 23;
    CTCVillage village; village.PDef = &def;
    CTCVillager villager; villager.m_pVillage = 0;
    CInterfaceMap::SEntry e[1]; e[0].type = 0x23; e[0].component = &villager;
    CThing creature; creature.m_interfaces.first = e; creature.m_interfaces.last = e + 1;
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_LightHomeCandle g; g.PBrain = &brain;
    bool ok = g.ShouldBeLit() == false;                       // not a villager
    creature.SetBit(0x23); ok = ok && g.ShouldBeLit() == false;   // no village
    villager.m_pVillage = &village;
    g_now = 20; ok = ok && g.ShouldBeLit() == true && def.m_refCount == 1 && g_destroyed == 0;
    g_now = 10; ok = ok && g.ShouldBeLit() == false && def.m_refCount == 1;
    if (!ok) { std::printf("00946690_TEST FAIL\n"); return 1; }
    std::printf("00946690_TEST PASS\n");
    return 0;
}