// Behaviour test for CThingCreatureBase::UnsheatheRangedWeapon (0x00664A10)
#include <cstdio>
struct CThingCreatureDef { long DefaultMeleeWeapon; long DefaultSecondaryMeleeWeapon; long DefaultRangedWeapon; };
static long g_log[8]; static int g_n;
struct CThingCreatureBase;
static bool IsThingCarryingMeleeWeapon(const CThingCreatureBase& creature);
static bool IsThingCarryingRangedWeapon(const CThingCreatureBase& creature);
struct CThingCreatureBase {
    bool m_unsheathed; bool m_carrying;
    CThingCreatureDef* m_pDef;
    virtual bool AreWeaponsUnsheathed() { return m_unsheathed; }
    void SheatheAllWeapons() { g_log[g_n++] = -1; }
    void UnsheatheWeapon(long weaponDef, bool primary) { g_log[g_n++] = weaponDef * 10 + (primary ? 1 : 0); }
    void UnsheatheMeleeWeapon()
    {
        if (AreWeaponsUnsheathed()) return;
        if (IsThingCarryingMeleeWeapon(*this)) return;
        SheatheAllWeapons();
        UnsheatheWeapon(m_pDef->DefaultMeleeWeapon, true);
        UnsheatheWeapon(m_pDef->DefaultSecondaryMeleeWeapon, false);
    }
    void UnsheatheRangedWeapon()
    {
        if (AreWeaponsUnsheathed()) return;
        if (IsThingCarryingRangedWeapon(*this)) return;
        SheatheAllWeapons();
        UnsheatheWeapon(m_pDef->DefaultRangedWeapon, true);
    }
};
static bool IsThingCarryingMeleeWeapon(const CThingCreatureBase& creature) { return creature.m_carrying; }
static bool IsThingCarryingRangedWeapon(const CThingCreatureBase& creature) { return creature.m_carrying; }
#define CHECK(c) if (!(c)) { std::printf("00664a10_TEST FAIL\n"); return 1; }
int main()
{
    CThingCreatureDef def = { 3, 4, 5 };
    CThingCreatureBase c; c.m_pDef = &def; c.m_unsheathed = true; c.m_carrying = false;
    g_n = 0; c.UnsheatheRangedWeapon(); CHECK(g_n == 0);
    c.m_unsheathed = false; c.m_carrying = true;
    g_n = 0; c.UnsheatheRangedWeapon(); CHECK(g_n == 0);
    c.m_carrying = false;
    g_n = 0; c.UnsheatheRangedWeapon();
    CHECK(g_n == 2 && g_log[0] == -1 && g_log[1] == 51);
    std::printf("00664a10_TEST PASS\n");
    return 0;
}
