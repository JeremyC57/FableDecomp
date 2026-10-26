// Behaviour test for 0x0069AD10 CGameCameraManager::PushBuildingCameraMode: mode removal, building lookup, the set def
// handed over and released, melee re-entry, and the in-building flag on every path.
#include <cstdio>
static int g_removed = -1, g_destroyed = 0, g_set = 0, g_melee = 0, g_inMelee = 0;
class CPlayer { public: void RemoveMode(long mode) { g_removed = mode; } };
class CPlayerManager { public: CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
class CEngine { public: CPlayerManager* m_pPlayerManager; };
static CEngine* g_pEngine;
class CCameraManagerSetDef { public:
    long m_count;
    virtual ~CCameraManagerSetDef() {}
    virtual void Destroy() { ++g_destroyed; }
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(T* o) : Object(o) { if (Object) ++Object->m_count; }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_count; }
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
};
class CThing { public: unsigned char m_dead : 1; unsigned char m_91_1 : 7; CThing* building; };
class CThingBuilding : public CThing { public:
    CCameraManagerSetDef* setDef;
    CDefPointer<CCameraManagerSetDef> PeekPCameraManagerSetDefinition() { return CDefPointer<CCameraManagerSetDef>(setDef); }
};
static CThingBuilding* GetPBuildingThingOccupying(CThing* thing) { return (CThingBuilding*)thing->building; }
struct CBaseIntelligentPointer { CThing* item; CThing* GetPItem() const { return item; } };
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); } };
class CGameCameraManager {
public:
    bool ObservedThingInBuilding;
    CIntelligentPointer<CThing> PObservedThing;
    CCameraManagerSetDef* lastSet;
    void SetCameraManagerSetDef(const CDefPointer<CCameraManagerSetDef>& def) { ++g_set; lastSet = def.Object; }
    bool IsObservedThingInMelee() { return g_inMelee != 0; }
    void AddMeleeCameraMode() { ++g_melee; }
    void PushBuildingCameraMode();
};
void CGameCameraManager::PushBuildingCameraMode()
{
    g_pEngine->m_pPlayerManager->GetMainPlayer()->RemoveMode(0x2f);
    if (PObservedThing.GetPItem()) {
        CThingBuilding* building = GetPBuildingThingOccupying(PObservedThing.GetPItem());
        if (building) {
            if (!building->m_dead) {
                CDefPointer<CCameraManagerSetDef> def = building->PeekPCameraManagerSetDefinition();
                SetCameraManagerSetDef(def);
                if (IsObservedThingInMelee())
                    AddMeleeCameraMode();
            }
        }
    }
    ObservedThingInBuilding = true;
}
int main()
{
    CPlayerManager pm; CEngine engine = { &pm }; g_pEngine = &engine;
    CCameraManagerSetDef setDef; setDef.m_count = 1;
    CThingBuilding house; house.m_dead = 0; house.building = 0; house.setDef = &setDef;
    CThing hero; hero.m_dead = 0; hero.building = 0;
    CGameCameraManager cm; cm.ObservedThingInBuilding = false; cm.PObservedThing.item = 0; cm.lastSet = 0;
    int fail = 0;
    cm.PushBuildingCameraMode();
    if (g_removed != 0x2f || !cm.ObservedThingInBuilding || g_set) fail |= 1;
    cm.ObservedThingInBuilding = false; cm.PObservedThing.item = &hero;
    cm.PushBuildingCameraMode();                       // not in a building
    if (!cm.ObservedThingInBuilding || g_set) fail |= 2;
    hero.building = &house; house.m_dead = 1; cm.ObservedThingInBuilding = false;
    cm.PushBuildingCameraMode();                       // dead building
    if (!cm.ObservedThingInBuilding || g_set) fail |= 4;
    house.m_dead = 0; g_inMelee = 1;
    cm.PushBuildingCameraMode();
    if (g_set != 1 || cm.lastSet != &setDef || g_melee != 1 || setDef.m_count != 1 || g_destroyed) fail |= 8;
    if (fail) { std::printf("0069ad10_TEST FAIL\n"); return 1; }
    std::printf("0069ad10_TEST PASS\n");
    return 0;
}