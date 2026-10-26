// Behaviour test for CThingCreatureBase::UnsheatheMeleeWeapon (0x006649C0)
#include <cstdio>
class CThingCreatureDef { public: long DefaultMeleeWeapon; long DefaultSecondaryMeleeWeapon; long DefaultRangedWeapon; };
static long g_log[8]; static int g_n;
class CThingCreatureBase;
static bool IsThingCarryingMeleeWeapon(const CThingCreatureBase& creature);
static bool IsThingCarryingRangedWeapon(const CThingCreatureBase& creature);
class CThingCreatureBase { public:
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
#define CHECK(c) if (!(c)) { std::printf("006649c0_TEST FAIL\n"); return 1; }
int main()
{
    CThingCreatureDef def = { 3, 4, 5 };
    CThingCreatureBase c; c.m_pDef = &def; c.m_unsheathed = true; c.m_carrying = false;
    g_n = 0; c.UnsheatheMeleeWeapon(); CHECK(g_n == 0);
    c.m_unsheathed = false; c.m_carrying = true;
    g_n = 0; c.UnsheatheMeleeWeapon(); CHECK(g_n == 0);
    c.m_carrying = false;
    g_n = 0; c.UnsheatheMeleeWeapon();
    CHECK(g_n == 3 && g_log[0] == -1 && g_log[1] == 31 && g_log[2] == 40);
    std::printf("006649c0_TEST PASS\n");
    return 0;
}