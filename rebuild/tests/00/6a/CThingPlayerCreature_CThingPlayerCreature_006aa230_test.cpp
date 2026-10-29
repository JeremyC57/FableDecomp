// Behaviour test for CThingPlayerCreature::~CThingPlayerCreature @ 0x006aa230: a creature in melee is deregistered from
// the combat manager as a destroyed combatant (one not in melee is not); then the def pointers are released in reverse
// order and the acceleration map is cleared.
#include <stdio.h>
static char g_log[64];
static void logc(char c) { int n = 0; while (g_log[n]) ++n; g_log[n] = c; g_log[n + 1] = 0; }
class CThingCreatureBase;
static const CThingCreatureBase* g_deregistered = 0;
class CCombatManager { public:
    void DeregisterDestroyedCombatant(CThingCreatureBase& c) { g_deregistered = &c; logc('D'); }
};
static CCombatManager g_combat;
class CDefPointee { public:
    long RefCount; char Tag;
    void Last() const { logc(Tag); }
};
class CDefPointer { public:
    CDefPointee* Object;
    ~CDefPointer() { CDefPointee* o = Object; if (o) { if (--o->RefCount == 0) o->Last(); Object = 0; } }
};
class CMap { public: int Count; ~CMap() { if (Count) logc('m'); Count = 0; } };
class CThingCreatureBase { public:
    unsigned char MeleeAttacker : 1;
    unsigned char MeleeAttacked : 1;
    virtual ~CThingCreatureBase() { logc('B'); }
    bool IsInMelee() const { return MeleeAttacker || MeleeAttacked; }
    CCombatManager& GetCombatManager() { return g_combat; }
};
class CThingPlayerCreature : public CThingCreatureBase { public:
    CMap RelativeMovementAccelerationComponents;
    CDefPointer PHeroCombatDef;
    CDefPointer PPlayerMovementDef;
    virtual ~CThingPlayerCreature()
    {
        if (IsInMelee())
            GetCombatManager().DeregisterDestroyedCombatant(*this);
    }
};
static int check(bool attacker, bool attacked, const char* want, bool deregistered)
{
    CDefPointee combat; combat.RefCount = 1; combat.Tag = 'c';
    CDefPointee movement; movement.RefCount = 1; movement.Tag = 'v';
    CThingPlayerCreature* p = new CThingPlayerCreature;
    p->MeleeAttacker = attacker; p->MeleeAttacked = attacked;
    p->RelativeMovementAccelerationComponents.Count = 2;
    p->PHeroCombatDef.Object = &combat; p->PPlayerMovementDef.Object = &movement;
    g_log[0] = 0; g_deregistered = 0;
    CThingCreatureBase* b = p; delete b;
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) return 0;
    if ((g_deregistered != 0) != deregistered) return 0;
    return 1;
}
int main()
{
    if (!check(true, false, "DvcmB", true) || !check(false, true, "DvcmB", true) || !check(false, false, "vcmB", false)) {
        printf("006aa230_TEST FAIL\n"); return 1;
    }
    printf("006aa230_TEST PASS\n");
    return 0;
}