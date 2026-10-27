// 00661fc0 behaviour test for CThingCreatureBase::IsIndestructible() const
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
class CThingCreatureDef {
public:
    bool LeaveDeadCreature : 1;
    bool AbleToStrafe : 1;
    bool CreateDummyOnDeath : 1;
    bool ImmuneToAttack : 1;
    bool UseMeleeWeaponAsDefault : 1;
    bool UnsheatheWeaponsAutomatically : 1;
    bool IsWillable : 1;
    bool IsIndestructible : 1;
};
class CThingCreatureBase {
public:
    CThingCreatureDef* PDef_Object;
    const CThingCreatureDef* PeekDef() const { return PDef_Object; }
    bool IsIndestructible() const;
};
bool CThingCreatureBase::IsIndestructible() const
{
    return PeekDef()->IsIndestructible;
}
static void run()
{
    CThingCreatureDef def;
    def.LeaveDeadCreature = true; def.AbleToStrafe = true; def.CreateDummyOnDeath = true; def.ImmuneToAttack = true;
    def.UseMeleeWeaponAsDefault = true; def.UnsheatheWeaponsAutomatically = true; def.IsWillable = true; def.IsIndestructible = false;
    CThingCreatureBase c; c.PDef_Object = &def;
    CHECK(!c.IsIndestructible());
    def.IsIndestructible = true; def.IsWillable = false;
    CHECK(c.IsIndestructible());
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00661fc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00661fc0_TEST PASS\n");
    return 0;
}