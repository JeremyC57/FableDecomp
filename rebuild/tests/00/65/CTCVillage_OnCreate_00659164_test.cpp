// behaviour test for 00659164 CTCVillage::OnCreate: village def from the thing def, stance copied, respawn/curfew cleared,
// the guard respawn scheduled as ratio * people-def offset for a live hero with interface 0x14, the light state the inverse
// of IsDay, MaxFrameToAssignTask -1, ResetCrier called, both looked-up defs released
#include <cstdio>
class CThing;
class CTCBase;
static int g_destroyed = 0, g_crier = 0;
struct CDefBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CVillageDef : public CDefBase { public: long DefaultStanceToHero; };
class CVillagePeopleDef : public CDefBase { public: long GuardRespawnMaxRenownBasedOffset; };
static CVillageDef g_villageDef; static CVillagePeopleDef g_peopleDef;
struct CThingDef : CDefBase {
    bool GetSubDef(CDefPointer<CVillageDef>* out) { ++g_villageDef.m_count; out->Object = &g_villageDef; return true; }
    bool GetSubDef(CDefPointer<CVillagePeopleDef>* out) { ++g_peopleDef.m_count; out->Object = &g_peopleDef; return true; }
};
static CThingDef g_thingDef; static long g_requestedIndex = -1;
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingDef>* out) const { g_requestedIndex = index; ++g_thingDef.m_count; out->Object = &g_thingDef; return true; }
};
static CDefinitionManager g_defs;
struct CTCRenownRatio { float ratio; float CalculateRatio() { return ratio; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    unsigned long  m_bits;
    CInterfaceMap  m_interfaces;
    unsigned short DefIndex;
    unsigned char  Flags91;
    bool HasTC(long type) const { return type == 0x14 && (m_bits & 1); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
bool IS_THING_ALIVE(const CThing* const& thing)
{
    if (thing == 0)
        return false;
    return (thing->Flags91 & 1) == 0;
}
class CGameTimeManager { public: bool day; bool IsDay() const { return day; } };
static CThing* g_hero = 0;
class CPlayer { public: CThing* GetCharacterThing() { return g_hero; } };
class CWorld { public: CGameTimeManager* PGameTimeManager; CPlayer player; CPlayer* GetCurrentPlayer() const { return (CPlayer*)&player; } };
static CGameTimeManager g_time; static CWorld g_world;
class CMainGameComponent { public: CWorld* World; };
static CMainGameComponent g_main; CMainGameComponent* g_pMainGameComponent = &g_main;
class CTCVillage { public:
    virtual void v00() {}
    CThing*       ParentThing;
    bool          InCurfew;
    CDefPointer<CVillageDef> PDef;
    long          StanceToHero;
    long          NextGuardRespawn;
    bool          LightActiveStateToChange;
    long          MaxFrameToAssignTask;
    const CDefinitionManager* GetDefinitions() const { return &g_defs; }
    const CWorld* GetWorld() const { return &g_world; }
    void ResetCrier() { ++g_crier; }
    virtual void OnCreate();
};
void CTCVillage::OnCreate()
{
    CDefPointer<CThingDef> thingDef;
    GetDefinitions()->GetDef(ParentThing->DefIndex, &thingDef);
    thingDef->GetSubDef(&PDef);
    StanceToHero = PDef->DefaultStanceToHero;
    NextGuardRespawn = 0;
    InCurfew = false;
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetCharacterThing();
    CDefPointer<CVillagePeopleDef> peopleDef;
    if (thingDef->GetSubDef(&peopleDef) && IS_THING_ALIVE(hero) && hero->HasTC(0x14))
        NextGuardRespawn = (long)(((CTCRenownRatio*)hero->GetTC(0x14))->CalculateRatio()
                                  * peopleDef->GuardRespawnMaxRenownBasedOffset);
    LightActiveStateToChange = !GetWorld()->PGameTimeManager->IsDay();
    MaxFrameToAssignTask = -1;
    ResetCrier();
}
int main()
{
    g_main.World = &g_world; g_world.PGameTimeManager = &g_time; g_time.day = true;
    g_villageDef.DefaultStanceToHero = 3; g_peopleDef.GuardRespawnMaxRenownBasedOffset = 200;
    CTCRenownRatio ratio; ratio.ratio = 0.25f;
    CInterfaceMap::SEntry entries[1] = { { 0x14, (CTCBase*)&ratio } };
    CThing hero; hero.m_bits = 1; hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 1; hero.Flags91 = 0;
    g_hero = &hero;
    CThing villageThing; villageThing.DefIndex = 42;
    CTCVillage village; village.ParentThing = &villageThing; village.InCurfew = true; village.NextGuardRespawn = 9;
    village.OnCreate();
    bool ok = g_requestedIndex == 42 && village.PDef.Object == &g_villageDef && village.StanceToHero == 3 && !village.InCurfew &&
              village.NextGuardRespawn == 50 && !village.LightActiveStateToChange && village.MaxFrameToAssignTask == -1 &&
              g_crier == 1 && g_thingDef.m_count == 0 && g_peopleDef.m_count == 0 && g_villageDef.m_count == 1;
    hero.Flags91 = 1; g_time.day = false;                  // dead hero, night
    village.PDef.Object = 0; g_villageDef.m_count = 0;
    village.OnCreate();
    ok = ok && village.NextGuardRespawn == 0 && village.LightActiveStateToChange && g_crier == 2;
    if (!ok) { std::printf("00659164_TEST FAIL\n"); return 1; }
    std::printf("00659164_TEST PASS\n");
    return 0;
}