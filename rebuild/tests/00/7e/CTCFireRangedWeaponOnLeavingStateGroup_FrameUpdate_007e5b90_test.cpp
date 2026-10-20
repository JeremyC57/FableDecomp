// Behaviour test for CTCFireRangedWeaponOnLeavingStateGroup::FrameUpdate (retail 0x007E5B90): model copy + stub callees.
#include <cstdio>

class CTCBase;
struct CCombatActionBase { int id; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};

static long g_removed = 0;

class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char Flags91;
    void RemoveTC(long interfaceType) { g_removed = interfaceType; }
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
    template<class T> bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing {
public:
    bool registered, busy, accepts;
    CCombatActionBase* current;
    bool IsRegisteredCombatant() { return registered; }
    bool IsPerformingAnyAction() { return busy; }
    bool SetCurrentAction(CCombatActionBase* action) { if (!accepts) return false; current = action; return true; }
};

struct CTCCarrying { CThing* primary; CThing* GetThingInPrimarySlot() { return primary; } };
struct CTCWeapon {
    bool projectile, loaded, fire;
    bool IsProjectileWeapon() { return projectile; }
    bool IsProjectileWeaponLoaded() { return loaded; }
    void SetFireRequested() { fire = true; }
};

static int g_attemptCalls = 0;
static bool g_attemptFlag = false;
struct CCombatManager {
    CCombatActionBase* next;
    CCombatActionBase* GetNextActionForCreature(CThingCreatureBase*) { return next; }
    void SetCreatureAsAttemptedToPerformedAction(CThingCreatureBase*, bool performed) { ++g_attemptCalls; g_attemptFlag = performed; }
};
struct CWorld {
    CCombatManager* PCombatManager;
    CCombatManager* GetPCombatManager() { return PCombatManager; }
};
static CWorld* g_world = 0;

class CTCBase {
public:
    CThing* ParentThing;
    CWorld* GetWorld() { return g_world; }
};

class CTCFireRangedWeaponOnLeavingStateGroup : public CTCBase {
public:
    bool QuitOut;
    void FrameUpdate();
};

void CTCFireRangedWeaponOnLeavingStateGroup::FrameUpdate()
{
    if (!QuitOut) {
        const CThing* thing = ParentThing;
        QuitOut = true;
        CTCCarrying* carrying;
        if (thing->GetInterface(&carrying, 0x46)) {
            CThing* item = carrying->GetThingInPrimarySlot();
            CTCWeapon* weapon;
            if (item && !(item->Flags91 & 1) && item->GetInterface(&weapon, 0x48)) {
                if (weapon->IsProjectileWeapon() && weapon->IsProjectileWeaponLoaded()) {
                    weapon->SetFireRequested();
                    QuitOut = false;
                }
            }
        }
    }
    if (!QuitOut) {
        CThingCreatureBase* creature = (CThingCreatureBase*)ParentThing;
        if (creature->IsRegisteredCombatant()) {
            CCombatActionBase* action = GetWorld()->GetPCombatManager()->GetNextActionForCreature(creature);
            if (creature->IsRegisteredCombatant()) {
                bool performed = false;
                if (action && !creature->IsPerformingAnyAction() && creature->SetCurrentAction(action))
                    performed = true;
                CWorld* world = GetWorld();
                world->PCombatManager->SetCreatureAsAttemptedToPerformedAction(creature, performed);
            }
        }
    }
    ParentThing->RemoveTC(0x104);
}

static void ClearThing(CThing& t) { for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_interfaces.first = t.m_interfaces.last = 0; t.Flags91 = 0; }

int main()
{
    int fail = 0;
    CCombatActionBase act = { 7 };
    CCombatManager cm; cm.next = &act;
    CWorld world; world.PCombatManager = &cm; g_world = &world;

    CTCWeapon weapon = { true, true, false };
    CThing bow; ClearThing(bow);
    CInterfaceMap::SEntry bowEntries[1] = { { 0x48, (CTCBase*)&weapon } };
    bow.m_presentBits[2] = 1UL << 8; bow.m_interfaces.first = bowEntries; bow.m_interfaces.last = bowEntries + 1;

    CTCCarrying carrying; carrying.primary = &bow;
    CThingCreatureBase hero; ClearThing(hero);
    CInterfaceMap::SEntry heroEntries[2] = { { 0x10, 0 }, { 0x46, (CTCBase*)&carrying } };
    hero.m_presentBits[2] = 1UL << 6; hero.m_interfaces.first = heroEntries; hero.m_interfaces.last = heroEntries + 2;
    hero.registered = true; hero.busy = false; hero.accepts = true; hero.current = 0;

    CTCFireRangedWeaponOnLeavingStateGroup tc; tc.ParentThing = &hero; tc.QuitOut = false;

    // 1. loaded projectile weapon: fire it, QuitOut dropped, then the combat manager is asked
    tc.FrameUpdate();
    if (!weapon.fire || tc.QuitOut || g_removed != 0x104) fail = 1;
    if (g_attemptCalls != 1 || !g_attemptFlag || hero.current != &act) fail = 2;

    // 2. weapon not loaded: QuitOut stays raised, no combat call
    tc.QuitOut = false; weapon.loaded = false; weapon.fire = false; g_removed = 0;
    tc.FrameUpdate();
    if (weapon.fire || !tc.QuitOut || g_attemptCalls != 1 || g_removed != 0x104) fail = 3;

    // 3. already quit: only the component removal
    g_removed = 0; tc.FrameUpdate();
    if (g_attemptCalls != 1 || g_removed != 0x104) fail = 4;

    // 4. dead bow: quit
    tc.QuitOut = false; weapon.loaded = true; bow.Flags91 = 1;
    tc.FrameUpdate();
    if (weapon.fire || !tc.QuitOut) fail = 5;
    bow.Flags91 = 0;

    // 5. creature busy: performed flag false
    tc.QuitOut = false; hero.busy = true; hero.current = 0;
    tc.FrameUpdate();
    if (g_attemptCalls != 2 || g_attemptFlag || hero.current != 0) fail = 6;

    // 6. not a registered combatant: no combat call
    tc.QuitOut = false; hero.registered = false;
    tc.FrameUpdate();
    if (g_attemptCalls != 2) fail = 7;

    // 7. no carrying component: quit
    hero.registered = true; hero.busy = false; hero.m_presentBits[2] = 0; tc.QuitOut = false; weapon.fire = false;
    tc.FrameUpdate();
    if (!tc.QuitOut || weapon.fire || g_attemptCalls != 2) fail = 8;

    if (fail) { std::printf("007e5b90_TEST FAIL\n"); return 1; }
    std::printf("007e5b90_TEST PASS\n");
    return 0;
}