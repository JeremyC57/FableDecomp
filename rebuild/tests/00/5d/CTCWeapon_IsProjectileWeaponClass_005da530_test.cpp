// 005da530 behaviour test for CTCWeapon::IsProjectileWeaponClass(EWeaponClass)
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
enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_MELEE_1 = 1, WEAPON_CLASS_MELEE_2 = 2, WEAPON_CLASS_PROJECTILE = 3 };
class CTCWeapon {
public:
    static bool __fastcall IsProjectileWeaponClass(EWeaponClass weapon_class);
};
bool __fastcall CTCWeapon::IsProjectileWeaponClass(EWeaponClass weapon_class)
{
    return weapon_class == WEAPON_CLASS_PROJECTILE;
}
static void run()
{
    CHECK(CTCWeapon::IsProjectileWeaponClass(WEAPON_CLASS_PROJECTILE));
    CHECK(!CTCWeapon::IsProjectileWeaponClass(WEAPON_CLASS_NONE));
    CHECK(!CTCWeapon::IsProjectileWeaponClass(WEAPON_CLASS_MELEE_1));
    CHECK(!CTCWeapon::IsProjectileWeaponClass(WEAPON_CLASS_MELEE_2));
    CHECK(!CTCWeapon::IsProjectileWeaponClass((EWeaponClass)4));
}

int main()
{
    run();
    if (g_fail) {
        std::printf("005da530_TEST FAIL\n");
        return 1;
    }
    std::printf("005da530_TEST PASS\n");
    return 0;
}