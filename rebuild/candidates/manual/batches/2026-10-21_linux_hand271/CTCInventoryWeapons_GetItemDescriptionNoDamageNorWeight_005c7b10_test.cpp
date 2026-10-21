// Behaviour test for CTCInventoryWeapons::GetItemDescriptionNoDamageNorWeight (0x005c7b10): a weapon item gets the melee damage when its weapon class
// is melee, the ranged damage (def damage times the augmentation multiplier) otherwise; defs are released; other items
// fall back to the base description.
#include <cstdio>

struct Def {
    long refs; long destroyed; long weaponClass; long damageDefIndex; float damage;
    void Release() { if (--refs == 0) destroyed++; }
};
static Def g_thingDef, g_ability, g_damageDef, g_itemDef, g_weaponDef;
static long g_called = 0, g_baseCalled = 0; static float g_damageArg = 0;
struct Weapons {
    bool meleeAlive, rangedAlive, hasAug;
    float meleeDamage, multiplier;
    void Construct(long, float damage) { g_called++; g_damageArg = damage; }
    void Base(long) { g_baseCalled++; }
    void Run(long defIndex)
    {
        if (defIndex > 100) {                       // IsWeaponItem
            float melee = 0.0f, ranged = 0.0f;
            if (meleeAlive) melee = meleeDamage;
            if (rangedAlive) {
                Def* thingDef = &g_thingDef; ++thingDef->refs;
                Def* ability = &g_ability; ++ability->refs;      // GetSubDef
                thingDef->Release();
                Def* damageDef = &g_damageDef; ++damageDef->refs;
                ranged = damageDef->damage;
                if (hasAug) ranged *= multiplier;
                damageDef->Release();
                ability->Release();
            }
            Def* def = &g_itemDef; ++def->refs;
            Def* weapon = &g_weaponDef; ++weapon->refs;
            Construct(defIndex, weapon->weaponClass == 1 ? melee : ranged);
            weapon->Release();
            def->Release();
        } else {
            Base(defIndex);
        }
    }
};

int main()
{
    g_thingDef.refs = 1; g_ability.refs = 1; g_damageDef.refs = 1; g_itemDef.refs = 1; g_weaponDef.refs = 1;
    g_damageDef.damage = 10.0f; g_weaponDef.weaponClass = 2;
    Weapons w; w.meleeAlive = true; w.rangedAlive = true; w.hasAug = true; w.meleeDamage = 5.0f; w.multiplier = 1.5f;
    w.Run(200);
    if (g_called != 1 || g_damageArg != 15.0f) { std::printf("005c7b10_TEST FAIL\n"); return 1; }
    g_weaponDef.weaponClass = 1;
    w.Run(200);
    if (g_called != 2 || g_damageArg != 5.0f) { std::printf("005c7b10_TEST FAIL\n"); return 1; }
    w.Run(5);
    if (g_baseCalled != 1 || g_called != 2) { std::printf("005c7b10_TEST FAIL\n"); return 1; }
    if (g_thingDef.refs != 1 || g_ability.refs != 1 || g_damageDef.refs != 1 || g_itemDef.refs != 1 || g_weaponDef.refs != 1
        || g_thingDef.destroyed || g_weaponDef.destroyed) { std::printf("005c7b10_TEST FAIL\n"); return 1; }
    std::printf("005c7b10_TEST PASS\n");
    return 0;
}
